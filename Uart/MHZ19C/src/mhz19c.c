/*
 * MHZ19C.c
 *
 *  Created on: 14 нояб. 2021 г.
 *      Author: root
 */
#include <mhz19c.h>
#include <uart_setting.h>


uint8_t getppm[] 	= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t zerocalib[]     = {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t spancalib[]     = {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t autocalib_on[]  = {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
uint8_t autocalib_off[] = {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};

uint8_t mhz19_checksum(uint8_t com[])
{
	uint8_t sum = 0x00;
	for (int i = 1; i < REQUEST_CNT; i++)
	{
		sum += com[i];
	}
	sum = 0xff - sum + 0x01;
	return sum;
}

/* undocumented function */
int getStatus(int fd)
{
	measurement_t m = getMeasurement(fd);
	return m.state;
}

void setAutoCalibration(int fd, bool autocalib)
{
	writeCommand(fd,autocalib ? autocalib_on : autocalib_off,NULL);
}

void calibrateZero(int fd )
{
	writeCommand(fd,zerocalib, NULL);
}

measurement_t getMeasurement(int fd)
{
	uint8_t response[RESPONSE_CNT];
	memset(response, 0, RESPONSE_CNT);

	writeCommand(fd ,getppm, response);


	// parse
	measurement_t measurement = {};
	if (response[0] == 0xff && response[1] == 0x86 && mhz19_checksum(response) == response[RESPONSE_CNT -1])
	{
		measurement.co2_ppm = response[2] * 256 + response[3];
		measurement.temperature = response[4] - 40;
		measurement.state = response[5];
	}
	else
	{
		measurement.co2_ppm = measurement.temperature = measurement.state = -1;
	}
	return measurement;
}

void calibrateSpan(int fd,int ppm)
{
	if (ppm < 1000)
		return;

	uint8_t cmd[REQUEST_CNT];
	for (int i = 0; i < REQUEST_CNT; i++)
	{
		cmd[i] = spancalib[i];
	}
	cmd[3] = (uint8_t)(ppm / 256);
	cmd[4] = (uint8_t)(ppm % 256);
	writeCommand(fd,cmd, NULL);
}

void writeCommand(int fd,uint8_t cmd[], uint8_t *response )
{
  printf("Start measurement \n");
	fd_set set;
        struct timeval tv;
	char *device;
	int nread = 0;

	uint8_t buff[RESPONSE_CNT];
	memcpy(buff,cmd,8);
	buff[8] = mhz19_checksum(cmd);

	write(fd, buff, RESPONSE_CNT);
	memset(response , 0,RESPONSE_CNT);
	if (response != NULL)
	{
		   do{
			FD_ZERO(&set);
			FD_SET(fd, &set);
			tv.tv_sec = 1;
		        tv.tv_usec = 0;
			    if (select(fd + 1, &set, NULL, NULL, &tv)){/*ready read data*/
				if (FD_ISSET(fd, &set)) {
				    nread += read(fd, response+nread, sizeof(RESPONSE_CNT));
				}
			    }
			    else{ /*timeout*/
				break;
			    }

		}while( nread < 9 );
	}
}
