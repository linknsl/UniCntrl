/*
 * uart_setting.h
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */

#ifndef UART_SETTING_H_
#define UART_SETTING_H_

void set_speed(int fd, int speed);
bool set_Parity(int fd,int databits,int stopbits,int parity);
int openDev(char *Dev);
int defaultConfUart(char *device);
int confUart(char *device, int speed, int databits, int stopbits);

#endif /* UART_SETTING_H_ */
