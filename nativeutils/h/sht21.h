/*
 * sht21.h
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_SHT21_H_
#define H_SHT21_H_

#include <common.h>

#define HWMON_NEW_DEVICE "/sys/bus/i2c/devices/i2c-0/new_device"
#define HWMON_DELETE_DEVICE "/sys/bus/i2c/devices/i2c-0/delete_device"
#define HWMON_SHT21 "/sys/class/i2c-dev"
#define HWMON_PREFIX_SHT21 "hwmon/hwmon1"
#define SHT21_TEMP "temp1_input"
#define SHT21_HUMIDITY "humidity1_input"

typedef struct measurement_sht21 {
	float humidity;
	float temperature;
} measurement_sht21_t;

int getSensorFncSht21(devSensorFunc_t *cfgFuncs);

#endif /* H_SHT21_H_ */
