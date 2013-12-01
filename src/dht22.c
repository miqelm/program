/******************************************************************************
 * INCLUDE SECTION
 *****************************************************************************/
#include <stdio.h>
#include <sys/time.h>
#include <stdint.h>
#include <unistd.h>
#include "defines.h"
#include "gpio.h"
#include "dht22.h"

/******************************************************************************
 * STATIC FUNCTION PROTOTYPES
 *****************************************************************************/
static uint16_t get_threshold(uint16_t data[], uint16_t size);

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
status dht22_read(float *temperature, float *humidity)
{
	status RetVal = ERR_READ_DHT;
	uint8_t data[600];
	uint16_t cnt[600];
	uint8_t bytes[5] = {0,};
	int16_t temp = 0;
	uint16_t hum = 0;
	uint8_t try = 0;
	uint16_t proper_data = 0;

	int counter = 0;
	uint16_t i = 0;
	value last_state;
	value state;
	int how1 = 0;
	if((NULL != temperature) && (NULL != humidity))
	{
		while(ERR_NONE != RetVal)
		{
			while(how1 < 42)
			{
				for(int a = 0; a < sizeof(data); ++a)
				{
					data[a] = 0xFF;
					cnt[a] = 0;
				}
				how1 = 0;
				i = 0;
				counter = 0;
				set_pin_out(P8_26);
				// Initialize DHT22 sensor
				set_pin_high(P8_26);
				usleep(500000);  // 500 ms
				set_pin_low(P8_26);
				usleep(20000);

				set_pin_in(P8_26);

				last_state = read_pin(P8_26);
				while(counter < 1000)
				{
					state = read_pin(P8_26);
					if(state == last_state)
					{
						++counter;
					}
					else
					{
						cnt[i] = counter;
						counter = 0;
						data[i] = last_state;
						last_state = state;
						++i;
					}
				}
				usleep(10000);

				for(i = 0; i < sizeof(data); ++i)
				{
					if(data[i] != 255)
					{
						if(data[i]) how1++;
					}
					else
					{
						proper_data = i;
						break;
					}
				}
				++try;
			}
			value bits[42];
			int j = 0;
			for(i = 0; i < sizeof(data); ++i)
			{
				if(data[i] != 255)
				{
					if(data[i])
					{
						bits[j] = (value)(cnt[i] > get_threshold(cnt, proper_data));
						j++;
					}
				}
				else break;
			}

			bytes[0] = ((uint8_t)bits[2] << 7) | ((uint8_t)bits[3] << 6) | ((uint8_t)bits[4] << 5) | ((uint8_t)bits[5] << 4) | ((uint8_t)bits[6] << 3) | ((uint8_t)bits[7] << 2) | ((uint8_t)bits[8] << 1) | ((uint8_t)bits[9] << 0);
			bytes[1] = ((uint8_t)bits[10] << 7) | ((uint8_t)bits[11] << 6) | ((uint8_t)bits[12] << 5) | ((uint8_t)bits[13] << 4) | ((uint8_t)bits[14] << 3) | ((uint8_t)bits[15] << 2) | ((uint8_t)bits[16] << 1) | ((uint8_t)bits[17] << 0);
			bytes[2] = ((uint8_t)bits[18] << 7) | ((uint8_t)bits[19] << 6) | ((uint8_t)bits[20] << 5) | ((uint8_t)bits[21] << 4) | ((uint8_t)bits[22] << 3) | ((uint8_t)bits[23] << 2) | ((uint8_t)bits[24] << 1) | ((uint8_t)bits[25] << 0);
			bytes[3] = ((uint8_t)bits[26] << 7) | ((uint8_t)bits[27] << 6) | ((uint8_t)bits[28] << 5) | ((uint8_t)bits[29] << 4) | ((uint8_t)bits[30] << 3) | ((uint8_t)bits[31] << 2) | ((uint8_t)bits[32] << 1) | ((uint8_t)bits[33] << 0);
			bytes[4] = ((uint8_t)bits[34] << 7) | ((uint8_t)bits[35] << 6) | ((uint8_t)bits[36] << 5) | ((uint8_t)bits[37] << 4) | ((uint8_t)bits[38] << 3) | ((uint8_t)bits[39] << 2) | ((uint8_t)bits[40] << 1) | ((uint8_t)bits[41] << 0);
			temp = ((uint16_t)(0x7F & bytes[2]) << 8) | bytes[3];
			if(bytes[2] & 0x80) temp *= -1;
			hum = ((uint16_t)bytes[0] << 8) | bytes[1];
			if(((bytes[0] + bytes[1] + bytes[2] + bytes[3]) & 0xFF) == bytes[4])
			{
				RetVal = ERR_NONE;
			}
			how1 = 0;
		}
		*temperature = (float)temp/10;
		*humidity = (float)hum/10;
	}
	else
	{
		RetVal = ERR_NULL_POINTER;
	}

	return RetVal;
}

status dht22_read2(float *temperature, float *humidity)
{
	status RetVal = ERR_READ_DHT;
	long int elapsed_time;
	struct timeval start, end;
	value last_state = HIGH;
	int counter = 0;

	if((NULL != temperature) && (NULL != humidity))
	{
		set_pin_out(P8_26);
		// Initialize DHT22 sensor
		set_pin_high(P8_26);
		usleep(500000);  // 500 ms
		set_pin_low(P8_26);
		usleep(20000);

		set_pin_in(P8_26);

		while (HIGH == read_pin(P8_26))
		{
			//usleep(1);
		}

		while(counter < 300)
		{
			gettimeofday( &start, NULL );
			while (read_pin(P8_26) == last_state) ++counter;
			gettimeofday( &end, NULL );
			last_state = read_pin(P8_26);
			elapsed_time = end.tv_usec - start.tv_usec;
		}
		printf( "usec: %ld, %d\n", elapsed_time, counter);
		usleep(1000);
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
static uint16_t get_threshold(uint16_t *data, uint16_t size)
{
	uint16_t threshold = 0, max = 0, min = 0xFFFF;
	uint16_t i, j = 0;
	uint16_t cnt[40] = {0, };

	// Szukanie minimalnej i maksymalnej wartosci wystapienia jedynki
	for(i = 4; i < size; i = i + 2)
	{
		cnt[j] = data[i];
		if(cnt[j] > max) max = cnt[j];
		if(cnt[j] < min) min = cnt[j];
		j++;
	}
	threshold = max + min;
	threshold /= 2;
	return threshold;
}
