/*
 * MHZ19C.c
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <common.h>
#include <usb_charging.h>
#include <linux/can.h>

static int polling_time;
static devFunc_t *df_can;
static pthread_mutex_t mutex_measurement;

#define SIZE_SUBSCRIBE_USBCHARGING 1
static char *subscribe[] = { "getIdUsbcharging" };

static int readCan(uint16_t cmd, uint8_t *response);
static int getMeasurementId(int *value_array, init_conf_t *conf);

static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	eUsbCharging num = 0;
	int value_array_int[3];
	init_conf_t conf;
	conf.id = 0;
	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
		num = (eUsbCharging) atoi(message->payload);
		getMeasurementId(value_array_int, &conf);
		switch (num) {
		case SOFT_ID:
			mqtt_gen_topic_and_pub_int(obj, "id", value_array_int[0]);
			break;
		case HARD_ID:
			mqtt_gen_topic_and_pub_int(obj, "id", value_array_int[1]);
			break;
		case UNIQ_ID:
			mqtt_gen_topic_and_pub_hex(obj, "id", value_array_int[2]);
			break;
		default:
			break;
		}
	}
}

static void mqtt_subscribe_init(char *topic) {
	int i;
	for (i = 0; i < SIZE_SUBSCRIBE_USBCHARGING; i++) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

static int getSoftId(uint8_t *data) {
	return ((data[1] << 8) & 0xff) | data[0];
}

static int getHardId(uint8_t *data) {
	return ((data[3] << 8) & 0xff) | data[2];
}

static int getUniqId(uint8_t *data) {
	return (data[7] << 24) | (data[6] << 16) | (data[5] << 8) | data[4];
}

static int getMeasurementId(int *value_array, init_conf_t *conf) {
	id_usb_chrg_t iuc;
	uint8_t responseId[RESPONSE_CNT_UCH];

	memset(&iuc, 0, sizeof(id_usb_chrg_t));

	readCan(BYTE_ID + conf->id, responseId);

	iuc.soft_id = getSoftId(responseId);
	iuc.hard_id = getHardId(responseId);
	iuc.uniq_id = getUniqId(responseId);

	value_array[0] = iuc.soft_id;
	value_array[1] = iuc.hard_id;
	value_array[2] = iuc.uniq_id;

	return SUCCESS;
}

static int getVoltage(float *out, uint8_t *data) {
	int i = 0;
	for (i = 0; i < 3; i++) {
		out[i] = data[i] * 0.025;
		if (out[i] > 6.375 || out[i] < 0)
			return FAILURE;
	}
	return 0;
}

static int getCurrent(float *out, uint8_t *data) {
	int i = 0;
	for (i = 0; i < 3; i++) {
		out[i] = data[i] * 0.01;
		if (out[i] > 2.55 || out[i] < 0)
			return FAILURE;
	}
	return 0;
}

static int getTemperature(float *out, uint8_t *data) {
	out[0] = data[0] * 1;
	if (out[0] > 127 || out[0] < -127)
		return FAILURE;
	else
		return SUCCESS;
}

static int getMistake(int *out, uint8_t *data) {
	int i = 0;
	for (i = 0; i < 3; i++) {
		out[i] = ((data[0] >> (i * 2)) & 3);
	}
	return SUCCESS;
}

static int getTotalMistake(int *out, uint8_t *data) {
	out[0] = ((data[0] >> (3 * 2)) & 3);
	return SUCCESS;
}

static int getMeasurementFloat(float *value_array, init_conf_t *conf) {
	measurement_usb_chrg_t muc;
	int rc;
	uint8_t responseMeasurement[RESPONSE_CNT_UCH];

	memset(&muc, 0, sizeof(measurement_usb_chrg_t));

	rc = readCan(BYTE_MEASURE + conf->id, responseMeasurement);
	if (rc != 0)
		return rc;
	rc = getVoltage(muc.usb_voltage, responseMeasurement);
	memcpy(value_array, muc.usb_voltage, 3 * sizeof(float));

	rc = getCurrent(muc.usb_current, &responseMeasurement[3]);
	memcpy(&value_array[3], muc.usb_current, 3 * sizeof(float));

	rc = getTemperature(&muc.temperature, &responseMeasurement[7]);
	memcpy(&value_array[6], &muc.temperature, sizeof(float));

	return rc;
}

static int getMeasurement(int *value_array, init_conf_t *conf) {
	int rc;
	measurement_usb_chrg_t muc;
	uint8_t responseMeasurement[RESPONSE_CNT_UCH];
	memset(&muc, 0, sizeof(measurement_usb_chrg_t));

	rc = readCan(BYTE_MEASURE + conf->id, responseMeasurement);
	if (rc != 0)
		return rc;

	rc = getMistake(muc.usb_mistake, &responseMeasurement[6]);
	rc = getTotalMistake(&muc.total_mistake, &responseMeasurement[6]);

	value_array[0] = muc.usb_mistake[0];
	value_array[1] = muc.usb_mistake[1];
	value_array[2] = muc.usb_mistake[2];
	value_array[3] = muc.total_mistake;
	return rc;
}

static int readCan(uint16_t cmd, uint8_t *response) {
	int rc = 0,cnt_timeout = 0;
	struct can_frame frame;
	fd_set set;
	struct timeval tv;
	int fd, nbytes = 0;

	fd = df_can->getDescriptor();

	/*	sleep(polling_time);*/
	if (response != NULL) {
		do {
			FD_ZERO(&set);
			FD_SET(fd, &set);
			tv.tv_sec = 2;
			tv.tv_usec = 0;
			if (select(fd + 1, &set, NULL, NULL, &tv)) {/*ready read data*/
				if (FD_ISSET(fd, &set)) {

					pthread_mutex_lock(&mutex_measurement);
					nbytes = read(fd, &frame, sizeof(struct can_frame));
					pthread_mutex_unlock(&mutex_measurement);

					if (nbytes > 0 && (cmd == frame.can_id)) {
						memcpy(response, frame.data, 8);
						rc = SUCCESS;
						cnt_timeout = 0;
						break;
					}
				}
			} else {
				printf("Exit timeout \n");
				cnt_timeout++;
				rc = FAILURE;
				break;
			}

		} while (1);
	}
	return rc;
}

static int UsbCharging_init(init_conf_t *conf) {
	df_can = ((devSensorFunc_t*) conf->dev_func)->devFunc;
	if (!df_can->autoConfDev(conf->id)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
}

int getSensorFncUsbCharging(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurement = getMeasurement;
	cfgFuncs->getMeasurementFloat = getMeasurementFloat;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = UsbCharging_init;
	cfgFuncs->mtx = &mutex_measurement;
	return SUCCESS;
}
