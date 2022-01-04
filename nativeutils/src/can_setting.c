/*
 * can.c
 *
 *  Created on: 5 дек. 2021 г.
 *      Author: root
 */
#include <common.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <can_setting.h>

static int fd_can = -1;

static int getDescriptor(void) {
	return fd_can;
}

static void closeCan(void) {
	if (fd_can > 0)
		close(fd_can);
}

static int openDevCan(char *device) {
	struct sockaddr_can addr;
	struct ifreq ifr;

	if (fd_can > 0) {
		close(fd_can);
		fd_can = -1;
	}
	/* create a socket */
	fd_can = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	strcpy(ifr.ifr_name, device);
	/* determine the interface index */
	ioctl(fd_can, SIOCGIFINDEX, &ifr);
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	/* bind the socket to a CAN interface */
	bind(fd_can, (struct sockaddr*) &addr, sizeof(addr));

	if (-1 == fd_can) {
		LOG_ERROR("Can't Open Serial Port");
		return -1;
	} else
		return 0;
}


static int confCan(init_conf_t *conf) {
	can_setting_t *cs = conf->dev_sett;
	int id_flag =0;

	struct can_filter rfilter[1];
	openDevCan(cs->device);

	if (fd_can > 0) {
		if (id_flag) {
			/* define the filter rules */
			rfilter[0].can_id = 0;
			rfilter[0].can_mask = CAN_SFF_MASK;
			/* Set the filter rules */
			setsockopt(fd_can, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
		}
	} else {
		LOG_ERROR( "Error opening %s", cs->device);
		exit(1);
	}

	return SUCCESS;
}

static int defaultConfCan(char *device) {
	init_conf_t conf;
	can_setting_t cs;
	cs.device = malloc(strlen(DEFAULT_DEVICE_CAN) + 1);
	memcpy(cs.device, DEFAULT_DEVICE_CAN, strlen(DEFAULT_DEVICE_CAN) + 1);
	conf.dev_sett = &cs;
	return confCan(&conf);
}

static int autoConfCan(int numInstance) {
	init_conf_t conf;
	can_setting_t cs;

	if (!readConfCAN(&cs, numInstance)) {
		conf.dev_sett = &cs;
		return confCan(&conf);
	} else {
		return defaultConfCan(NULL);
	}
}

int getFncCan(devFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getDescriptor = getDescriptor;
	cfgFuncs->openDev = openDevCan;
	cfgFuncs->closeDev = closeCan;
	cfgFuncs->autoConfDev = autoConfCan;
	cfgFuncs->confDev = confCan;
	cfgFuncs->fd_dev = fd_can;
	return SUCCESS;
}
