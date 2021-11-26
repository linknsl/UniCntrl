/*
 * mqtt.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_MQTT_H_
#define H_MQTT_H_

#include <read_confuguration_file.h>

void mqtt_subscribe_init(char *topic);
void mqtt_setup(mqtt_setting_t *ms);
int mqtt_send(char *msg, char *topic);
int mqtt_sub(char *topic);
int mqtt_gen_topic_and_sub(char *topic, char *sub_topic);
int mqtt_gen_topic_and_pub_int(char *topic, char *sub_topic, int value);

#endif /* H_MQTT_H_ */