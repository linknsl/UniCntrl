/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <mhz19c.h>
#include <uart_setting.h>
#include <common.h>

void* read_sensor(void *param) {
	int *value_array_int = NULL;
	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	devFunc_t df_uart;
	init_conf_t ic;

	getFncUart(&df_uart);
	dSf.devFunc = &df_uart;

	getSensorFncMhz19c(&dSf);
	init(param, &ucfg, &dSf, &ic, UARTS);
	value_array_int = malloc((ucfg.mqtt_read->param_size) * sizeof(int));
	while (1) {
		if (dSf.getMeasurement(value_array_int, &ic) == SUCCESS)
			mqttResultPubInt(&ucfg, value_array_int);
		usleep(100);
	}
	df_uart.closeDev();

	pthread_exit(SUCCESS);
	return SUCCESS;
}

int main(int argc, const char *argv[]) {

	if (osStartEngine(argc, argv, read_sensor) != SUCCESS) {
		exit(FAILURE);
	} else {
		exit(SUCCESS);
	}
}
