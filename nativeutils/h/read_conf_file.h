/*
 * readConfugurationFile.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_READ_CONF_FILE_H_
#define H_READ_CONF_FILE_H_

#include <common.h>

#define PATH_CONF_FILE "/etc"
#define NAME_CONF_FILE "unicntrl.cfg"

#define CONF_MQTT      "application.mqtt.general"
#define CONF_MQTT_READ "application.mqtt.read"
#define VERSION_MQTT_GNRL 0

#define CONF_UART      "application.uarts.setting"
#define CONF_I2C       "application.i2cs.setting"
#define CONF_ONEW1S    "application.oneW1.setting"
#define CONF_CANS      "application.cans.setting"

#define SIZE_STRING 64
#define SIZE_LONG_STRING 128

typedef struct uart_setting {
	char *device;
	char *name;
	int mqtt_id;
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

typedef struct mqtt_config_read {
	int id;
	int polling_time;
	char *sensor_name;
	params_t *params;
/*	struct mqtt_config_children *children;*/
} mqtt_config_read_t;

typedef struct mqtt_config {
	char *server;
	int number_board;
	int port;
	int keepalive;
	void *fun_mess_clb;
	char *topic;
/*	mqtt_config_children_t *children;*/
} mqtt_config_t;

typedef struct usr_cfg {
	mqtt_config_t *mqtt_general;
	mqtt_config_read_t *mqtt_read;
	uart_setting_t *dev_cfg;
} usr_cfg_t;

int readConfMqtt(mqtt_setting_t *ms, int numInstance);
int readConfUart(uart_setting_t *us, int numInstance);
int readConfMqtt_new(mqtt_config_t *ms);
int read_usr_uart_conf(usr_cfg_t *ms, int numInstance);
int get_usr_param_cnf(mqtt_config_read_t *ms, char *param[]);
int newconfig(void);
#endif /* H_READ_CONF_FILE_H_ */
