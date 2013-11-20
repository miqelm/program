#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include "defines.h"
#include "gpio.h"
#include "dht22.h"

int main(void)
{
	status RetVal = ERR_NONE;
	float temperature, humidity;

	RetVal = gpio_init();
	if(ERR_NONE == RetVal)
	{
		RetVal = dht22_read(&temperature, &humidity);
		while(1) dht22_read(&temperature, &humidity);
		if(ERR_NONE == RetVal)
		{
			//printf("T: %.1f *C, H: %.1f%\n", temperature, humidity);
		}
		else
		gpio_clean();
	}

	return RetVal;
}
