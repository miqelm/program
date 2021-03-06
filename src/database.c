/******************************************************************************
 * INCLUDE SECTION
 *****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <my_global.h>
#include <mysql.h>
#include "defines.h"

/******************************************************************************
 * FUNCTION DEFINITIONS
 *****************************************************************************/
status add_measurments(float dht22_temperature, float dht22_humidity, float bmp85_temperature, float bmp85_pressure)
{
	status RetVal = ERR_NONE;
	char query[QUERY_LENGTH];
	time_t rawtime;
	struct tm * timeinfo;
	char date_str[DATE_LENGTH];
	char time_str[TIME_LENGTH];
	MYSQL *connection = mysql_init(NULL);
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;

	if (NULL != connection)
	{
		if(NULL != mysql_real_connect(connection, MYSQL_ADDRESS, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0))
		{
			time (&rawtime);
			timeinfo = localtime (&rawtime);
			strftime (date_str, DATE_LENGTH, "%Y-%m-%d", timeinfo);
			strftime (time_str, TIME_LENGTH, "%H:%M:%S", timeinfo);
			snprintf(query, QUERY_LENGTH, "INSERT INTO %s VALUES('%s', '%s', %.1f, %.2f, %.1f, %.1f)",
										MYSQL_TABLE_NAME,
										date_str,
										time_str,
										dht22_temperature,
										bmp85_pressure,
										dht22_humidity,
										bmp85_temperature);
			if(0 == mysql_query(connection, query))
			{
				if (0 == mysql_query(connection, "SELECT delay FROM weather_config"))
				{
					result = mysql_store_result(connection);
					if(NULL != result)
					{
						row = mysql_fetch_row(result);
						delay_between_measurements = (unsigned long)atoi(*row);
					}
					else
					{
						RetVal = ERR_MYSQL_GET_DATA;
					}
				}
				else
				{
					RetVal = ERR_MYSQL_QUERY;
				}
			}
			else
			{
				RetVal = ERR_MYSQL_QUERY;
			}
		}
		else
		{
			RetVal = ERR_MYSQL_CONN;
		}
		mysql_close(connection);
	}
	else
	{
		RetVal = ERR_MYSQL_INIT;
	}
	return RetVal;
}
