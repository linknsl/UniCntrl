/*
 * sht21.h
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_SHT21_H_
#define H_SHT21_H_

#include <common.h>

#define HWMON_ROOT_APDS9300 "/sys/class/i2c-dev"
#define HWMON_PREFIX_APDS9300 "iio\:device0"
/*char HWMON_PREFIX_APDS9300[] = "iio\\:device0";*/

#define APDS9300_ILLUMINANCE "in_illuminance0_input"
#define APDS9300_INTENSITY0 "in_intensity0_raw"
#define APDS9300_INTENSITY1 "in_intensity1_raw"

typedef struct measurement_apds9300 {
	int in_illuminance;
	int in_intensity0;
	int in_intensity1;
} measurement_apds9300_t;

int getSensorFncAPDS9300(devSensorFunc_t *cfgFuncs);

#endif /* H_SHT21_H_ */
