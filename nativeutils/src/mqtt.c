/*
 needs libmosquitto-dev
 $ gcc -o libmosq libmosq.c -lmosquitto
 */
#include <mhz19c.h>
#include <sht21.h>
#include <common.h>

int fd_uart, numInstance = 0;
struct mosquitto *mosq = NULL;
void connect_callback(struct mosquitto *mosq, void *obj, int result) {
	printf("connect callback, rc=%d\n", result);
}

int mqtt_set_topic_sub(void *obj, char *param, char *topic) {

	bool match = 0;
	char path[MAX_BUF];
	sprintf(path, "%s/%s", (char*) obj, param);
	mosquitto_topic_matches_sub(path, topic, &match);

	return match;
}

void mosq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str) {
	switch (level) {
	case MOSQ_LOG_DEBUG:
	case MOSQ_LOG_INFO:
	case MOSQ_LOG_NOTICE:
	case MOSQ_LOG_WARNING:
	case MOSQ_LOG_ERR: {
		printf("%i:%s\n", level, str);
	}
	}
}

void mqtt_setup(mqtt_config_t *ms) {
	char *host = ms->server;
	int port = ms->port;
	int keepalive = ms->keepalive;
	bool clean_session = true;

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, ms->topic);
	if (!mosq) {
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	} else {
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, ms->fun_mess_clb);
	}

	/*	if (ms->log != 0)
	 mosquitto_log_callback_set(mosq, mosq_log_callback);*/

	if (mosquitto_connect(mosq, host, port, keepalive)) {
		fprintf(stderr, "Unable to connect.\n");
		exit(1);
	}
	int loop = mosquitto_loop_start(mosq);
	if (loop != MOSQ_ERR_SUCCESS) {
		fprintf(stderr, "Unable to start loop: %i\n", loop);
		exit(1);
	}
}

int mqtt_gen_topic_and_sub(char *topic, char *sub_topic) {
	char fulltopic[SIZE_LONG_STRING];
	sprintf(fulltopic, "%s/%s", topic, sub_topic);
	return mqtt_sub(fulltopic);
}

int mqttResultPubInt(usr_cfg_t *ucfg, int *value_array) {
	params_t *item;
	int i;
	for (item = ucfg->mqtt_read->params, i = 0; item; item = item->next, i++) {
		mqtt_gen_topic_and_pub_int(ucfg->mqtt_general->topic, item->param, value_array[i]);
	}
	return SUCCESS;
}

int mqttResultPubFloat(usr_cfg_t *ucfg, float *value_array) {
	params_t *item;
	int i;
	for (item = ucfg->mqtt_read->params, i = 0; item; item = item->next, i++) {
		mqtt_gen_topic_and_pub_float(ucfg->mqtt_general->topic, item->param, value_array[i]);
	}
	return SUCCESS;
}

int mqtt_gen_topic_and_pub_int(char *topic, char *sub_topic, int value) {
	int snd;
	char message[SIZE_STRING];
	char fulltopic[SIZE_LONG_STRING];

	sprintf(fulltopic, "%s/%s", topic, sub_topic);
	sprintf(message, "%d", value);

	snd = mqtt_send(message, fulltopic);
	if (snd != 0) {
		fprintf(stderr, "mqtt_send error: %i\n", snd);
		return -1;
	} else {
		return 0;
	}
}

int mqtt_gen_topic_and_pub_hex(char *topic, char *sub_topic, int value) {
	int snd;
	char message[SIZE_STRING];
	char fulltopic[SIZE_LONG_STRING];

	sprintf(fulltopic, "%s/%s", topic, sub_topic);
	sprintf(message, "0x%x", value);

	snd = mqtt_send(message, fulltopic);
	if (snd != 0) {
		fprintf(stderr, "mqtt_send error: %i\n", snd);
		return -1;
	} else {
		return 0;
	}
}

int mqtt_gen_topic_and_pub_float(char *topic, char *sub_topic, float value) {
	int snd;
	char message[SIZE_STRING];
	char fulltopic[SIZE_LONG_STRING];

	sprintf(fulltopic, "%s/%s", topic, sub_topic);
	sprintf(message, "%f", value);

	snd = mqtt_send(message, fulltopic);
	if (snd != 0) {
		fprintf(stderr, "mqtt_send error: %i\n", snd);
		return -1;
	} else {
		return 0;
	}
}

int mqtt_sub(char *topic) {
	return mosquitto_subscribe(mosq, NULL, topic, 0);
}

int mqtt_send(char *msg, char *topic) {
	return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}
