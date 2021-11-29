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
char *publicate[] = { "co2_ppm", "temperature" };

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
void mqtt_read_config(mqtt_config_t *obj) {
	readConfMqtt_new(obj);

	mqtt_config_children_t *item;
	params_t *params;
	for (item = obj->children; item; item = item->children) {
		printf("%s \n", item->sensor_name);

		for (params = item->params; params; params = params->next) {
			printf("%s \n", params->param);
		}
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
	measurement_mhz19_t measurement;
	mqtt_setting_t ms;
#ifdef ARM
	autoConfUart() ;
#else
#endif
	mqtt_config_t cfg;
	mqtt_read_config(&cfg);
	newconfig();
	ms.fun_mess_clb = message_callback;
	mqtt_setup(&ms);
	mqtt_subscribe_init(ms.topic);
	while (1) {
		measurement = getMeasurementMhz19c();
		mqtt_gen_topic_and_pub_int(ms.topic, publicate[0], measurement.co2_ppm);
		mqtt_gen_topic_and_pub_int(ms.topic, publicate[1], measurement.temperature);
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
	signal(SIGTERM, terminate);
	signal(SIGINT, terminate);

	pthread_t thr;
	int status;
	status = pthread_create(&thr, NULL, read_sensor, &thr);
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
