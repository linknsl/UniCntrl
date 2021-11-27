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
	char path[MAX_BUF];

	snprintf(path, sizeof path, "%s/%s", ifname, param);
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
