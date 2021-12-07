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
#include <sht21.h>

static void setPollingTime(int pol_time);
static float calc_temperature_sht21(float temp_raw);
static float calc_humidity_sht21(float humidity_raw);
static int sht21_init(init_conf_t *conf);

static int polling_time;
static char root[SIZE_LONG_STRING];

#define SIZE_SUBSCRIBE_SHT21 1
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
	for (i = 0; i < SIZE_SUBSCRIBE_SHT21; i++) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

static float calc_temperature_sht21(float temp_raw) {
	float result = ((175.72 * (temp_raw / 65536)) - 46.85);
	return result;
}

static float calc_humidity_sht21(float humidity_raw) {
	float result = (125 * (humidity_raw / 65536) - 6);
	return result;
}

static int getMeasurement(float *value_array, init_conf_t *conf) {
	measurement_sht21_t measurement;
	sleep(polling_time);
	measurement.temperature = calc_temperature_sht21(get_setting_float(root, SHT21_TEMP));
	measurement.humidity = calc_humidity_sht21(get_setting_float(root, SHT21_HUMIDITY));
	value_array[0] = measurement.temperature;
	value_array[1] = measurement.humidity;
	return SUCCESS;
}

static int sht21_init(init_conf_t *conf) {
	FILE *fp;
	char path[MAX_BUF];
	memset(path, 0, MAX_BUF);

	i2c_setting_t *is = conf->dev_sett;
	char out[MAX_BUF];
	memset(out, 0, MAX_BUF);
	addr_tochar(out, is->addr);
	generate_root_i2c_string(root, HWMON_SHT21, is->name, out, HWMON_PREFIX_SHT21);

	if (access(root, F_OK)) {
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
	if (!access(root, F_OK)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int getSensorFncSht21(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurementFloat = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = sht21_init;
	return SUCCESS;
}
