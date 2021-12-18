/*
 * MHZ19C.h
 *
 *  Created on: 14 нояб. 2021 г.4e76trf
 *      Author: root
 */
#include <common.h>

#ifndef MHZ19C_H_
#define MHZ19C_H_

#define RESPONSE_CNT_UCH 8
#define BYTE_ID 0x531
#define BYTE_MEASURE 0x521

typedef enum USB_CHARGING{
	SOFT_ID, HARD_ID, UNIQ_ID
} eUsbCharging;

typedef struct id_usb_chrg {
	int soft_id;
	int hard_id;
	int uniq_id;
} id_usb_chrg_t;

typedef struct measurement_usb_chrg {
	float usb_voltage[3];
	float usb_current[3];
	int usb_mistake[3];
	int total_mistake;
	float temperature;
} measurement_usb_chrg_t;

int getSensorFncUsbCharging(devSensorFunc_t *cfgFuncs);

#endif /* MHZ19C_H_ */
