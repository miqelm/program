/******************************************************************************
 * INCLUDE SECTION
 *****************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "i2c-dev.h"
#include "defines.h"
#include "bmp085.h"

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/
static uint16_t read_i2c(int file, uint8_t reg);
static uint16_t read_raw_temp(int file);
static uint32_t read_raw_pres(int file);
static void read_compensated_data(uint16_t raw_temp,
								  uint32_t raw_pres,
								  uint16_t *compensation_table,
								  float *temp,
								  float *pres);
static status check_compensation_data(uint16_t *compensation_table);
static status get_compensation_data(int file, uint16_t *compensation_table);

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
status bmp085_read(float *temperature, float *pressure)
{
	status RetVal = ERR_NONE;
	int temp = 0;
	int pres = 0;
	FILE *file;
	if(NULL != temperature && NULL != pressure)
	{
		file = fopen("/sys/bus/i2c/drivers/bmp085/1-0077/temp0_input", "r");
		if(NULL != file)
		{
			fscanf(file, "%d", &temp);
			fclose(file);
			file = fopen("/sys/bus/i2c/drivers/bmp085/1-0077/pressure0_input", "r");
			if(NULL != file)
			{
				fscanf(file, "%d", &pres);
				fclose(file);
				*temperature = (float)temp/10;
				*pressure = (float)pres/100;
			}
			else
			{
				RetVal = ERR_OPEN_FILE;
			}
		}
		else
		{
			RetVal = ERR_OPEN_FILE;
		}
	}
	else
	{
		RetVal = ERR_NULL_POINTER;
	}
	return RetVal;
}

status bmp085_read2(float *temperature, float *pressure)
{
	status RetVal = ERR_NONE;
	int file;
	uint16_t compensation_table[CALIBRATION_WORDS] = {0};
	uint16_t raw_temp = 0;
	uint32_t raw_pres = 0;
	if(NULL != temperature && NULL != pressure)
	{
		file = open("/dev/i2c-1", O_RDWR);
		if(-1 != file)
		{
			if(-1 != ioctl(file, I2C_SLAVE, BMP085_ADDRESS))
			{
				RetVal = get_compensation_data(file, compensation_table);
				if(ERR_NONE == RetVal)
				{
					raw_temp = read_raw_temp(file);
					raw_pres = read_raw_pres(file);
					read_compensated_data(raw_temp,
										  raw_pres,
										  compensation_table,
										  temperature,
										  pressure);
				}
			}
			else
			{
				RetVal = ERR_IOCTL;
			}
		}
		else
		{
			RetVal = ERR_OPEN_FILE;
		}
	}
	else
	{
		RetVal = ERR_NULL_POINTER;
	}
	return RetVal;
}

/******************************************************************************
 * STATIC FUNCTION DEFINITIONS
 *****************************************************************************/
static uint16_t read_i2c(int file, uint8_t reg)
{
	uint16_t RetVal;
	RetVal = i2c_smbus_read_word_data(file, reg);
	RetVal = ((0xFF & RetVal) << 8) | ((0xFF00 & RetVal) >> 8);
	return RetVal;
}

static uint16_t read_raw_temp(int file)
{
	uint16_t RetVal;
	i2c_smbus_write_byte_data(file, TEMP_PRES_REQUEST_ADDR, TEMP_REQUEST_COMMAND);
	usleep(50000); // wait at least 4,5 ms
	RetVal = read_i2c(file, TEMP_PRES_ADDRESS);
	return RetVal;
}

static uint32_t read_raw_pres(int file)
{
	uint32_t RetVal;
	i2c_smbus_write_byte_data(file, TEMP_PRES_REQUEST_ADDR, PRES_REQUEST_COMMAND + ((uint8_t)BMP085_MODE << 6));
	usleep(50000); // wait at least 4,5 ms
	RetVal = (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS) << 16;
	RetVal |= (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS + 1) << 8;
	RetVal |= (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS + 2);
	RetVal >>= (8 - BMP085_MODE);
	return RetVal;
}

static void read_compensated_data(uint16_t raw_temp,
								  uint32_t raw_pres,
								  uint16_t *compensation_table,
								  float *temp,
								  float *pres)
{
	long X1 = (raw_temp - AC6) * AC5 / (1 << 15);
	long X2 = (short) MC * (1 << 11) / (X1 + (short) MD);
	long B5 = X1 + X2;
	*temp = (float)((B5 + 8) / (1 << 4)) / 10;
	long B6 = B5 - 4000;
	X1 = (B2 * (B6 * B6 / (1 << 12))) / (1 << 11);
	X2 = AC2 * B6 / (1 << 11);
	long X3 = X1 + X2;
	long B3 = (((AC1 * 4 + X3) << BMP085_MODE) + 2) / 4;
	X1 = AC3 * B6 / (1 << 13);
	X2 = (B1 * (B6 * B6 / (1 << 12))) / (1 << 16);
	X3 = ((X1 + X2) + 2) / 4;
	unsigned long B4 = AC4 * ((unsigned long)(X3 + 32768)) / (1 << 15);
	unsigned long B7 = ((unsigned long)raw_pres - B3) * (50000 >> BMP085_MODE);
	long p;
	if(B7 < 0x80000000)
	{
		p = (B7 * 2) / B4;
	}
	else
	{
		p = (B7 / B4) * 2;
	}
	X1 = (p / (1 << 8)) * (p / (1 << 8));
	X1 = (X1 * 3038) / (1 << 16);
	X2 = (-7357 * p) / (1 << 16);
	*pres = (float)(p + (X1 + X2 + 3791) / (1 << 4)) / 100;
}

static status check_compensation_data(uint16_t *compensation_table)
{
	status RetVal = ERR_NONE;

	for(uint8_t i = 0; (i < CALIBRATION_WORDS) && (ERR_NONE == RetVal); ++i)
	{
		if((0 == compensation_table[i]) || (0xFFFF == compensation_table[i]))
		{
			RetVal = ERR_COMPENSATION_DATA;
		}
	}
	return RetVal;
}

static status get_compensation_data(int file, uint16_t *compensation_table)
{
	status RetVal;
	AC1 = read_i2c(file, AC1_ADDRESS);
	AC2 = read_i2c(file, AC2_ADDRESS);
	AC3 = read_i2c(file, AC3_ADDRESS);
	AC4 = read_i2c(file, AC4_ADDRESS);
	AC5 = read_i2c(file, AC5_ADDRESS);
	AC6 = read_i2c(file, AC6_ADDRESS);
	B1 = read_i2c(file, B1_ADDRESS);
	B2 = read_i2c(file, B2_ADDRESS);
	MB = read_i2c(file, MB_ADDRESS);
	MC = read_i2c(file, MC_ADDRESS);
	MD = read_i2c(file, MD_ADDRESS);
	RetVal = check_compensation_data(compensation_table);
	return RetVal;
}
