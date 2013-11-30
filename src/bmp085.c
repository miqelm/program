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
 * FUNCTION DEFINITIONS
 *****************************************************************************/
status bmp085_read(float *temperature, float *pressure)
{
	value RetVal = ERR_NONE;
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
	value RetVal = ERR_NONE;
	int temp = 0;
	int pres = 0;
	int file;
	uint16_t ac1 = 0;
	uint16_t ac2 = 0;
	uint16_t ac3 = 0;
	uint16_t ac4 = 0;
	uint16_t ac5 = 0;
	uint16_t ac6 = 0;
	uint16_t b1 = 0;
	uint16_t b2 = 0;
	uint16_t mb = 0;
	uint16_t mc = 0;
	uint16_t md = 0;
	uint16_t raw_temp = 0;
	if(NULL != temperature && NULL != pressure)
	{
		file = open("/dev/i2c-1", O_RDWR);
		if(-1 != file)
		{
			if(-1 != ioctl(file, I2C_SLAVE, BMP085_ADDRESS))
			{
				ac1 = read_i2c(file, AC1_ADDRESS);
				ac2 = read_i2c(file, AC2_ADDRESS);
				ac3 = read_i2c(file, AC3_ADDRESS);
				ac4 = read_i2c(file, AC4_ADDRESS);
				ac5 = read_i2c(file, AC5_ADDRESS);
				ac6 = read_i2c(file, AC6_ADDRESS);
				b1 = read_i2c(file, B1_ADDRESS);
				b2 = read_i2c(file, B2_ADDRESS);
				mb = read_i2c(file, MB_ADDRESS);
				mc = read_i2c(file, MC_ADDRESS);
				md = read_i2c(file, MD_ADDRESS);
				raw_temp = read_raw_temp(file);
				long X1 = (raw_temp - ac6) * ac5 / (1 << 15);
				long X2 = (short)mc * (1 << 11) / (X1 + (short)md);
				long B5 = X1 + X2;
				*temperature = (float)((B5 + 8) / (1 << 4))/10;
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
	i2c_smbus_write_byte_data(file, TEMP_REQUEST_ADDR, TEMP_REQUEST_COMMAND);
	usleep(50000); // wait at least 4,5 ms
	RetVal = read_i2c(file, TEMP_ADDRESS);
	return RetVal;
}
