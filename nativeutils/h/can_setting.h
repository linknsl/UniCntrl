/*
 * uart_setting.h
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <common.h>

#ifndef CAN_SETTING_H_
#define CAN_SETTING_H_

int getFncCan(devFunc_t *cfgFuncs);

#define DEFAULT_DEVICE_CAN "can0"

#endif /* CAN_SETTING_H_ */
