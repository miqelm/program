#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "regs.h"
#include "defines.h"

typedef uint32_t u32_t;
typedef uint8_t u8_t;

status init(void);
void clean(void);
void set_pin_out(u32_t pin);
void set_pin_in(u32_t pin);
void set_pin_high(u32_t pin);
void set_pin_low(u32_t pin);

volatile u32_t* gpio1 = NULL;
int file = 0;

int main(void)
{
	status RetVal = ERR_NONE;

	RetVal = init();
	if(ERR_NONE == RetVal)
	{
		set_pin_out(P8_26);
		while(1)
		{
			printf("HIGH\n");
			set_pin_high(P8_26);
			sleep(1);
			printf("LOW\n");
			set_pin_low(P8_26);
			sleep(1);
		}
		clean();
	}

	return RetVal;
}

status init(void)
{
	status RetVal = ERR_NONE;
	file = open("/dev/mem", O_RDWR);
	if(-1 != file)
	{
		gpio1 = mmap(NULL, GPIO_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, file, GPIO1_START);
		if(MAP_FAILED == gpio1)
		{
			printf("Mapping memory for GPIO1 failed!\n");
			RetVal = ERR_MAP_FAIL;
		}
	}
	else
	{
		printf("Opening file /dev/mem failed!\n");
		RetVal = ERR_OPEN_FILE;
	}
	return RetVal;
}

void clean(void)
{
	close(file);
}

void set_pin_out(u32_t pin)
{
	*(volatile u32_t *)((void*)gpio1 + GPIO_OE) &= ~pin;
}

void set_pin_in(u32_t pin)
{
	*(volatile u32_t *)((void*)gpio1 + GPIO_OE) |= pin;
}

void set_pin_high(u32_t pin)
{
	*(volatile u32_t *)((void*)gpio1 + GPIO_SETDATAOUT) = pin;
}

void set_pin_low(u32_t pin)
{
	*(volatile u32_t *)((void*)gpio1 + GPIO_CLEARDATAOUT) = pin;
}
