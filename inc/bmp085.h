#ifndef BMP085
#define BMP085

#define BMP085_ADDRESS			(0x77)
#define TEMP_REQUEST_ADDR		(0xF4)
#define TEMP_REQUEST_COMMAND	(0x2E)

#define CALIBRATION_WORDS		(11 * 2)
#define AC1_ADDRESS				(0xAA)
#define AC2_ADDRESS				(0xAC)
#define AC3_ADDRESS				(0xAE)
#define AC4_ADDRESS				(0xB0)
#define AC5_ADDRESS				(0xB2)
#define AC6_ADDRESS				(0xB4)
#define B1_ADDRESS				(0xB6)
#define B2_ADDRESS				(0xB8)
#define MB_ADDRESS				(0xBA)
#define MC_ADDRESS				(0xBC)
#define MD_ADDRESS				(0xBE)

#define TEMP_ADDRESS			(0xF6)

status bmp085_read(float *temperature, float *pressure);
status bmp085_read2(float *temperature, float *pressure);
static uint16_t read_i2c(int file, uint8_t reg);
static uint16_t read_raw_temp(int file);

#endif
