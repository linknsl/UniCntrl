/*
 * readConfugurationFile.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_READ_CONF_FILE_H_
#define H_READ_CONF_FILE_H_

#include <common.h>

#define SIZE_STRING 128
#define SIZE_LONG_STRING 256

typedef struct uart_setting {
	char *device;
	int speed;
	int databits;
	int stopbits;
} uart_setting_t;

typedef struct mqtt_setting {
	char *topic;
	char *name;
	char *host;
	int port;
	int keepalive;
	int polling_time;
	int number;
	int fd;
	int log;
	void *fun_mess_clb;
} mqtt_setting_t;

typedef struct params {
	char *param;
	struct params *next;
} params_t;

typedef struct mqtt_config_children {
	int id;
	int polling_time;
	char *sensor_name;
	params_t *params;
	struct mqtt_config_children *children;
} mqtt_config_children_t;

typedef struct mqtt_config {
	char *server;
	int number_board;
	int port;
	int keepalive;
	mqtt_config_children_t *children;
} mqtt_config_t;

int readConfMqtt(mqtt_setting_t *ms, int numInstance);
int readConfUart(uart_setting_t *us, int numInstance);
int readConfMqtt_new(mqtt_config_t *ms);
int newconfig(void);
#endif /* H_READ_CONF_FILE_H_ */
