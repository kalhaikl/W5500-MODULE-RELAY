#ifndef ETHERNET_W550_MQTT_EXAMPLE_H_
#define ETHERNET_W550_MQTT_EXAMPLE_H_

#include <stdint.h>

int mqtt_network_init(void);
int mqtt_connect_broker(void);

void mqtt_publish(char *topic, const char *payload);
int mqtt_subscribe(char *topic);

void mqtt_publish_int(char *topic, int value);
void mqtt_publish_float(char *topic, float value);

void mqtt_publish_ip(char *topic);

void mqtt_yield(void);

void MQTT_Example(void);

#endif /* ETHERNET_W550_MQTT_EXAMPLE_H_ */
