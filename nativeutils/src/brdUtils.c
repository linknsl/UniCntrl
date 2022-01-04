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
#include <brdUtils.h>

static int polling_time;
static char root[SIZE_LONG_STRING];

#define SIZE_SUBSCRIBE_BRDUTILS 1
static char *subscribe[] = { "setdate" };

static settime(int datetime) {
	struct timeval now,curtime;
	int rc;

	now.tv_sec = datetime;
	now.tv_usec = 0;

	rc = settimeofday(&now, NULL);
	if (rc == 0) {
		LOG("settimeofday() successful.");
	} else {
		LOG("settimeofday() failed, ""errno = %d", errno);
		return -1;
	}
}

static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	int num = 0;

	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
		num = atoi(message->payload);
		settime(num);
		LOG("settime %t", num);
	}
}

static void mqtt_subscribe_init(char *topic) {
	int i;
	for (i = 0; i < SIZE_SUBSCRIBE_BRDUTILS; i++) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

static int getMeasurement(int *value_array, init_conf_t *conf) {
	return SUCCESS;
}

static int init_brd(init_conf_t *conf) {
	return SUCCESS;
}

int getBrdUtilsFnc(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurementFloat = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = init_brd;
	return SUCCESS;
}
