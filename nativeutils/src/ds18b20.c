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

static int polling_time;
static char root[SIZE_LONG_STRING];

/*Information datasheet*/
uint16_t test_temperature_data[] = { 0x07D0, 0x0550, 0x0191, 0x00A2, 0x0008, 0x0000, 0xFFF8, 0xFF5E, 0xFE6F, 0xFC90 };
float test_equels_data[] = { 125, 85, 25.0625, 10.125, 0.5, 0, -0.5, -10.125, -25.0625, -55 };

#define SIZE_SUBSCRIBE_DS18B20 1
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
	for (i = 0; i < SIZE_SUBSCRIBE_DS18B20; i++) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

static float calc_temperature( temp_raw) {
	float result = 0;
	if (temp_raw & 0x8000)
		result = (((0xFFFF - temp_raw) + 1) / 16.0) * (-1);
	else
		result = temp_raw / 16.0;
	return result;
}

static float calc_temperature_parser(char *temp_raw) {
	int msb, lsb;
	sscanf(temp_raw, "%x%x", &lsb, &msb);
	return calc_temperature((msb << 8) + lsb);
}

static int getMeasurement(float *value_array, init_conf_t *conf) {
	measurement_ds18b20 measurement;
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	sleep(polling_time);
	if (get_setting_str(buf, root, DS18B20_TEMP) != 0) {
		return FAILURE;
	}
	measurement.temperature = calc_temperature_parser(buf);
	value_array[0] = measurement.temperature;
	return SUCCESS;
}

static int ds18b20_init(init_conf_t *conf) {

	onew1_setting_t *os = conf->dev_sett;
	generate_root_ow1_string(root, HWMON_DS18B20, os->addr);

	if (!access(root, F_OK)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int getSensorFncDs18b20(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurementFloat = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = ds18b20_init;
	return SUCCESS;
}
