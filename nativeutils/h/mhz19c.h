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

void calibrateSpan(int ppm);
void cntrlCalibrate(eMhz19_calibrate key);
void calibrateZero(void);
void setAutoCalibration( bool autocalib);
int getSensorFncMhz19c(devSensorFunc_t *cfgFuncs);

#endif /* MHZ19C_H_ */
