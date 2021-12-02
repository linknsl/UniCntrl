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


#define MAX_BUF 128

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define ERROR_GET_DEVICE    -13
#define FAILURE             -1
#define SUCCESS        		 0


typedef int (*FUNCP_GET_MESUREMENT_INT)(  int *value_array);
typedef int (*FUNCP_GET_MESUREMENT_FLOAT)(  float *value_array);
typedef void (*FUNCP_SET_POLLING_TIME)(int pol_time);
typedef int (*FUNCP_SET_INIT)(int id);
typedef int (*FUNCP_SET_DEINIT)(void);

typedef struct devSensorFunc {
	FUNCP_GET_MESUREMENT_INT getMeasurement;
	FUNCP_GET_MESUREMENT_FLOAT getMeasurementFloat;
	FUNCP_SET_POLLING_TIME setPollingTime;
	FUNCP_SET_INIT init;
	FUNCP_SET_INIT deinit;
} devSensorFunc_t;

float get_setting_float(char *ifname, char *param);
int get_setting_str(char *out, char *ifname, char *param);

#endif /* H_COMMON_H_ */
