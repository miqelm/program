#ifndef GPIO
#define GPIO

/******************************************************************************
 * MACRODEFINITIONS
 *****************************************************************************/
#define CONTROL_MODULE_START 	(0x44E10000)
#define CONTROL_MODULE_END 		(0x44E11FFF)
#define CONTROL_MODULE_SIZE		(CONTROL_MODULE_END - CONTROL_MODULE_START)

#define GPIO_SIZE				(0x1000)
#define GPIO_OE					(0x134)
#define GPIO_DATAIN				(0x138)
#define GPIO_DATAOUT			(0x13C)
#define GPIO_CLEARDATAOUT		(0x190)
#define GPIO_SETDATAOUT			(0x194)

#define GPIO1_START				(0x4804C000)
#define GPIO1_END				(0x4804CFFF)

/******************************************************************************
 * GLOBAL VARIABLES
 *****************************************************************************/
extern int file;
extern volatile uint8_t* gpio1;
extern volatile uint8_t* gpio1_oe;
extern volatile uint8_t* gpio1_clear;
extern volatile uint8_t* gpio1_set;
extern volatile uint8_t* gpio1_in;

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
status gpio_init(void);
void gpio_clean(void);
void set_pin_out(uint32_t pin);
void set_pin_in(uint32_t pin);
void set_pin_high(uint32_t pin);
void set_pin_low(uint32_t pin);
value read_pin(uint32_t pin);

#endif
