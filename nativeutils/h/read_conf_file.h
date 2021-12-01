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

typedef struct params {
	char *param;
	struct params *next;
} params_t;

typedef struct mqtt_config_read {
	int id;
	int polling_time;
	char *sensor_name;
	params_t *params;
} mqtt_config_read_t;

typedef struct mqtt_config {
	char *server;
	int number_board;
	int port;
	int keepalive;
	void *fun_mess_clb;
	char *topic;
} mqtt_config_t;

typedef struct usr_cfg {
	mqtt_config_t *mqtt_general;
	mqtt_config_read_t *mqtt_read;
	uart_setting_t *dev_cfg;
} usr_cfg_t;

int readGnrlMqtt(mqtt_config_t *ms, int version);
int readConfMqtt(mqtt_config_read_t *msr, int mqtt_id);
int readConfUart(uart_setting_t *us, int numInstance);
int read_usr_uart_conf(usr_cfg_t *ms, int numInstance);
int get_usr_param_cnf(mqtt_config_read_t *ms, char *param[]);
#endif /* H_READ_CONF_FILE_H_ */
