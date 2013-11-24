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
status add_measurments(float temperature, float humidity, float pressure)
{
	status RetVal = ERR_NONE;
	char query[QUERY_LENGTH];
	time_t rawtime;
	struct tm * timeinfo;
	char date_str[DATE_LENGTH];
	char time_str[TIME_LENGTH];
	MYSQL *connection = mysql_init(NULL);

	if (NULL != connection)
	{
		if(NULL != mysql_real_connect(connection, MYSQL_ADDRESS, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE, 0, NULL, 0))
		{
			time (&rawtime);
			timeinfo = localtime (&rawtime);
			strftime (date_str, DATE_LENGTH, "%Y-%m-%d", timeinfo);
			strftime (time_str, TIME_LENGTH, "%H:%M:%S", timeinfo);
			snprintf(query, QUERY_LENGTH, "INSERT INTO %s VALUES('%s', '%s', %.1f, %.2f, %.1f)",
										MYSQL_TABLE_NAME,
										date_str,
										time_str,
										temperature,
										pressure,
										humidity);
			if(0 != mysql_query(connection, query))
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
