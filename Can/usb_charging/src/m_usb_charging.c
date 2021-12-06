/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <can_setting.h>
#include <usb_charging.h>
#include <common.h>

/*"usb_current[3]","usb_voltage[3]" */
#define PARAM_FLOAT_USB_CHARGING_SIZE 6
/* "soft_id","hard_id","uniq_id","usb1_mistake","usb2_mistake","usb3_mistake","total_mistake","temperature"*/
#define PARAM_INT_USB_CHARGING_SIZE 8

void* read_sensor(void *param) {
	int *value_array_int = malloc((PARAM_INT_USB_CHARGING_SIZE) * sizeof(int));
	float *value_array_float = malloc((PARAM_FLOAT_USB_CHARGING_SIZE) * sizeof(float));
	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	devFunc_t df_can;
	init_conf_t ic;
	ic.id =0;

	getFncCan(&df_can);
	dSf.devFunc = &df_can;

	getSensorFncUsbCharging(&dSf);
	init(param, &ucfg, &dSf, CANS);

	while (1) {
		dSf.getMeasurement(value_array_int, &ic);
		mqttResultPubInt(&ucfg, value_array_int);

		dSf.getMeasurementFloat(value_array_float, &ic);
		mqttResultPubFloat(&ucfg, value_array_float);
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
