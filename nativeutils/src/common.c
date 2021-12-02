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
