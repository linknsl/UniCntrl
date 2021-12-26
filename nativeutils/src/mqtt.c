/*
 needs libmosquitto-dev
 $ gcc -o libmosq libmosq.c -lmosquitto
 */
#include <mhz19c.h>
#include <sht21.h>
#include <common.h>

int fd_uart, numInstance = 0;
struct mosquitto *mosq = NULL;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void connect_callback(struct mosquitto *mosq, void *obj, int result) {
	LOG("connect callback, rc=%d\n", result);
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
		LOG_ERROR("%i:%s", level, str);
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
		LOG_ERROR("Error: Out of memory.");
		exit(1);
	} else {
		mosquitto_connect_callback_set(mosq, connect_callback);
		mosquitto_message_callback_set(mosq, ms->fun_mess_clb);
	}

	/*	if (ms->log != 0)
	 mosquitto_log_callback_set(mosq, mosq_log_callback);*/

	while (1) {

		if (mosquitto_connect(mosq, host, port, keepalive)) {
			LOG_ERROR("waiting to connect server. %s", host);
			sleep(1);
		} else {
			break;
		}
	}
	int loop = mosquitto_loop_start(mosq);
	if (loop != MOSQ_ERR_SUCCESS) {
		LOG_ERROR("Unable to start loop: %i", loop);
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
	int i, j, start = ucfg->mqtt_read->param_int.start;
	for (item = ucfg->mqtt_read->params, i = 0; item; item = item->next, i++) {

		j = start ? (i >= start) ? i - start : -1 : i;

		if (j == -1)
			continue;

		if (mqtt_gen_topic_and_pub(ucfg->mqtt_general->topic, item->param, &value_array[j], INT) == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

int mqttResultPubFloat(usr_cfg_t *ucfg, float *value_array) {
	params_t *item;
	int i, j, start = ucfg->mqtt_read->param_float.start;
	int end = ucfg->mqtt_read->param_int.start;

	for (item = ucfg->mqtt_read->params, i = 0; item; item = item->next, i++) {
		j = start ? (i >= start && i < end) ? i - start : -1 : i;

		if (j == -1)
			continue;

		if (mqtt_gen_topic_and_pub(ucfg->mqtt_general->topic, item->param, &value_array[j], FLOAT) == FAILURE) {
			return FAILURE;
		}
	}
	return SUCCESS;
}

int mqtt_gen_topic_and_pub(char *topic, char *sub_topic, void *value, eSend_type type) {
	char message[SIZE_STRING];
	memset(message, 0, SIZE_STRING);
	char fulltopic[SIZE_LONG_STRING];
	memset(fulltopic, 0, SIZE_LONG_STRING);

	sprintf(fulltopic, "%s/%s", topic, sub_topic);
	switch (type) {
	case INT:
		sprintf(message, "%d", *((int*) value));
		break;
	case HEX:
		sprintf(message, "0x%x", *((int*) value));
		break;
	case FLOAT:
		sprintf(message, "%f", *((float*) value));
		break;
	case STRING:
		sprintf(message, "%s", (char*) value);
		break;
	default:
		break;
	}

	if (mqtt_send(message, fulltopic) != 0) {
		return FAILURE;
	} else {
		return SUCCESS;
	}
}

int mqtt_sub(char *topic) {
	return mosquitto_subscribe(mosq, NULL, topic, 0);
}

int mqtt_send(char *msg, char *topic) {
	int rc = 0;
	pthread_mutex_lock(&mtx);
	rc = mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
	pthread_mutex_unlock(&mtx);
	return rc;
}
