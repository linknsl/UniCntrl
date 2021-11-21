/*
 * MHZ19C.h
 *
 *  Created on: 14 нояб. 2021 г.
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

#ifndef MHZ19C_H_
#define MHZ19C_H_

#define REQUEST_CNT 8
#define RESPONSE_CNT 9

typedef enum bool {false=0, true=1} bool;

enum MHZ19_UART_DATA
{
	PPM,
	TEMPERATURE,
	STAT
};

typedef struct measurement {
	int co2_ppm;
	int temperature;
	int state;
} measurement_t;

uint8_t mhz19_checksum(uint8_t com[]);
void writeCommand(uint8_t cmd[], uint8_t *response,int fd);
int getStatus();
measurement_t getMeasurement(int fd);
void calibrateSpan(int ppm);
void calibrateZero();
void setAutoCalibration(bool autocalib);

#endif /* MHZ19C_H_ */
