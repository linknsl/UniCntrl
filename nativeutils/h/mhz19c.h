/*
 * MHZ19C.h
 *
 *  Created on: 14 нояб. 2021 г.4e76trf
 *      Author: root
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#ifndef MHZ19C_H_
#define MHZ19C_H_

#define REQUEST_CNT 8
#define RESPONSE_CNT 9

enum MHZ19_UART_DATA {
	PPM, TEMPERATURE, STAT
};

typedef enum MHZ19_CALIBRATE {
	ZERO, SPAN, AUTO_ON, AUTO_OFF
} eMhz19_calibrate;

typedef struct measurement_mhz19 {
	int co2_ppm;
	int temperature;
	int state;
	bool cheksum;
} measurement_mhz19_t;

uint8_t mhz19_checksum(uint8_t com[]);
void writeCommand(uint8_t cmd[], uint8_t *response);
int getStatus(void);
void setPollingTimeMhz19c(int pol_time);
measurement_mhz19_t getMeasurementMhz19c(void);
void calibrateSpan(int ppm);
void cntrlCalibrate(eMhz19_calibrate key);
void calibrateZero(void);
void setAutoCalibration( bool autocalib);

#endif /* MHZ19C_H_ */
