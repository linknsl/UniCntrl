/*
 * uart_setting.h
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */

#ifndef UART_SETTING_H_
#define UART_SETTING_H_

#include <mhz19c.h>

#define DEFAULT_DEVICE "can0"

bool set_Parity(int databits, int stopbits, int parity);
int openDev(char *Dev);
int confUart(char *device, int speed, int databits, int stopbits);
int autoConfUart(int numInstance);
int defaultConfUart(char *device);
int getDescriptor(void);
void closeUart(void);
void Reconnect(bool status);

#endif /* UART_SETTING_H_ */
