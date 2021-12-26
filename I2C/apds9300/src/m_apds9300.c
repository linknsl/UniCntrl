/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <apds9300.h>
#include <common.h>

void* read_sensor(void *param) {
	int *value_array_int = NULL;
	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	init_conf_t ic;

	getSensorFncAPDS9300(&dSf);
	init(param, &ucfg, &dSf, &ic, I2CS);
	value_array_int = malloc((ucfg.mqtt_read->param_size) * sizeof(int));
	while (1) {
		if (dSf.getMeasurement(value_array_int, &ic) == SUCCESS)
			mqttResultPubInt(&ucfg, value_array_int);
		usleep(100);
	}
	pthread_exit(SUCCESS);
	return SUCCESS;
}

void terminate(int param) {
	exit(FAILURE);
}

int main(int argc, const char *argv[]) {

	if (osStartEngine(argc, argv, read_sensor) != SUCCESS) {
		exit(FAILURE);
	} else {
		exit(SUCCESS);
	}
}
