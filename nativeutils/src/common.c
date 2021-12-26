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

static enum {
	E_STATE_UNINITIALIZED, E_STATE_INITIALIZED, E_STATE_WAITING, E_STATE_STOP
} programState = E_STATE_UNINITIALIZED;
static pthread_t rootTask;

static char prefix_dev[32];

void init_dir_tele(void) {
	mkdir(PATH_LOG, S_IRWXU | S_IRWXG | S_IRWXO);
}

void init_log(const char *name, int id) {
	init_dir_tele();
	sprintf(prefix_dev, "%s/%d", name, id);
}

static char* get_prefix(void) {
	return prefix_dev;
}

void terminate(int param) {
	exit(FAILURE);
}

int osStartEngine(int argc, const char *argv[], void *entryPoint) {
	int id;

	setbuf(stdout, NULL);
	setbuf(stderr, NULL);

	rootTask = pthread_self();
	programState = E_STATE_INITIALIZED;
	signal(SIGTERM, terminate);
	signal(SIGINT, terminate);

	if (entryPoint) {
		init_log(argv[0], atoi(argv[1]));
		if (argc < 2) {
			LOG_ERROR("You need input id aplication.");
			exit(1);
		}
		id = atoi(argv[1]);
		LOG("Start aplication id %d ", id);

		pthread_t thr;
		int status;
		status = pthread_create(&thr, NULL, entryPoint, &id);
		if (status != 0) {
			LOG_ERROR("main error: can't create thread, status = %d", status);
			exit(ERROR_CREATE_THREAD);
		}

		status = pthread_join(thr, 0);
		if (status != SUCCESS) {
			LOG_ERROR("main error: can't join thread, status = %d", status);
			exit(ERROR_JOIN_THREAD);
		}
		exit(SUCCESS);
	}

	programState = E_STATE_WAITING;
	while (programState != E_STATE_STOP) {
		pause();
	}

	return SUCCESS;
}

int init(int *id, usr_cfg_t *uc, devSensorFunc_t *dSf, init_conf_t *ic, eRead_configure block) {
	char **params = NULL;

	read_usr_configure(uc, *id, block);
	get_usr_param_cnf(uc->mqtt_read, params);

	ic->id = *id;
	ic->dev_sett = uc->dev_cfg;
	ic->dev_func = dSf;

#ifdef ARM
	if (dSf->init(ic) != 0) {
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
		LOG_ERROR("cannot open device file");
		return 1;
	}
	if ((fgets(buf, MAX_BUF, fp)) == NULL) {
		LOG_ERROR("cannot read device");
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
		LOG_ERROR("cannot open device file");
		return 1;
	}
	if ((fgets(out, MAX_BUF, fp)) == NULL) {
		LOG_ERROR("cannot read device");
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
		LOG_ERROR("cannot open device file");
		return 1;
	}
	if ((fgets(buf, MAX_BUF, fp)) == NULL) {
		LOG_ERROR("cannot read device");
	}
	fclose(fp);

	return atoi(buf);
}

void save_log_str(char *name, char *val) {
	char path[32];
	strcpy(path, PATH_LOG);
	strcat(path, name);
	FILE *fp = fopen(path, "a+");
	fprintf(fp, "%s %s", get_prefix(), val);
	fclose(fp);
}

char* sprintts(struct timeval *tv) {
	static char res[400];
	time_t time;
	struct timeval tv_now;
	struct tm *tm;

	if (!tv) {
		gettimeofday(&tv_now, 0);
		tv = &tv_now;
	}

	time = tv->tv_sec;
	tm = localtime(&time);

	strftime(res, sizeof(res), "%Y-%m-%d %H:%M:%S", tm);
	snprintf(res + strlen(res), sizeof(res), ".%06d", (int) (tv->tv_usec));

	return res;
}
