#ifndef DEFINES_H
#define DEFINES_H

#define P8_26		(1 << 29)

typedef enum
{
	INPUT = 0,
	OUTPUT = 1,
} direction;

typedef enum
{
	LOW = 0,
	HIGH = 1,
} value;

typedef enum
{
	ERR_NONE = 0,
	ERR_OPEN_FILE,
	ERR_MAP_FAIL,
} status;

#endif
