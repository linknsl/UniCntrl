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


char *subscribe[] = { "reset" };
char *publicate[] = { "humidity", "temperature" };

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	int num = 0;

	if (mqtt_set_topic_sub(obj, subscribe[0], message->topic)) {
		num = atoi(message->payload);
		printf("reset %d ",num);
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
	measurement_sht21_t measurement;
	mqtt_setting_t ms;

#ifdef ARM
	rc = sht21_init() ;
#else
#endif
	newconfig();
	ms.fun_mess_clb = message_callback;
	mqtt_setup(&ms);
	mqtt_subscribe_init(ms.topic);
	while (1) {
		measurement = getMeasurementSht21();
		mqtt_gen_topic_and_pub_float(ms.topic, publicate[0], measurement.humidity);
		mqtt_gen_topic_and_pub_float(ms.topic, publicate[1], measurement.temperature);
		usleep(100);
	}
	pthread_exit(SUCCESS);
	return SUCCESS;
}

void terminate(int param) {
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
