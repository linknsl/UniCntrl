/*
 * uart_setting.c
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <uart_setting.h>
#include <common.h>

int fd_uart = -1;
int cnt_wr_timeout = 0;

int getDescriptor(void) {
	return fd_uart;
}

void Reconnect(bool status) {
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
		printf("Reconect \n");
#endif
	}
}

void closeUart(void) {
	if (fd_uart > 0)
		close(fd_uart);
}

int speed_arr[] = { B921600, B460800, B230400, B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = { 921600, 460800, 230400, 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

int set_speed(int speed) {
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
		printf("\tSorry, please set the correct baud rate!\n\n");
	}
	return SUCCESS;
}

bool set_Parity(int databits, int stopbits, int parity) {
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

int openDev(char *Dev) {
	fd_uart = open(Dev, O_RDWR);         //| O_NOCTTY | O_NDELAY
	if (-1 == fd_uart) {
		perror("Can't Open Serial Port");
		return -1;
	} else
		return 0;
}

int defaultConfUart(char *device) {
	return confUart(DEFAULT_DEVICE, 9600, 8, 1);
}

int autoConfUart(int numInstance) {

	uart_setting_t us;

	if (!readConfUart(&us, numInstance)) {
		return confUart(us.device, us.speed, us.databits, us.stopbits);
	} else {
		return defaultConfUart(NULL);
	}
}

int confUart(char *device, int speed, int databits, int stopbits) {

	openDev(device);

	if (fd_uart > 0) {
		set_speed(9600);
	} else {
		fprintf(stderr, "Error opening %s: %s\n", device, strerror(errno));
		exit(1);
	}

	if (set_Parity(databits, stopbits, 'N') == false) {
		fprintf(stderr, "Set Parity Error\n");
		close(fd_uart);
		exit(1);
	}
	return SUCCESS;
}

