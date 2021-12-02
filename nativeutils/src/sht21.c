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
static int getMeasurement(float *value_array);
static float calc_temperature_sht21(float temp_raw);
static float calc_humidity_sht21(float humidity_raw);
static int sht21_init(int id);

static int polling_time;

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

static int getMeasurement(float *value_array) {
	measurement_sht21_t measurement;
	sleep(polling_time);
	measurement.temperature = calc_temperature_sht21(get_setting_float(HWMON_SHT21, SHT21_TEMP));
	measurement.humidity = calc_humidity_sht21(get_setting_float(HWMON_SHT21, SHT21_HUMIDITY));
	value_array[0] = measurement.temperature;
	value_array[1] = measurement.humidity;
	return SUCCESS;
}

static int sht21_init(int id) {
	FILE *fp;
	char path[MAX_BUF];
	char start[MAX_BUF];
	memset(start, 0, MAX_BUF);
	memset(path, 0, MAX_BUF);
	snprintf(start, sizeof start, "%s/%s", HWMON_SHT21, SHT21_HUMIDITY);
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
	}
}

int getSensorFncSht21(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurementFloat = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->init = sht21_init;
	return SUCCESS;
}
