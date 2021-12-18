/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <ds18b20.h>
#include <common.h>

void* read_sensor(void *param) {

	float *value_array;

	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	init_conf_t ic;
	ic.id = 0;

	getSensorFncDs18b20(&dSf);

	init(param, &ucfg, &dSf, ONEW1S);
	value_array = malloc((ucfg.mqtt_read->param_size) * sizeof(float));
	while (1) {
		dSf.getMeasurementFloat(value_array, &ic);
		mqttResultPubFloat(&ucfg, value_array);
		usleep(100);
	}
	pthread_exit(SUCCESS);
	return SUCCESS;
}

void terminate(int param) {
	exit(FAILURE);
}

int main(int argc, char *argv[]) {
	int id;

	signal(SIGTERM, terminate);
	signal(SIGINT, terminate);

	if (argc < 2) {
		printf("You need input id aplication.\n");
		exit(1);
	}
	id = atoi(argv[1]);
	printf("Start aplication id %d \n", id);

	pthread_t thr;
	int status;
	status = pthread_create(&thr, NULL, read_sensor, &id);
	if (status != 0) {
		printf("main error: can't create thread, status = %d\n", status);
		exit(ERROR_CREATE_THREAD);
	}

	status = pthread_join(thr, 0);
	if (status != SUCCESS) {
		printf("main error: can't join thread, status = %d\n", status);
		exit(ERROR_JOIN_THREAD);
	}
	exit(SUCCESS);
}
