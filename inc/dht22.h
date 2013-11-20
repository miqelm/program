#ifndef DHT22
#define DHT22

status dht22_read(float *temperature, float *humidity);
status dht22_read2(float *temperature, float *humidity);
uint16_t get_threshold(uint16_t data[], uint16_t size);

#endif
