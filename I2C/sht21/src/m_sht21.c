/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <sht21.h>
#include <common.h>

void* read_sensor(void *param) {
	float *value_array_float = NULL;
	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	init_conf_t ic;

	getSensorFncSht21(&dSf);
	init(param, &ucfg, &dSf, &ic, I2CS);
	value_array_float = malloc((ucfg.mqtt_read->param_size) * sizeof(float));
	while (1) {
		if (dSf.getMeasurementFloat(value_array_float, &ic) == SUCCESS)
			mqttResultPubFloat(&ucfg, value_array_float);
		usleep(100);
	}
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
