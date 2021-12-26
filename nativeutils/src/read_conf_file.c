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
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
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

int readConfI2C(i2c_setting_t *is, int numInstance) {

	config_t cfg;
	config_setting_t *setting;

	is->device = (char*) malloc(SIZE_STRING);
	is->name = (char*) malloc(SIZE_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_I2C);
	if (setting != NULL) {
		config_setting_t *i2c_set = config_setting_get_elem(setting, numInstance);

		const char *device = NULL, *name = NULL;

		if ((config_setting_lookup_string(i2c_set, "device", &device)
				&& config_setting_lookup_string(i2c_set, "name", &name)
				&& config_setting_lookup_int(i2c_set, "mqtt_id", &is->mqtt_id)
				&& config_setting_lookup_int(i2c_set, "addr", &is->addr))) {
			memcpy(is->device, device, strlen(device) + 1);
			memcpy(is->name, name, strlen(name) + 1);
		}
	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int readConfOneW1(onew1_setting_t *os, int numInstance) {

	config_t cfg;
	config_setting_t *setting;

	os->device = (char*) malloc(SIZE_STRING);
	os->name = (char*) malloc(SIZE_STRING);
	os->addr = (char*) malloc(SIZE_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_ONEW1S);
	if (setting != NULL) {
		config_setting_t *one_set = config_setting_get_elem(setting, numInstance);

		const char *device = NULL, *name = NULL, *addr = NULL;

		if ((config_setting_lookup_string(one_set, "device", &device)
				&& config_setting_lookup_string(one_set, "name", &name)
				&& config_setting_lookup_int(one_set, "mqtt_id", &os->mqtt_id)
				&& config_setting_lookup_string(one_set, "addr", &addr))) {
			memcpy(os->device, device, strlen(device) + 1);
			memcpy(os->name, name, strlen(name) + 1);
			memcpy(os->addr, addr, strlen(addr) + 1);
		}
	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int readConfCAN(can_setting_t *cs, int numInstance) {

	config_t cfg;
	config_setting_t *setting;

	cs->device = (char*) malloc(SIZE_STRING);
	cs->name = (char*) malloc(SIZE_STRING);

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_CANS);
	if (setting != NULL) {
		config_setting_t *can_set = config_setting_get_elem(setting, numInstance);

		const char *device = NULL, *name = NULL;

		if ((config_setting_lookup_string(can_set, "device", &device)
				&& config_setting_lookup_string(can_set, "name", &name)
				&& config_setting_lookup_int(can_set, "mqtt_id", &cs->mqtt_id)
				&& config_setting_lookup_int(can_set, "addr", &cs->addr))) {
			memcpy(cs->device, device, strlen(device) + 1);
			memcpy(cs->name, name, strlen(name) + 1);
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
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
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
		LOG_ERROR("%s:%d - %s", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
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
		char *param = malloc(SIZE_PARAM);
		rc = 1, j = 1;
		msr->params = NULL;
		while (rc) {
			const char *new_param = NULL;
			sprintf(param, "param%d", j);
			rc = config_setting_lookup_string(mqtt_read, param, &new_param);
			if (rc) {
				msr->param_size = j;
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
				j++;
			}
		}
	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

int get_usr_param_cnf(mqtt_config_read_t *ms, char **param) {
	int i = 0;
	params_t *p;
	param = malloc(ms->param_size * SIZE_PARAM);
	memset(param, 0, ms->param_size * SIZE_PARAM);
	for (p = ms->params; p; p = p->next) {
		param[i] = malloc(SIZE_PARAM);
		memset(param[i], 0, SIZE_PARAM);
		memcpy(param[i], p->param, strlen(p->param) + 1);
		i++;
	}
	return SUCCESS;
}

int read_dev_configure(usr_cfg_t *uc, eRead_configure block, int *id, char *name, int numInstance) {

	switch (block) {
	case UARTS:
		uc->dev_cfg = malloc(sizeof(uart_setting_t));
		readConfUart(uc->dev_cfg, numInstance);
		*id = ((uart_setting_t*) uc->dev_cfg)->mqtt_id;
		memcpy(name, ((uart_setting_t*) uc->dev_cfg)->name, strlen(((uart_setting_t*) uc->dev_cfg)->name) + 1);
		break;
	case I2CS:
		uc->dev_cfg = malloc(sizeof(i2c_setting_t));
		readConfI2C(uc->dev_cfg, numInstance);
		*id = ((i2c_setting_t*) uc->dev_cfg)->mqtt_id;
		memcpy(name, ((i2c_setting_t*) uc->dev_cfg)->name, strlen(((i2c_setting_t*) uc->dev_cfg)->name) + 1);
		break;
	case ONEW1S:
		uc->dev_cfg = malloc(sizeof(onew1_setting_t));
		readConfOneW1(uc->dev_cfg, numInstance);
		*id = ((onew1_setting_t*) uc->dev_cfg)->mqtt_id;
		memcpy(name, ((onew1_setting_t*) uc->dev_cfg)->name, strlen(((onew1_setting_t*) uc->dev_cfg)->name) + 1);
		break;
	case CANS:
		uc->dev_cfg = malloc(sizeof(can_setting_t));
		readConfCAN(uc->dev_cfg, numInstance);
		*id = ((can_setting_t*) uc->dev_cfg)->mqtt_id;
		memcpy(name, ((can_setting_t*) uc->dev_cfg)->name, strlen(((can_setting_t*) uc->dev_cfg)->name) + 1);
		break;
	default:
		break;
	}
	return 0;
}

int read_usr_configure(usr_cfg_t *uc, int numInstance, eRead_configure block) {
	int id;
	char *name = NULL;
	name = (char*) malloc(SIZE_STRING);
	read_dev_configure(uc, block, &id, name, numInstance);

	uc->mqtt_general = malloc(sizeof(mqtt_config_t));
	memset(uc->mqtt_general, 0, sizeof(mqtt_config_t));
	readGnrlMqtt(uc->mqtt_general, VERSION_MQTT_GNRL);

	uc->mqtt_read = malloc(sizeof(mqtt_config_read_t));
	memset(uc->mqtt_read, 0, sizeof(mqtt_config_read_t));
	readConfMqtt(uc->mqtt_read, id);

	sprintf(uc->mqtt_general->topic, "%d/%d/%s/%s", uc->mqtt_general->number_board, numInstance, name,
			uc->mqtt_read->sensor_name);

	return (EXIT_SUCCESS);
}
