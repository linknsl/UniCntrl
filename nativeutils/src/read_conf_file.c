/*
 * read_conf.c
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */
#include <libconfig.h>
#include <common.h>

int readConfUart(uart_setting_t *us, int numInstance) {

	config_t cfg;
	config_setting_t *setting;

	us->device = (char*) malloc(SIZE_STRING);
	us->name = (char*) malloc(SIZE_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_UART);
	if (setting != NULL) {
		config_setting_t *uart_set = config_setting_get_elem(setting, numInstance);

		const char *device = NULL, *name = NULL;

		if ((config_setting_lookup_string(uart_set, "device", &device)
				&& config_setting_lookup_string(uart_set, "name", &name)
				&& config_setting_lookup_int(uart_set, "mqtt_id", &us->mqtt_id)
				&& config_setting_lookup_int(uart_set, "speed", &us->speed)
				&& config_setting_lookup_int(uart_set, "databits", &us->databits)
				&& config_setting_lookup_int(uart_set, "stopbits", &us->stopbits))) {
			memcpy(us->device, device, strlen(device) + 1);
			memcpy(us->name, name, strlen(name) + 1);
		}
	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int readGnrlMqtt(mqtt_config_t *ms, int version) {
	config_t cfg;
	config_setting_t *setting = NULL;

	ms->server = malloc(SIZE_LONG_STRING);
	ms->topic = malloc(SIZE_LONG_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_MQTT);
	if (setting != NULL) {

		const char *server = NULL;
		config_setting_t *mqtt_setting = config_setting_get_elem(setting, version);
		config_setting_lookup_string(mqtt_setting, "server", &server);
		config_setting_lookup_int(mqtt_setting, "number_board", &ms->number_board);
		config_setting_lookup_int(mqtt_setting, "port", &ms->port);
		config_setting_lookup_int(mqtt_setting, "keepalive", &ms->keepalive);
		memcpy(ms->server, server, strlen(server) + 1);
	}
	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int readConfMqtt(mqtt_config_read_t *msr, int mqtt_id) {

	config_t cfg;
	config_setting_t *setting = NULL;

	int j = 0, rc;
	msr->sensor_name = malloc(SIZE_LONG_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_MQTT_READ);
	if (setting != NULL) {
		const char *sensor = NULL;

		config_setting_t *mqtt_read = config_setting_get_elem(setting, mqtt_id);
		config_setting_lookup_int(mqtt_read, "id", &msr->id);
		config_setting_lookup_int(mqtt_read, "polling_time", &msr->polling_time);
		config_setting_lookup_string(mqtt_read, "sensor", &sensor);
		memcpy(msr->sensor_name, sensor, strlen(sensor) + 1);

		params_t *params = NULL;
		char *param = malloc(64);
		rc = 1, j = 1;
		while (rc) {
			const char *new_param = NULL;
			sprintf(param, "param%d", j++);
			rc = config_setting_lookup_string(mqtt_read, param, &new_param);
			if (rc) {
				params_t *p = malloc(sizeof(params_t));
				p->param = malloc(strlen(new_param) + 1);
				memcpy(p->param, new_param, strlen(new_param) + 1);
				p->next = NULL;

				if (params) {
					params->next = p;
					params = params->next;
				} else {
					params = p;
				}

				if (!msr->params)
					msr->params = p;
			}
		}
	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int get_usr_param_cnf(mqtt_config_read_t *ms, char *param[]) {
	params_t *p;
	int i = 0;
	for (p = ms->params; p; p = p->next) {
		param[i] = malloc(SIZE_STRING);
		memcpy(param[i], p->param, strlen(p->param) + 1);
		i++;
	}
	return 0;
}

int read_usr_uart_conf(usr_cfg_t *ms, int numInstance) {

	ms->dev_cfg = malloc(sizeof(uart_setting_t));
	readConfUart(ms->dev_cfg, numInstance);

	ms->mqtt_general = malloc(sizeof(mqtt_config_t));
	readGnrlMqtt(ms->mqtt_general, VERSION_MQTT_GNRL);

	ms->mqtt_read = malloc(sizeof(mqtt_config_read_t));
	readConfMqtt(ms->mqtt_read, ms->dev_cfg->mqtt_id);

	sprintf(ms->mqtt_general->topic, "%d/%d/%s/%s", ms->mqtt_general->number_board, numInstance, ms->dev_cfg->name,
			ms->mqtt_read->sensor_name);

	return (EXIT_SUCCESS);
}
