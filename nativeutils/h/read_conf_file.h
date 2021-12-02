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

typedef enum READ_CONFIGURE {
	UARTS, I2CS, ONEW1S, CANS
} eRead_configure;

typedef struct uart_setting {
	char *device;
	char *name;
	int mqtt_id;
	int speed;
	int databits;
	int stopbits;
} uart_setting_t;

typedef struct i2c_setting {
	char *device;
	char *name;
	int mqtt_id;
	int addr;
} i2c_setting_t;

typedef struct onew1_setting {
	char *device;
	char *name;
	int mqtt_id;
	int addr;
} onew1_setting_t;

typedef struct can_setting {
	char *device;
	char *name;
	int mqtt_id;
	int addr;
} can_setting_t;

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
	void *dev_cfg;
} usr_cfg_t;
int read_uart_conf(usr_cfg_t *uc, eRead_configure block, int *id, char *name, int numInstance);
int readGnrlMqtt(mqtt_config_t *ms, int version);
int readConfMqtt(mqtt_config_read_t *msr, int mqtt_id);
int readConfUart(uart_setting_t *us, int numInstance);
int readConfI2C(i2c_setting_t *is, int numInstance);
int readConfOneW1(onew1_setting_t *os, int numInstance);
int readConfCAN(can_setting_t *cs, int numInstance);
int get_usr_param_cnf(mqtt_config_read_t *ms, char *param[]);
#endif /* H_READ_CONF_FILE_H_ */
