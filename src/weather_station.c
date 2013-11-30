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
//	if(ERR_NONE == RetVal)
	{
		RetVal = dht22_read(&dht22_temperature, &dht22_humidity);
		if(ERR_NONE == RetVal)
		{
			RetVal = bmp085_read2(&bmp85_temperature, &bmp85_pressure);
			if(ERR_NONE == RetVal)
			{
				printf("%.1f*C, %.1f%%, %.1f*C, %.2f hPa\n",
						dht22_temperature,
						dht22_humidity,
						bmp85_temperature,
						bmp85_pressure);
//				RetVal = add_measurments((dht22_temperature + bmp85_temperature)/2, dht22_humidity, bmp85_pressure);
				if(ERR_NONE != RetVal)
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
		if(ERR_NONE == RetVal)
		{
//			sleep(DELAY_BETWEEN_MEASURMENTS);
		}
		else
		{
			check_retval(RetVal);
		}
	}
	gpio_clean();

	return RetVal;
}

void check_retval(status RetVal)
{
	switch(RetVal)
	{
		case ERR_OPEN_FILE:
			printf("Blad otwarcia pliku!\n");
			break;
		case ERR_MAP_FAIL:
			printf("Blad mapowania pamieci!\n");
			break;
		case ERR_READ_DHT:
			printf("Blad odczytu DHT-22!\n");
			break;
		case ERR_NULL_POINTER:
			printf("Blad, argument jest wskaznikiem NULL!\n");
			break;
		case ERR_READ_BMP:
			printf("Blad odczytu BMP085!\n");
			break;
		case ERR_MYSQL_INIT:
			printf("Blad inicjalizacji MySQL!\n");
			break;
		case ERR_MYSQL_CONN:
			printf("Blad polaczenie MySQL!\n");
			break;
		case ERR_MYSQL_QUERY:
			printf("Blad zapytania MySQL!\n");
			break;
		case ERR_MYSQL_STORE:
			printf("Blad przechwycenia MySQL!\n");
			break;
		case ERR_ADD_DATABASE:
			printf("Blad dodania bazy MySQL!\n");
			break;
		case ERR_IOCTL:
			printf("Blad wykonania IOCTL!\n");
			break;
		default:
			break;
	}
}
