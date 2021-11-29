/*
 * sht21.h
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#ifndef H_SHT21_H_
#define H_SHT21_H_

#define HWMON_NEW_DEVICE "/sys/bus/i2c/devices/i2c-0/new_device"
#define HWMON_SHT21 "/sys/bus/i2c/devices/0-0040/hwmon/hwmon1"
#define SHT21_TEMP "temp1_input"
#define SHT21_HUMIDITY "humidity1_input"

typedef struct measurement_sht21 {
	float humidity;
	float temperature;
} measurement_sht21_t;

float calc_humidity_sht21(float humidity_raw);
float calc_temperature_sht21(float temp_raw);
measurement_sht21_t getMeasurementSht21(void);
void setPollingTimeSht21(int pol_time);
int sht21_init(void);

#endif /* H_SHT21_H_ */
