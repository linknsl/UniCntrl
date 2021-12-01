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

	   char cwd[SIZE_STRING];
	   if (getcwd(cwd, sizeof(cwd)) != NULL) {
	       printf("Current working dir: %s\n", cwd);
	   } else {
	       perror("getcwd() error");
	       return 1;
	   }

	us->device = malloc(SIZE_STRING);
	us->name = malloc(SIZE_STRING);

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

int readConfMqtt(mqtt_setting_t *ms, int numInstance) {

	ms->topic = malloc(SIZE_LONG_STRING);
	ms->host = malloc(SIZE_STRING);
	ms->name = malloc(SIZE_STRING);

	config_t cfg;
	config_setting_t *setting;

	config_init(&cfg);

	if (!config_read_file(&cfg, "unicntrl.cfg")) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, "application.uarts.mqtt");
	if (setting != NULL) {
		const char *topic = NULL, *name = NULL, *host =NULL;
		config_setting_t *mqtt_setting = config_setting_get_elem(setting, numInstance);

		if ((config_setting_lookup_string(mqtt_setting, "topic", &topic)
				&& config_setting_lookup_string(mqtt_setting, "name", &name)
				&& config_setting_lookup_string(mqtt_setting, "host", &host)
				&& config_setting_lookup_int(mqtt_setting, "port", &ms->port)
				&& config_setting_lookup_int(mqtt_setting, "keepalive", &ms->keepalive)
				&& config_setting_lookup_int(mqtt_setting, "polling_time", &ms->polling_time)
				&& config_setting_lookup_int(mqtt_setting, "number", &ms->number)
				&& config_setting_lookup_int(mqtt_setting, "log", &ms->log))) {
			memcpy(ms->topic, topic, strlen(topic) + 1);
			memcpy(ms->name, name, strlen(name) + 1);
			memcpy(ms->host, host, strlen(host) + 1);
		}

	}

	config_destroy(&cfg);
	return (EXIT_SUCCESS);
}

/*int readConfMqtt_new(mqtt_config_t *ms) {
 int i, j = 1, rc, count;
 ms->server = malloc(SIZE_LONG_STRING);

 config_t cfg;
 config_setting_t *setting = NULL;

 config_init(&cfg);

 if (!config_read_file(&cfg, "unicntrl.cfg")) {
 fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
 config_destroy(&cfg);
 return (EXIT_FAILURE);
 }

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

 config_destroy(&cfg);
 return (EXIT_SUCCESS);
 }*/

int readcnfMqttGnrl(mqtt_config_t *ms, int version) {
	ms->server = malloc(SIZE_LONG_STRING);

	config_t cfg;
	config_setting_t *setting = NULL;

	config_init(&cfg);

	if (!config_read_file(&cfg, NAME_CONF_FILE)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);
		return (EXIT_FAILURE);
	}

	setting = config_lookup(&cfg, CONF_MQTT);
	if (setting != NULL) {

		const char *server =NULL;
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

int readcnfMqttRead(mqtt_config_read_t *ms, int mqtt_id) {
	int j = 0, rc;
	ms->sensor_name = malloc(SIZE_LONG_STRING);

	config_t cfg;
	config_setting_t *setting = NULL;

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
		config_setting_lookup_int(mqtt_read, "id", &ms->id);
		config_setting_lookup_int(mqtt_read, "polling_time", &ms->polling_time);
		config_setting_lookup_string(mqtt_read, "sensor", &sensor);
		memcpy(ms->sensor_name, sensor, strlen(sensor) + 1);

		params_t *params = NULL;
		char *param = malloc(64);
		rc = 1, j = 1;
		while (rc) {
			const char *new_param = NULL;
			sprintf(param, "param%d", j++);
			rc = config_setting_lookup_string(mqtt_read, param, &new_param);
			if (rc) {
				params_t *p = malloc(sizeof(params_t));
				p->param = malloc(strlen(new_param)+1);
				memcpy(p->param, new_param, strlen(new_param) + 1);
				p->next = NULL;

				if (params) {
					params->next = p;
					params = params->next;
				} else {
					params = p;
				}

				if (!ms->params)
					ms->params = p;
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

	uart_setting_t us;
	ms->dev_cfg = &us;
	readConfUart(&us, numInstance);

	ms->mqtt_general = malloc(sizeof(mqtt_config_t));
	ms->mqtt_general->topic = malloc(sizeof(SIZE_LONG_STRING));
	readcnfMqttGnrl(ms->mqtt_general, VERSION_MQTT_GNRL);

	ms->mqtt_read = malloc(sizeof(mqtt_config_read_t));
	readcnfMqttRead(ms->mqtt_read, us.mqtt_id);

	sprintf(ms->mqtt_general->topic, "%d/%d/%s/%s", ms->mqtt_general->number_board, numInstance, us.name,
			ms->mqtt_read->sensor_name);

	return 0;
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
