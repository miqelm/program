#ifndef DHT22
#define DHT22

/******************************************************************************
 * FUNCTION PROTOTYPES
 *****************************************************************************/
status dht22_read(float *temperature, float *humidity);
status dht22_read2(float *temperature, float *humidity);

#endif
