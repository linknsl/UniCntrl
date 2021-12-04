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
static int getMeasurement(float *value_array);

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

static int getMeasurement(float *value_array) {
	measurement_apds9300_t measurement;
	sleep(polling_time);

/*	measurement.temperature = calc_temperature_sht21(get_setting_float(HWMON_SHT21, SHT21_TEMP));
	measurement.humidity = calc_humidity_sht21(get_setting_float(HWMON_SHT21, SHT21_HUMIDITY));
	value_array[0] = measurement.temperature;
	value_array[1] = measurement.humidity;*/
	return SUCCESS;
}

static int apds9300_init(init_conf_t *conf) {
	i2c_setting_t *is = conf->dev_sett;
	FILE *fp;
	char path[MAX_BUF];
	char start[MAX_BUF];
	memset(start, 0, MAX_BUF);
	memset(path, 0, MAX_BUF);
	generate_root_i2c_string(root,HWMON_ROOT_APDS9300,is->device,is->addr,HWMON_PREFIX_APDS9300);
/*	snprintf(start, sizeof start, "%s/%s", HWMON_SHT21, SHT21_HUMIDITY);
	if (access(start, F_OK)) {
		snprintf(path, sizeof(path), HWMON_NEW_DEVICE);
		if ((fp = fopen(path, "w")) == NULL) {
			printf("file open failed\n");
			return FAILURE;
		}
		rewind(fp);
		fprintf(fp, "sht21 0x40\n");
		fflush(fp);
		fclose(fp);
	}
	if (!access(start, F_OK)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}*/
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
