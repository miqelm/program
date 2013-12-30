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
static long read_i2c(int file, uint8_t reg);
static long read_raw_temp(int file);
static long read_raw_pres(int file);
static void read_compensated_data(long raw_temp,
								  long raw_pres,
								  uint16_t *compensation_table,
								  float *temp,
								  float *pres);
static status check_compensation_data(uint16_t *compensation_table);
static status get_calibration_data(int file, uint16_t *compensation_table);

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
				RetVal = get_calibration_data(file, compensation_table);
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
	sleep(1);
	return RetVal;
}

/******************************************************************************
 * STATIC FUNCTION DEFINITIONS
 *****************************************************************************/
static long read_i2c(int file, uint8_t reg)
{
	long RetVal;
	RetVal = i2c_smbus_read_word_data(file, reg);
	RetVal = ((0xFF & RetVal) << 8) | ((0xFF00 & RetVal) >> 8);
	return RetVal;
}

static long read_raw_temp(int file)
{
	long RetVal;
	i2c_smbus_write_byte_data(file, TEMP_PRES_REQUEST_ADDR, TEMP_REQUEST_COMMAND);
	usleep(30000); // wait at least 4,5 ms
	RetVal = read_i2c(file, TEMP_PRES_ADDRESS);
	return RetVal;
}

static long read_raw_pres(int file)
{
	long RetVal;
	i2c_smbus_write_byte_data(file, TEMP_PRES_REQUEST_ADDR, PRES_REQUEST_COMMAND + ((uint8_t)BMP085_MODE << 6));
	usleep(30000); // wait at least 4,5 ms
	RetVal = (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS) << 16;
	RetVal |= (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS + 1) << 8;
	RetVal |= (uint32_t)i2c_smbus_read_byte_data(file, TEMP_PRES_ADDRESS + 2);
	RetVal >>= (8 - BMP085_MODE);
	return RetVal;
}

static void read_compensated_data(long raw_temp,
								  long raw_pres,
								  uint16_t *compensation_table,
								  float *temp,
								  float *pres)
{
	long ut = raw_temp;
	long up = raw_pres;
	long x1, x2, b5, b6, x3, b3, p;
	unsigned long b4, b7;

	short ac1 = AC1;
	short ac2 = AC2;
	short ac3 = AC3;
	unsigned short ac4 = AC4;
	unsigned short ac5 = AC5;
	unsigned short ac6 = AC6;
	short b1 = B1;
	short b2 = B2;
	short mb = MB;
	short mc = MC;
	short md = MD;
	x1 = ((long)ut - ac6) * ac5 >> 15;
	x2 = ((long) mc << 11) / (x1 + md);
	b5 = x1 + x2;
	*temp = (float)((b5 + 8) >> 4) / 10;
	b6 = b5 - 4000;
	x1 = (b2 * (b6 * b6 >> 12)) >> 11;
	x2 = ac2 * b6 >> 11;
	x3 = x1 + x2;
	b3 = (((int32_t) ac1 * 4 + x3) + 2)/4;
	x1 = ac3 * b6 >> 13;
	x2 = (b1 * (b6 * b6 >> 12)) >> 16;
	x3 = ((x1 + x2) + 2) >> 2;
	b4 = (ac4 * (unsigned long) (x3 + 32768)) >> 15;
	b7 = ((unsigned long) up - b3) * (50000 >> BMP085_MODE);
	p = b7 < 0x80000000 ? (b7 * 2) / b4 : (b7 / b4) * 2;
	x1 = (p >> 8) * (p >> 8);
	x1 = (x1 * 3038) >> 16;
	x2 = (-7357 * p) >> 16;
	*pres = (float)(p + ((x1 + x2 + 3791) >> 4)) / 100;
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

static status get_calibration_data(int file, uint16_t *compensation_table)
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
