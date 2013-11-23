#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include "defines.h"

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
