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

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <poll.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <syslog.h>

#define MAX_BUF 128
#define PATH_LOG "/var/tele/"
#define FILE_LOG "log"
#define FILE_LOG_ERROR "log_error"

#ifdef NOLOG
  #define LOG_DATE(format, ...)
 #else
#define LOG_DATE(format, ...) \
	do { \
		char file[128],name[32];\
		sprintf((char*) &file, "%s " format "\n", sprintts(0), ##__VA_ARGS__); \
		save_log_str( FILE_LOG , file ); \
	} while (0);
#endif

#ifdef NOLOG
#define LOG(format, ...)
#else
#define LOG(format, ...) \
	do { \
		char file[128]; \
		struct timeval __xxts; \
		gettimeofday(&__xxts, 0); \
		sprintf((char*) &file, "%03d.%06d " format "\n", \
			(int)__xxts.tv_sec % 1000, (int)__xxts.tv_usec, ##__VA_ARGS__); \
			save_log_str( FILE_LOG , file ); \
	} while (0);
#endif

#ifdef NOLOG
#define LOG_ERROR(format, ...)
#else
#define LOG_ERROR(format, ...) \
	do { \
		char file[128]; \
		struct timeval __xxts; \
		gettimeofday(&__xxts, 0); \
		sprintf((char*) &file, "%03d.%06d " format "\n", \
			(int)__xxts.tv_sec % 1000, (int)__xxts.tv_usec, ##__VA_ARGS__); \
			save_log_str( FILE_LOG_ERROR , file ); \
	} while (0);
#endif

#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define ERROR_GET_DEVICE    -13
#define FAILURE             -1
#define SUCCESS        		 0

typedef struct init_conf {
	int id;
	void *dev_sett;
	void *dev_func;
} init_conf_t;

typedef struct deinit_conf {
	int id;
} deinit_conf_t;

typedef int (*FUNCP_GET_DESCRIPTOR)(void);
typedef int (*FUNCP_GET_OPEN_DEVICE)(char *device);
typedef void (*FUNCP_GET_CLOSE_DEVICE)(void);
typedef int (*FUNCP_SET_CONF_DEVICE)(init_conf_t *conf);
typedef int (*FUNCP_SET_AUTO_CONF_DEVICE)(int numInstance);
typedef void (*FUNCP_RECCONECT)(bool status);

typedef struct devFunc {
	int fd_dev;
	FUNCP_GET_DESCRIPTOR getDescriptor;
	FUNCP_GET_OPEN_DEVICE openDev;
	FUNCP_GET_CLOSE_DEVICE closeDev;
	FUNCP_SET_CONF_DEVICE confDev;
	FUNCP_SET_AUTO_CONF_DEVICE autoConfDev;
	FUNCP_RECCONECT recconect;
} devFunc_t;

typedef int (*FUNCP_GET_MESUREMENT_INT)(int *value_array, init_conf_t *conf);
typedef int (*FUNCP_GET_MESUREMENT_FLOAT)(float *value_array, init_conf_t *conf);
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
	devFunc_t *devFunc;
	pthread_mutex_t *mtx;
} devSensorFunc_t;

int osStartEngine(int argc, const char *argv[], void *entryPoint);
void save_log_str(char *name, char *val);
char* sprintts(struct timeval *tv);

float get_setting_float(char *ifname, char *param);
int get_setting_int(char *ifname, char *param);
int get_setting_str(char *out, char *ifname, char *param);
void init_log(const char *name, int id);
int init(int *id, usr_cfg_t *uc, devSensorFunc_t *dSf, init_conf_t *ic, eRead_configure block);
int generate_root_i2c_string(char *out, char *root, char *device, char *addr, char *prefix);
int generate_root_ow1_string(char *out, char *root, char *addr);
int addr_tochar(char *out, int addr);
#endif /* H_COMMON_H_ */
