/*
 * sht21.h
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_SHT21_H_
#define H_SHT21_H_

#include <common.h>

#define HWMON_DS18B20 "/sys/bus/w1/devices"
#define DS18B20_TEMP "w1_slave"

typedef struct measurement_ds18b20 {
	float temperature;
} measurement_ds18b20;

int getSensorFncDs18b20(devSensorFunc_t *cfgFuncs);

#endif /* H_SHT21_H_ */
