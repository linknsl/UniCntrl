/*
 ============================================================================
 Name        : sht21.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <poll.h>
#include <common.h>
#include <ds18b20.h>

static void setPollingTime(int pol_time);
static int getMeasurement(float *value_array);
static int ds18b20_init(int id);

static int polling_time;

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

float calc_temperature_parser(char *temp_raw) {
	float result = 0;
	return result;
}

static int getMeasurement(float *value_array) {
	measurement_ds18b20 measurement;
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sleep(polling_time);
	if (get_setting_str(buf, HWMON_DS18B20, DS18B20_TEMP) != 0) {
		return FAILURE;
	}
	measurement.temperature = calc_temperature_parser(buf);
	value_array[0] = measurement.temperature;
	return SUCCESS;
}

static int ds18b20_init(int id) {
	if (!access(DS18B20_TEMP, F_OK)) {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}

int getSensorFncDs18b20(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurementFloat = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->init = ds18b20_init;
	return SUCCESS;
}
