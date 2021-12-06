/*
 * uart_setting.h
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */

#ifndef UART_SETTING_H_
#define UART_SETTING_H_

#include <mhz19c.h>

#define DEFAULT_DEVICE "/dev/ttyUSB0"


int getFncUart(devFunc_t *cfgFuncs);
#endif /* UART_SETTING_H_ */
