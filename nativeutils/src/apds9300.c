/*
 ============================================================================
 Name        : sht21.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <common.h>
#include <apds9300.h>

static void setPollingTime(int pol_time);

static int polling_time;
static char root[SIZE_LONG_STRING];

#define SIZE_SUBSCRIBE_APDS9300 1
static char *subscribe[] = { "reset" };

static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	int num = 0;

	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
		num = atoi(message->payload);
		printf("reset %d \n", num);
	}
}

static void mqtt_subscribe_init(char *topic) {
	int i;
	for (i = 0; i < SIZE_SUBSCRIBE_APDS9300; i++) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

static int getMeasurement(int *value_array , init_conf_t *conf) {
	measurement_apds9300_t ms;
	sleep(polling_time);

	ms.in_illuminance = get_setting_int(root, APDS9300_ILLUMINANCE);
	ms.in_intensity0 = get_setting_int(root, APDS9300_INTENSITY0);
	ms.in_intensity1 = get_setting_int(root, APDS9300_INTENSITY1);
	value_array[0] = ms.in_illuminance;
	value_array[1] = ms.in_intensity0;
	value_array[2] = ms.in_intensity1;
	return SUCCESS;
}

static int apds9300_init(init_conf_t *conf) {
	i2c_setting_t *is = conf->dev_sett;
	char out[MAX_BUF];
	memset(out, 0, MAX_BUF);
	addr_tochar(out, is->addr);
	generate_root_i2c_string(root, HWMON_ROOT_APDS9300, is->name, out, HWMON_PREFIX_APDS9300);
	if (!access(root, F_OK)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int getSensorFncAPDS9300(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurement = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = apds9300_init;
	return SUCCESS;
}
