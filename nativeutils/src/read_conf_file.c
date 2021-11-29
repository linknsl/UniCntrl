/*
 * read_conf.c
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */
#include <libconfig.h>
#include <common.h>

int readConfUart(uart_setting_t *us, int numInstance) {

	us->device = malloc(SIZE_STRING);
	/* используются свои типы. */
	config_t cfg;
	config_setting_t *setting;

	config_init(&cfg); /* обязательная инициализация */

	/* Читаем файл. Если ошибка, то завершаем работу */
	if (!config_read_file(&cfg, "unicntrl.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	/* Вывод списка книжек с полок. */
	setting = config_lookup(&cfg, "application.uarts.setting");
	if (setting != NULL) {
		config_setting_t *uart_setting = config_setting_get_elem(setting, numInstance);
		/* Выводим только те записи, если они имеют все нужные поля. */
		const char *device;

		if ((config_setting_lookup_string(uart_setting, "device", &device)
				&& config_setting_lookup_int(uart_setting, "speed", &us->speed)
				&& config_setting_lookup_int(uart_setting, "databits", &us->databits)
				&& config_setting_lookup_int(uart_setting, "stopbits", &us->stopbits))) {
			memcpy(us->device, device, SIZE_STRING);
		}
	}

	config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
	return (EXIT_SUCCESS);
}

int readConfMqtt(mqtt_setting_t *ms, int numInstance) {

	ms->topic = malloc(SIZE_LONG_STRING);
	ms->host = malloc(SIZE_STRING);
	ms->name = malloc(SIZE_STRING);
	/* используются свои типы. */
	config_t cfg;
	config_setting_t *setting;

	config_init(&cfg); /* обязательная инициализация */

	/* Читаем файл. Если ошибка, то завершаем работу */
	if (!config_read_file(&cfg, "unicntrl.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	/* Вывод списка книжек с полок. */
	setting = config_lookup(&cfg, "application.uarts.mqtt");
	if (setting != NULL) {
		const char *topic, *name, *host;
		config_setting_t *mqtt_setting = config_setting_get_elem(setting, numInstance);

		if ((config_setting_lookup_string(mqtt_setting, "topic", &topic)
				&& config_setting_lookup_string(mqtt_setting, "name", &name)
				&& config_setting_lookup_string(mqtt_setting, "host", &host)
				&& config_setting_lookup_int(mqtt_setting, "port", &ms->port)
				&& config_setting_lookup_int(mqtt_setting, "keepalive", &ms->keepalive)
				&& config_setting_lookup_int(mqtt_setting, "polling_time", &ms->polling_time)
				&& config_setting_lookup_int(mqtt_setting, "number", &ms->number)
				&& config_setting_lookup_int(mqtt_setting, "log", &ms->log))) {
			memcpy(ms->topic, topic, SIZE_LONG_STRING);
			memcpy(ms->name, name, SIZE_STRING);
			memcpy(ms->host, host, SIZE_STRING);
		}

	}

	config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
	return (EXIT_SUCCESS);
}

int readConfMqtt_new(mqtt_config_t *ms) {
	int i, j = 1, rc, count;
	ms->server = malloc(SIZE_LONG_STRING);
	/* используются свои типы. */
	config_t cfg;
	config_setting_t *setting = NULL;

	config_init(&cfg); /* обязательная инициализация */

	/* Читаем файл. Если ошибка, то завершаем работу */
	if (!config_read_file(&cfg, "unicntrl.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	/* Вывод списка книжек с полок. */
	setting = config_lookup(&cfg, "application.mqtt.general");
	if (setting != NULL) {

		const char *server;
		config_setting_t *mqtt_setting = config_setting_get_elem(setting, 0);
		config_setting_lookup_string(mqtt_setting, "server", &server);
		config_setting_lookup_int(mqtt_setting, "number_board", &ms->number_board);
		config_setting_lookup_int(mqtt_setting, "port", &ms->port);
		config_setting_lookup_int(mqtt_setting, "keepalive", &ms->keepalive);
		memcpy(ms->server, server, SIZE_LONG_STRING);
	}
	setting = config_lookup(&cfg, "application.mqtt.read");
	if (setting != NULL) {
		const char *sensor;

		count = config_setting_length(setting);
		mqtt_config_children_t *parent = NULL;
		for (i = 0, rc = 1, j = 1; i < count; ++i) {

			mqtt_config_children_t *mqtt_chld = malloc(sizeof(mqtt_config_children_t));
			mqtt_chld->sensor_name = malloc(SIZE_LONG_STRING);
			config_setting_t *mqtt_read = config_setting_get_elem(setting, i);
			config_setting_lookup_int(mqtt_read, "id", &mqtt_chld->id);
			config_setting_lookup_string(mqtt_read, "sensor", &sensor);
			memcpy(mqtt_chld->sensor_name, sensor, SIZE_STRING);
			if (parent) {
				parent->children = mqtt_chld;
				parent = parent->children;
			} else {
				parent = mqtt_chld;
			}
			if (!ms->children) {
				ms->children = parent;
			}
			params_t *params = NULL;
			char *param = malloc(64);
			rc = 1, j = 1;
			while (rc) {
				const char *new_param;
				sprintf(param, "param%d", j++);
				rc = config_setting_lookup_string(mqtt_read, param, &new_param);
				if (rc) {
					params_t *p = malloc(sizeof(params_t));
					p->param = malloc(SIZE_STRING);
					memcpy(p->param, new_param, SIZE_STRING);
					p->next = NULL;

					if (params) {
						params->next = p;
						params = params->next;
					} else {
						params = p;
					}

					if (!parent->params)
						parent->params = p;
				}
			}
		}
	}

	config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
	return (EXIT_SUCCESS);
}

/* This example constructs a new configuration in memory and writes it to
 * 'newconfig.cfg'.
 */

int newconfig(void) {
	static const char *output_file = "newconfig.cfg";
	config_t cfg;
	config_setting_t *root, *setting, *group, *array;
	int i;

	config_init(&cfg);
	root = config_root_setting(&cfg);

	/* Add some settings to the configuration. */
	group = config_setting_add(root, "address", CONFIG_TYPE_GROUP);

	setting = config_setting_add(group, "street", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "1 Woz Way");

	setting = config_setting_add(group, "city", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "San Jose");

	setting = config_setting_add(group, "state", CONFIG_TYPE_STRING);
	config_setting_set_string(setting, "CA");

	setting = config_setting_add(group, "zip", CONFIG_TYPE_INT);
	config_setting_set_int(setting, 95110);

	array = config_setting_add(root, "numbers", CONFIG_TYPE_ARRAY);

	for (i = 0; i < 10; ++i) {
		setting = config_setting_add(array, NULL, CONFIG_TYPE_INT);
		config_setting_set_int(setting, 10 * i);
	}

	/* Write out the new configuration. */
	if (!config_write_file(&cfg, output_file)) {
		fprintf(stderr, "Error while writing file.\n");
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	fprintf(stderr, "New configuration successfully written to: %s\n", output_file);

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}
