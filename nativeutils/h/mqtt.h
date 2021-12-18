/*
 * mqtt.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_MQTT_H_
#define H_MQTT_H_

#include <read_conf_file.h>

void mqtt_setup(mqtt_config_t *ms);
int mqtt_send(char *msg, char *topic);
int mqtt_sub(char *topic);
int mqtt_gen_topic_and_sub(char *topic, char *sub_topic);
int mqttResultPubInt(usr_cfg_t *ucfg, int *value_array);
int mqttResultPubFloat(usr_cfg_t *ucfg, float *value_array);
int mqtt_gen_topic_and_pub_float(char *topic, char *sub_topic, float value);
int mqtt_gen_topic_and_pub_int(char *topic, char *sub_topic, int value);
int mqtt_gen_topic_and_pub_hex(char *topic, char *sub_topic, int value);
int mqtt_set_topic_sub(void *obj, char *param, char *topic);
int mqtt_gen_topic_topic(void *in, char *param, char *res);

#endif /* H_MQTT_H_ */
