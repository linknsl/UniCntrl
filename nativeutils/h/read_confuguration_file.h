/*
 * readConfugurationFile.h
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_READ_CONFUGURATION_FILE_H_
#define H_READ_CONFUGURATION_FILE_H_

#define SIZE_STRING 128
#define SIZE_LONG_STRING 256
#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        		0

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

int readConfMqtt(mqtt_setting_t *ms, int numInstance);
int readConfUart(uart_setting_t *us, int numInstance);
#endif /* H_READ_CONFUGURATION_FILE_H_ */
