#include <stdint.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include "defines.h"
#include "gpio.h"

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
int file = -1;
volatile uint8_t* gpio1 = NULL;
volatile uint8_t* gpio1_oe = NULL;
volatile uint8_t* gpio1_clear = NULL;
volatile uint8_t* gpio1_set = NULL;
volatile uint8_t* gpio1_in = NULL;

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
status gpio_init(void)
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
		else
		{
			gpio1_oe = gpio1 + GPIO_OE;
			gpio1_set = gpio1 + GPIO_SETDATAOUT;
			gpio1_clear = gpio1 + GPIO_CLEARDATAOUT;
			gpio1_in = gpio1 + GPIO_DATAIN;
		}
	}
	else
	{
		printf("Opening file /dev/mem failed!\n");
		RetVal = ERR_OPEN_FILE;

	}
	return RetVal;
}

void gpio_clean(void)
{
	if(-1 != file)
	{
		close(file);
	}
	if(MAP_FAILED != gpio1)
	{
		munmap((void *)gpio1, GPIO_SIZE);
	}
}

void set_pin_out(uint32_t pin)
{
	*(volatile uint32_t *)(gpio1_oe) &= ~pin;
}

void set_pin_in(uint32_t pin)
{
	*(volatile uint32_t *)(gpio1_oe) |= pin;
}

void set_pin_high(uint32_t pin)
{
	*(volatile uint32_t *)(gpio1_set) = pin;
}

void set_pin_low(uint32_t pin)
{
	*(volatile uint32_t *)(gpio1_clear) = pin;
}

value read_pin(uint32_t pin)
{
	value RetVal = LOW;
	volatile uint32_t reg = *(volatile uint32_t *)(gpio1_in);
	if(reg & pin)
	{
		RetVal = HIGH;
	}
	return RetVal;
}
