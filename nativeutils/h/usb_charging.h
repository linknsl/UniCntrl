/*
 * MHZ19C.h
 *
 *  Created on: 14 нояб. 2021 г.4e76trf
 *      Author: root
 */
#include <common.h>

#ifndef MHZ19C_H_
#define MHZ19C_H_

#define REQUEST_CNT 8
#define RESPONSE_CNT 9

typedef struct measurement_usb_chrg {
	int soft_id;
	int hard_id;
	int uniq_id;
	int usb_voltage[3];
	int usb_current[3];
	int usb_mistake[3];
	int total_mistake;
	int temperature;
} measurement_usb_chrg_t;

int getSensorFncUsbCharging(devSensorFunc_t *cfgFuncs);

#endif /* MHZ19C_H_ */
