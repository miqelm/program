#ifndef DEFINES_H
#define DEFINES_H

#define P8_26						(1 << 29)
#define P8_16						(1 << 14)

#define DELAY_BETWEEN_MEASURMENTS 	(600)

#define MYSQL_ADDRESS				("mysql.agh.edu.pl")
#define MYSQL_USER					("makam")
#define MYSQL_PASSWORD				("28CkcBUj")
#define MYSQL_DATABASE				("makam")
#define MYSQL_TABLE_NAME			("weather")

#define DATE_LENGTH					(11)
#define TIME_LENGTH					(9)
#define QUERY_LENGTH				(80)

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
	ERR_READ_DHT,
	ERR_NULL_POINTER,
	ERR_READ_BMP,
	ERR_MYSQL_INIT,
	ERR_MYSQL_CONN,
	ERR_MYSQL_QUERY,
	ERR_MYSQL_STORE,
	ERR_ADD_DATABASE,
} status;

void check_retval(status RetVal);

#endif
