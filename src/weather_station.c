#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "defines.h"
#include "gpio.h"
#include "dht22.h"
#include "bmp085.h"

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
				printf("DHT-22: T: %.1f *C, H: %.1f%%\n", dht22_temperature, dht22_humidity);
				printf("BMP085: T: %.1f *C, H: %.2f hPa\n", bmp85_temperature, bmp85_pressure);
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
		sleep(1);
	}
	gpio_clean();

	return RetVal;
}
