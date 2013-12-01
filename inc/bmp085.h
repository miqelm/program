#ifndef BMP085
#define BMP085

/******************************************************************************
 * BMP085 SENSOR DEFINITIONS
 *****************************************************************************/
#define BMP085_ADDRESS			(0x77)
#define TEMP_PRES_REQUEST_ADDR	(0xF4)
#define TEMP_REQUEST_COMMAND	(0x2E)
#define PRES_REQUEST_COMMAND	(0x34)

#define BMP085_ULTRA_LOW_POWER	(0)
#define BMP085_STANDARD			(1)
#define BMP085_HIGH_RES			(2)
#define BMP085_ULTRA_HIGH_RES	(3)
#define BMP085_MODE				BMP085_ULTRA_HIGH_RES

#define CALIBRATION_WORDS		(11)
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

#define TEMP_PRES_ADDRESS		(0xF6)

#define AC1						(compensation_table[0])
#define AC2						(compensation_table[1])
#define AC3						(compensation_table[2])
#define AC4						(compensation_table[3])
#define AC5						(compensation_table[4])
#define AC6						(compensation_table[5])
#define B1						(compensation_table[6])
#define B2						(compensation_table[7])
#define MB						(compensation_table[8])
#define MC						(compensation_table[9])
#define MD						(compensation_table[10])

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
status bmp085_read(float *temperature, float *pressure);
status bmp085_read2(float *temperature, float *pressure);

#endif
