/*
 * uart_setting.c
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <uart_setting.h>
#include <common.h>

static int fd_uart = -1;
static int cnt_wr_timeout = 0;

static int speed_arr[] =
		{ B921600, B460800, B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
static int name_arr[] = { 921600, 460800, 230400, 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

static int autoConfUart(int numInstance);
static int confUart(init_conf_t *conf);

static int getDescriptor(void) {
	return fd_uart;
}

static void Reconnect(bool status) {
	if (status) {
		cnt_wr_timeout = 0;
	} else {
		cnt_wr_timeout++;
	}
	if (cnt_wr_timeout > 5) {
		cnt_wr_timeout = 0;
#ifdef ARM
		autoConfUart(0);
#else
		fprintf(stderr, "Reconect \n");
#endif
	}
}

static void closeUart(void) {
	if (fd_uart > 0)
		close(fd_uart);
}

static int set_speed(int speed) {
	int i;
	int status;
	struct termios Opt;
	tcgetattr(fd_uart, &Opt);

	for (i = 0; i < sizeof(speed_arr) / sizeof(int); i++) {
		if (speed == name_arr[i]) {
			tcflush(fd_uart, TCIOFLUSH);
			cfsetispeed(&Opt, speed_arr[i]);
			cfsetospeed(&Opt, speed_arr[i]);
			status = tcsetattr(fd_uart, TCSANOW, &Opt);
			if (status != 0)
				perror("tcsetattr fd_uart1");
			return FAILURE;
		}
		tcflush(fd_uart, TCIOFLUSH);
	}

	if (i == 12) {
		fprintf(stderr, "\tSorry, please set the correct baud rate!\n\n");
	}
	return SUCCESS;
}

static bool set_Parity(int databits, int stopbits, int parity) {
	struct termios options;
	if (tcgetattr(fd_uart, &options) != 0) {
		perror("SetupSerial 1");
		return (false);
	}
	options.c_cflag &= ~CSIZE;
	switch (databits) {
	case 7:
		options.c_cflag |= CS7;
		break;
	case 8:
		options.c_cflag |= CS8;
		break;
	default:
		fprintf(stderr, "Unsupported data size\n");
		return (false);
	}

	switch (parity) {
	case 'n':
	case 'N':
		options.c_cflag &= ~PARENB; /* Clear parity enable */
		options.c_iflag &= ~INPCK; /* Enable parity checking */
		break;
	case 'o':
	case 'O':
		options.c_cflag |= (PARODD | PARENB);
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'e':
	case 'E':
		options.c_cflag |= PARENB; /* Enable parity */
		options.c_cflag &= ~PARODD;
		options.c_iflag |= INPCK; /* Disnable parity checking */
		break;
	case 'S':
	case 's': /*as no parity*/
		options.c_cflag &= ~PARENB;
		options.c_cflag &= ~CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported parity\n");
		return (false);
	}
	switch (stopbits) {
	case 1:
		options.c_cflag &= ~CSTOPB;
		break;
	case 2:
		options.c_cflag |= CSTOPB;
		break;
	default:
		fprintf(stderr, "Unsupported stop bits\n");
		return (false);
	}
	/* Set input parity option */
	if (parity != 'n')
		options.c_iflag |= INPCK;
	options.c_cc[VTIME] = 150; // 15 seconds
	options.c_cc[VMIN] = 0;

	options.c_lflag &= ~(ECHO | ICANON);

	tcflush(fd_uart, TCIFLUSH); /* Update the options and do it NOW */
	if (tcsetattr(fd_uart, TCSANOW, &options) != 0) {
		perror("SetupSerial 3");
		return (false);
	}
	return (true);
}

static int openDev(char *Dev) {
	if (fd_uart > 0) {
		fprintf(stderr, "Reconect... Old descr %d \n", fd_uart);
		close(fd_uart);
		fd_uart = -1;
	}
	fd_uart = open(Dev, O_RDWR);
	fprintf(stderr, "Connect... New descr %d \n", fd_uart);
	if (-1 == fd_uart) {
		perror("Can't Open Serial Port");
		return -1;
	} else
		return 0;
}

static int confUart(init_conf_t *conf) {
	uart_setting_t *us = conf->dev_sett;

	openDev(us->device);

	if (fd_uart > 0) {
		set_speed(us->speed);
	} else {
		fprintf(stderr, "Error opening %s: %s\n", us->device, strerror(errno));
		exit(1);
	}

	if (set_Parity(us->databits, us->stopbits, 'N') == false) {
		fprintf(stderr, "Set Parity Error\n");
		close(fd_uart);
		exit(1);
	}
	return SUCCESS;
}

static int defaultConfUart(char *device) {
	init_conf_t conf;
	uart_setting_t us;
	us.speed = 9600;
	us.databits = 8;
	us.stopbits = 1;
	us.device = malloc(strlen(DEFAULT_DEVICE) + 1);
	memcpy(&us.device, DEFAULT_DEVICE, strlen(DEFAULT_DEVICE) + 1);
	conf.dev_sett = &us;
	return confUart(&conf);
}

static int autoConfUart(int numInstance) {
	init_conf_t conf;
	uart_setting_t us;

	if (!readConfUart(&us, numInstance)) {
		conf.dev_sett = &us;
		return confUart(&conf);
	} else {
		return defaultConfUart(NULL);
	}

}

int getFncUart(devFunc_t *cfgFuncs) {
	if (cfgFuncs == NULL) {
		return -1;
	}
	cfgFuncs->getDescriptor = getDescriptor;
	cfgFuncs->openDev = openDev;
	cfgFuncs->closeDev = closeUart;
	cfgFuncs->autoConfDev = autoConfUart;
	cfgFuncs->recconect = Reconnect;
	cfgFuncs->confDev = confUart;
	cfgFuncs->fd_dev = fd_uart;
	return SUCCESS;
}

