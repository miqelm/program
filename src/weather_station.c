#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "defines.h"
#include "gpio.h"
#include "dht22.h"
#include "bmp085.h"
#include "database.h"

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
int main(void)
{
	status RetVal = ERR_NONE;
	float dht22_temperature, dht22_humidity;
	float bmp85_temperature, bmp85_pressure;

	RetVal = gpio_init();
	while(ERR_NONE == RetVal)
	{
		RetVal = dht22_read(&dht22_temperature, &dht22_humidity);
		if(ERR_NONE == RetVal)
		{
			RetVal = bmp085_read(&bmp85_temperature, &bmp85_pressure);
			if(ERR_NONE == RetVal)
			{
				RetVal = add_measurments((dht22_temperature + bmp85_temperature)/2, dht22_humidity, bmp85_pressure);
				if(ERR_NONE == RetVal)
				{

				}
				else
				{
					RetVal = ERR_ADD_DATABASE;
				}
			}
			else
			{
				RetVal = ERR_READ_BMP;
			}
		}
		else
		{
			RetVal = ERR_READ_DHT;
		}
		sleep(DELAY_BETWEEN_MEASURMENTS);
	}
	gpio_clean();

	return RetVal;
}
