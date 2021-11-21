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

typedef struct uart_setting {
  char *device;
  int speed;
  int databits;
  int stopbits;
} uart_setting_t;

typedef struct mqtt_setting {
  char *topic;
  char *name;
  int polling_time;
  int number;
} mqtt_setting_t;

int readConfMqtt( mqtt_setting_t *ms , int numInstance);
int readConfUart( uart_setting_t *us , int numInstance);
#endif /* H_READ_CONFUGURATION_FILE_H_ */
