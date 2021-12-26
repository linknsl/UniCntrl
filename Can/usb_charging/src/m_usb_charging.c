/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <can_setting.h>
#include <usb_charging.h>
#include <common.h>

/*"usb_current[3]","usb_voltage[3]" */
#define PARAM_FLOAT_USB_CHARGING_SIZE 6
/* "soft_id","hard_id","uniq_id","usb1_mistake","usb2_mistake","usb3_mistake","total_mistake","temperature"*/
#define PARAM_INT_USB_CHARGING_SIZE 8

void* read_sensor(void *param) {
	int *value_array_int = malloc((PARAM_INT_USB_CHARGING_SIZE) * sizeof(int));
	float *value_array_float = malloc((PARAM_FLOAT_USB_CHARGING_SIZE) * sizeof(float));
	usr_cfg_t ucfg;
	devSensorFunc_t dSf;
	devFunc_t df_can;
	init_conf_t ic;

	getFncCan(&df_can);
	dSf.devFunc = &df_can;

	getSensorFncUsbCharging(&dSf);
	init(param, &ucfg, &dSf, &ic, CANS);

	ucfg.mqtt_read->param_add.start = 0;
	ucfg.mqtt_read->param_float.start = 3;
	ucfg.mqtt_read->param_int.start = 10;

	while (1) {
		if (dSf.getMeasurement(value_array_int, &ic) == SUCCESS)
			mqttResultPubInt(&ucfg, value_array_int);

		if (dSf.getMeasurementFloat(value_array_float, &ic) == SUCCESS)
			mqttResultPubFloat(&ucfg, value_array_float);
		usleep(100);
	}
	pthread_exit(SUCCESS);
	return SUCCESS;
}

int main(int argc, const char *argv[]) {

	if (osStartEngine(argc, argv, read_sensor) != SUCCESS) {
		exit(FAILURE);
	} else {
		exit(SUCCESS);
	}
}
