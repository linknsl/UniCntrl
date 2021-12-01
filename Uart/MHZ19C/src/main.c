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

char *subscribe[] = { "setCalibrateSpan", "setCalibrate" };

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	int num = 0;

	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
		num = (eMhz19_calibrate) atoi(message->payload);
		calibrateSpan(num);
	}
	if (mqtt_set_topic_sub(obj, subscribe[1], message->topic)) {
		num = (eMhz19_calibrate) atoi(message->payload);
		cntrlCalibrate(num);
	}
}

void mqtt_subscribe_init(char *topic) {
	int i = 0;
	char *mess;
	for (mess = subscribe[i]; mess; i++, mess = subscribe[i]) {
		mqtt_gen_topic_and_sub(topic, subscribe[i]);
	}
}

void* read_sensor(void *param) {
	int *id = (int*) param;
	measurement_mhz19_t measurement;
	int value_array[10];
	usr_cfg_t ucfg;
	read_usr_uart_conf(&ucfg, *id);
	char *params[10];
	get_usr_param_cnf(ucfg.mqtt_read, params);

#ifdef ARM
	autoConfUart(*id) ;
#else
#endif

	devSensorFunc_t dSf;
    osMemSet(&dSf, 0, sizeof(devSensorFunc_t));
	getSensorFncMhz19c(&dSf);
	ucfg.mqtt_general->fun_mess_clb = message_callback;
	mqtt_setup(ucfg.mqtt_general);
	mqtt_subscribe_init(ucfg.mqtt_general->topic);
	while (1) {
		dSf.getMeasurement(&measurement);
		value_array[0] = measurement.temperature;
		value_array[1] = measurement.co2_ppm;
		mqttResultPubInt(&ucfg, value_array);
		usleep(100);
	}
	closeUart();
	pthread_exit(SUCCESS);
	return SUCCESS;
}

void terminate(int param) {
	closeUart();
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
