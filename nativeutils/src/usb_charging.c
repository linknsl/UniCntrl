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

#define SIZE_SUBSCRIBE_USBCHARGING 2
static char *subscribe[] = { "setCalibrateSpan", "setCalibrate" };

static void readCan(uint8_t cmd[], uint8_t *response);

static void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	int num = 0;

	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
	}
	if (mqtt_set_topic_sub(obj, subscribe[1], message->topic)) {
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


static int getMeasurement(int *value_array , mqtt_config_read_t *conf) {
	measurement_usb_chrg_t muc;
	uint8_t response[128];
	memset(response, 0, 128);

	pthread_mutex_lock(&mutex_measurement);
	readCan(NULL, response);
	pthread_mutex_unlock(&mutex_measurement);

/*	if (response[0] == 0xff && response[1] == 0x86 && (mhz19_checksum(response) == response[RESPONSE_CNT - 1])) {
		ms.co2_ppm = response[2] * 256 + response[3];
		ms.temperature = response[4] - 40;
		ms.state = response[5];
	} else {
		ms.co2_ppm = ms.temperature = ms.state = -1;
	}*/
	value_array[0] = muc.soft_id;
	value_array[1] = muc.hard_id;
	value_array[2] = muc.uniq_id;
	value_array[3] = muc.usb_voltage[0];
	value_array[4] = muc.usb_voltage[1];
	value_array[5] = muc.usb_voltage[2];
	value_array[6] = muc.usb_current[0];
	value_array[7] = muc.usb_current[1];
	value_array[8] = muc.usb_current[2];
	value_array[9] = muc.usb_mistake[0];
	value_array[10] = muc.usb_mistake[1];
	value_array[11] = muc.usb_mistake[2];
	value_array[12] = muc.total_mistake;
	value_array[13] = muc.temperature;
	return SUCCESS;
}

static void readCan(uint8_t cmd[], uint8_t *response) {
	struct can_frame frame;
	fd_set set;
	struct timeval tv;
	int fd, nbytes = 0,i;

	fd = df_can->getDescriptor();

	sleep(polling_time);
	if (response != NULL) {
		memset(response, 0, 128);
		do {
			FD_ZERO(&set);
			FD_SET(fd, &set);
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			if (select(fd + 1, &set, NULL, NULL, &tv)) {/*ready read data*/
				if (FD_ISSET(fd, &set)) {
					nbytes = read(fd, &frame, sizeof(frame));
					if (nbytes > 0) {
						printf("%#x  [%d]  ", frame.can_id, frame.can_dlc);
						for (i = 0; i < frame.can_dlc; i++)
						printf("%#x ", frame.data[i]);
						printf("\n");
					}
				}

			} else { /*timeout*/
				printf("Exit timeout \n");
				break;
			}

		} while (nbytes < 9);
	}
}

static int UsbCharging_init(init_conf_t *conf) {
	df_can = conf->dev_func;
	if (!df_can->autoConfDev(conf->id)) {
		return SUCCESS;
	} else {
		return FAILURE;
	}
	return SUCCESS;
}

int getSensorFncUsbCharging(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurement = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;
	cfgFuncs->mqtt_init_sub = mqtt_subscribe_init;
	cfgFuncs->mqtt_clb = message_callback;
	cfgFuncs->init = UsbCharging_init;
	return SUCCESS;
}
