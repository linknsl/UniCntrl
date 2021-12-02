/*
 * MHZ19C.c
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <mhz19c.h>
#include <uart_setting.h>
#include <common.h>

static int polling_time;
pthread_mutex_t mutex_measurement;

static void writeCommand(uint8_t cmd[], uint8_t *response);
static int getMeasurement(int *value_array);

uint8_t getppm[] = { 0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t zerocalib[] = { 0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t spancalib[] = { 0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t autocalib_on[] = { 0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00 };
uint8_t autocalib_off[] = { 0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00 };

static uint8_t mhz19_checksum(uint8_t com[]) {
	int i;
	uint8_t sum = 0x00;
	for (i = 1; i < REQUEST_CNT; i++) {
		sum += com[i];
	}
	sum = 0xff - sum + 0x01;
	return sum;
}

static void setPollingTime(int pol_time) {
	polling_time = pol_time;
}

/* undocumented function */
/*static int getStatus() {
 measurement_mhz19_t m;
 getMeasurement(&m);
 return m.state;
 }*/

void setAutoCalibration(bool autocalib) {
	pthread_mutex_lock(&mutex_measurement);
	writeCommand(autocalib ? autocalib_on : autocalib_off, NULL);
	pthread_mutex_unlock(&mutex_measurement);
}

void calibrateZero(void) {
	pthread_mutex_lock(&mutex_measurement);
	writeCommand(zerocalib, NULL);
	pthread_mutex_unlock(&mutex_measurement);
}

static int getMeasurement(int *value_array) {
	measurement_mhz19_t ms;
	uint8_t response[RESPONSE_CNT];
	memset(response, 0, RESPONSE_CNT);

	pthread_mutex_lock(&mutex_measurement);
	writeCommand(getppm, response);
	pthread_mutex_unlock(&mutex_measurement);

	if (response[0] == 0xff && response[1] == 0x86 && (mhz19_checksum(response) == response[RESPONSE_CNT - 1])) {
		ms.co2_ppm = response[2] * 256 + response[3];
		ms.temperature = response[4] - 40;
		ms.state = response[5];
	} else {
		ms.co2_ppm = ms.temperature = ms.state = -1;
	}
	value_array[0] = ms.temperature;
	value_array[1] = ms.co2_ppm;
	return SUCCESS;
}

void calibrateSpan(int ppm) {
	int i;
	if (ppm < 1000)
		return;

	uint8_t cmd[REQUEST_CNT];
	for (i = 0; i < REQUEST_CNT; i++) {
		cmd[i] = spancalib[i];
	}
	cmd[3] = (uint8_t) (ppm / 256);
	cmd[4] = (uint8_t) (ppm % 256);
	writeCommand(cmd, NULL);
}

void cntrlCalibrate(eMhz19_calibrate key) {

	switch (key) {
	case ZERO:
		calibrateZero();
		break;
	case AUTO_ON:
		setAutoCalibration(true);
		break;
	case AUTO_OFF:
		setAutoCalibration(false);
		break;
	default:
		break;
	}
}

static void writeCommand(uint8_t cmd[], uint8_t *response) {
	fd_set set;
	struct timeval tv;
	int fd, nread = 0;

	fd = getDescriptor();

	uint8_t buff[RESPONSE_CNT];
	memcpy(buff, cmd, 8);
	buff[8] = mhz19_checksum(cmd);

	write(fd, buff, RESPONSE_CNT);
	sleep(polling_time);
	if (response != NULL) {
		memset(response, 0, RESPONSE_CNT);
		do {
			FD_ZERO(&set);
			FD_SET(fd, &set);
			tv.tv_sec = 1;
			tv.tv_usec = 0;
			if (select(fd + 1, &set, NULL, NULL, &tv)) {/*ready read data*/
				if (FD_ISSET(fd, &set)) {
					nread += read(fd, response + nread, sizeof(RESPONSE_CNT));
				}
				Reconnect(true);
			} else { /*timeout*/
				Reconnect(false);
				printf("Exit timeout \n");
				break;
			}

		} while (nread < 9);
	}
}

int getSensorFncMhz19c(devSensorFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getMeasurement = getMeasurement;
	cfgFuncs->setPollingTime = setPollingTime;

	return SUCCESS;
}
