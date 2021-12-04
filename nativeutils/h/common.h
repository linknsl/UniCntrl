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

typedef struct init_conf {
	int id;
	void *dev_sett;
} init_conf_t;

typedef struct deinit_conf {
	int id;
} deinit_conf_t;

typedef int (*FUNCP_GET_MESUREMENT_INT)(int *value_array, mqtt_config_read_t *conf);
typedef int (*FUNCP_GET_MESUREMENT_FLOAT)(float *value_array, mqtt_config_read_t *conf);
typedef void (*FUNCP_SET_POLLING_TIME)(int pol_time);
typedef void (*FUNCP_MQTT_CLB)(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);
typedef void (*FUNCP_MQTT_INIT_SUB)(char *topic);
typedef int (*FUNCP_SET_INIT)(init_conf_t *conf);
typedef int (*FUNCP_SET_DEINIT)(deinit_conf_t *conf);

typedef struct devSensorFunc {
	FUNCP_GET_MESUREMENT_INT getMeasurement;
	FUNCP_GET_MESUREMENT_FLOAT getMeasurementFloat;
	FUNCP_SET_POLLING_TIME setPollingTime;
	FUNCP_MQTT_INIT_SUB mqtt_init_sub;
	FUNCP_MQTT_CLB mqtt_clb;
	FUNCP_SET_INIT init;
	FUNCP_SET_INIT deinit;
} devSensorFunc_t;

float get_setting_float(char *ifname, char *param);
int get_setting_int(char *ifname, char *param);
int get_setting_str(char *out, char *ifname, char *param);
int init(int *id, usr_cfg_t *uc, devSensorFunc_t *dSf, eRead_configure block);
int generate_root_i2c_string(char *out, char *root, char *device, char *addr, char *prefix);
int generate_root_ow1_string(char *out, char *root, char *addr);
int addr_tochar(char *out, int addr);
#endif /* H_COMMON_H_ */
