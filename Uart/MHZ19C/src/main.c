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
#include <mqtt.h>
#include <read_confuguration_file.h>
#include <mosquitto.h>
#include <uart_setting.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>

int fd;

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message) {
	bool match = 0;
	int num = 0;
	printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

	mosquitto_topic_matches_sub("uart1/MHZ19C/setCalibrateSpan", message->topic, &match);
	if (match) {
		num = atoi(message->payload);
		calibrateSpan(fd, num);
	}
	mosquitto_topic_matches_sub("uart1/MHZ19C/setCalibrate", message->topic, &match);
	if (match) {
		num = atoi(message->payload);
		cntrlCalibrate(fd, (eMhz19_calibrate) num);
	}
}

void mqtt_subscribe_init(char *topic) {
	mqtt_gen_topic_and_sub(topic, "setCalibrate");
	mqtt_gen_topic_and_sub(topic, "setCalibrateSpan");
}

void* read_z19c_to_public(void *param) {
	int *thr = (int*) param;

	measurement_t measurement;
	mqtt_setting_t ms;
	uart_setting_t us;

	if (!readConfUart(&us, 0)) { /* настройки порта из конфиг файла*/
		fd = confUart(us.device, us.speed, us.databits, us.stopbits);
	} else {/* настройки порта по умолчанию если нет в конфиг файле*/
		fd = defaultConfUart(NULL);
	}
	ms.fun_mess_clb = message_callback;
	mqtt_setup(&ms);
	mqtt_subscribe_init(ms.topic);
	while (1) {
		measurement = getMeasurement(fd);
		mqtt_gen_topic_and_pub_int(ms.topic, "co2_ppm", measurement.co2_ppm);
		mqtt_gen_topic_and_pub_int(ms.topic, "temperature", measurement.temperature);
		printf("co2_ppm:%d temperature:%d state:%d\n", measurement.co2_ppm, measurement.temperature, measurement.state);
		usleep(1000);
	}
	close(fd);
	pthread_exit(0);
	return SUCCESS;
}

void terminate(int param) {
	exit(1);                               // завершить работу программы со значением 1
}

int main(int argc, char *argv[]) {
	signal(SIGTERM, terminate);              // обработка сигнала
	signal(SIGINT, terminate);              // обработка сигнала

	pthread_t thr;
	int status;
	status = pthread_create(&thr, NULL, read_z19c_to_public, &thr);
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
