/*
 ============================================================================
 Name        : Mqtt_Mh-Z19C.c
 Author      : lns
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <mhz19c.h>
#include <mqtt.h>
#include <uart_setting.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>


#define ERROR_CREATE_THREAD -11
#define ERROR_JOIN_THREAD   -12
#define SUCCESS        0

int fd;

void* task_read_z19c(void *param) {
  /*  int *thr = (int*)param;*/

  measurement_t measurement;
  mqtt_setting_t ms;
  uart_setting_t us;


  if(!readConfUart(&us,0)){ /* настройки порта из конфиг файла*/
      fd = confUart(us.device, us.speed, us.databits, us.stopbits);
  }
  else{/* настройки порта по умолчанию если нет в конфиг файле*/
      fd = defaultConfUart(NULL);
  }

  mqtt_setup(&ms);

	while (1) {
	    measurement = getMeasurement (fd);

	    mqtt_gen_topic_and_pub_int(ms.topic , "co2_ppm" , measurement.co2_ppm);
	    mqtt_gen_topic_and_pub_int(ms.topic , "temperature" , measurement.temperature);
 	    printf ("co2_ppm:%d temperature:%d state:%d\n", measurement.co2_ppm, measurement.temperature, measurement.state);
 	    usleep(1000);
	}
	close(fd);
	pthread_exit(0);
	return 0;
}

void terminate (int param)
{
  puts( "Завершение программы...");
  close (fd);                             // удалить временный файл
  exit(1);                               // завершить работу программы со значением 1
}

int main (int argc, char *argv[])
{
  void (*funcptr)(int);                              // указатель на функцию

  funcptr = signal (SIGTERM,terminate);              // обработка сигнала
  funcptr = signal (SIGINT,terminate);              // обработка сигнала
/*  if (funcptr == SIGINT) signal(SIGTERM,SIGINT);*/  // в случае, если funcptr будет указывать на SIG_IGN, то сигнал SIGTERM будет игнорироваться


  pthread_t thr;
  int status;
  status = pthread_create(&thr, NULL, task_read_z19c, &thr);
  if (status != 0) {
      printf("main error: can't create thread, status = %d\n", status);
      exit(ERROR_CREATE_THREAD);
  }

  status = pthread_join(thr, 0);
  if (status != SUCCESS) {
      printf("main error: can't join thread, status = %d\n", status);
      exit(ERROR_JOIN_THREAD);
  }
  exit (0);
}
