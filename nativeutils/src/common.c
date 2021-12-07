/*
 * common.c
 *
 *  Created on: 26 нояб. 2021 г.
 *      Author: root
 */

#include <stdio.h>
#include <errno.h>
#include <common.h>
#include <stdlib.h>

int init(int *id, usr_cfg_t *uc, devSensorFunc_t *dSf, eRead_configure block) {
/*	char **params = NULL;*/

	init_conf_t ic;
	read_usr_configure(uc, *id, block);
/*	get_usr_param_cnf(uc->mqtt_read, params);*/

	ic.id = *id;
	ic.dev_sett = uc->dev_cfg;
	ic.dev_func = dSf;

#ifdef ARM
	if (dSf->init(&ic) != 0) {
		pthread_exit(SUCCESS);
		exit(SUCCESS);
	}
#else
#endif
	uc->mqtt_general->fun_mess_clb = dSf->mqtt_clb;
	mqtt_setup(uc->mqtt_general);
	dSf->mqtt_init_sub(uc->mqtt_general->topic);
	dSf->setPollingTime(uc->mqtt_read->polling_time);
	return SUCCESS;
}

int addr_tochar(char *out, int addr) {
	memset(out, 0, MAX_BUF);
	sprintf(out, "0-00%x", addr);
	return SUCCESS;
}
int generate_root_i2c_string(char *out, char *root, char *device, char *addr, char *prefix) {
	memset(out, 0, MAX_BUF);
	sprintf(out, "%s/%s/device/%s/%s", root, device, addr, prefix);
	return SUCCESS;
}

int generate_root_ow1_string(char *out, char *root, char *addr) {
	memset(out, 0, MAX_BUF);
	sprintf(out, "%s/%s", root, addr);
	return SUCCESS;
}

float get_setting_float(char *ifname, char *param) {
	FILE *fp;
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	char path[MAX_BUF];
	memset(path, 0, MAX_BUF);
	/*	int i = strlen(ifname);*/
	sprintf(path, "%s/%s", ifname, param);

	if ((fp = fopen(path, "r")) == NULL) {
		printf("cannot open device file\n");
		return 1;
	}
	if ((fgets(buf, MAX_BUF, fp)) == NULL) {
		printf("cannot read device\n");
	}
	fclose(fp);

	return atoi(buf);
}

int get_setting_str(char *out, char *ifname, char *param) {
	FILE *fp;
	char path[MAX_BUF];
	memset(path, 0, MAX_BUF);

	sprintf(path, "%s/%s", ifname, param);

	if ((fp = fopen(path, "r")) == NULL) {
		printf("cannot open device file\n");
		return 1;
	}
	if ((fgets(out, MAX_BUF, fp)) == NULL) {
		printf("cannot read device\n");
		return 1;
	}
	fclose(fp);

	return SUCCESS;
}

int get_setting_int(char *ifname, char *param) {
	FILE *fp;
	char buf[MAX_BUF];
	memset(buf, 0, MAX_BUF);
	char path[MAX_BUF];
	memset(path, 0, MAX_BUF);

	sprintf(path, "%s/%s", ifname, param);

	if ((fp = fopen(path, "r")) == NULL) {
		printf("cannot open device file\n");
		return 1;
	}
	if ((fgets(buf, MAX_BUF, fp)) == NULL) {
		printf("cannot read device\n");
	}
	fclose(fp);

	return atoi(buf);
}
