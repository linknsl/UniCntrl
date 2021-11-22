/*
 * mqtt.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_MQTT_H_
#define H_MQTT_H_

#include <read_confuguration_file.h>

void mqtt_setup(mqtt_setting_t *ms);
int mqtt_send(char *msg , char *topic);
int mqtt_sub( char *topic );

#endif /* H_MQTT_H_ */
