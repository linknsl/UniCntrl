/*
 * common.h
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_COMMON_H_
#define H_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <mqtt.h>
#include <read_conf_file.h>

#include <mosquitto.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

typedef struct devSensorFunc {
	void *getMeasurement;
	void *setPollingTime;
} devSensorFunc_t;

#define MAX_BUF 128

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define FAILURE             -1
#define SUCCESS        		 0

float get_setting_float(char *ifname, char *param);

#endif /* H_COMMON_H_ */
