/*
 ============================================================================
 Name        : sht21.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <common.h>
#include <sht21.h>

float calc_temperature_sht21(float temp_raw) {
	float result = ((175.72 * (temp_raw / 65536)) - 46.85);
	return result;
}

float calc_humidity_sht21(float humidity_raw) {
	float result = (125 * (humidity_raw / 65536) - 6);
	return result;
}

measurement_sht21_t getMeasurementSht21(void) {
	measurement_sht21_t measurement;
	measurement.humidity = calc_humidity_sht21(get_setting_float(HWMON_SHT21, SHT21_HUMIDITY));
	measurement.temperature = calc_temperature_sht21(get_setting_float(HWMON_SHT21, SHT21_TEMP));
	return measurement;
}

int sht21_init(void) {
	FILE *fp;
	char path[MAX_BUF];
	char start[MAX_BUF];
	snprintf(start, sizeof start, "%s/%s", HWMON_SHT21, SHT21_HUMIDITY);
	if (!access(path, F_OK)) {
		snprintf(path, sizeof(path), HWMON_NEW_DEVICE);
		if ((fp = fopen(path, "w")) == NULL) {
			printf("file open failed\n");
			return FAILURE;
		}

		rewind(fp);
		fprintf(fp, "sht21 0x40\n");
		fflush(fp);
		fclose(fp);
	}
	if (!access(start, F_OK)) {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}
