/*
 * read_conf.c
 *
 *  Created on: 21 нояб. 2021 г.
 *      Author: root
 */
#include <libconfig.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <read_confuguration_file.h>

int readConfUart( uart_setting_t *us , int numInstance)
{

  /* используются свои типы. */
  config_t cfg;
  config_setting_t *setting;

  config_init(&cfg); /* обязательная инициализация */

  /* Читаем файл. Если ошибка, то завершаем работу */
  if(! config_read_file(&cfg, "unicntrl.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  /* Вывод списка книжек с полок. */
  setting = config_lookup(&cfg, "application.uarts.setting");
  if(setting != NULL)
  {
      config_setting_t *uart_setting = config_setting_get_elem(setting, numInstance);
      /* Выводим только те записи, если они имеют все нужные поля. */
      const char *device;

      if((config_setting_lookup_string(uart_setting, "device", &device)
           && config_setting_lookup_int(uart_setting, "speed", &us->speed)
           && config_setting_lookup_int(uart_setting, "databits", &us->databits)
           && config_setting_lookup_int(uart_setting, "stopbits", &us->stopbits)
           )){
      memcpy(us->device,device, SIZE_STRING);
      }
  }

  config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
  return(EXIT_SUCCESS);
}

int readConfMqtt( mqtt_setting_t *ms , int numInstance)
{

  /* используются свои типы. */
  config_t cfg;
  config_setting_t *setting;

  config_init(&cfg); /* обязательная инициализация */

  /* Читаем файл. Если ошибка, то завершаем работу */
  if(! config_read_file(&cfg, "unicntrl.cfg"))
  {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
            config_error_line(&cfg), config_error_text(&cfg));
    config_destroy(&cfg);
    return(EXIT_FAILURE);
  }

  /* Вывод списка книжек с полок. */
  setting = config_lookup(&cfg, "application.uarts.mqtt");
  if(setting != NULL)
  {
      const char *topic, *name;
      config_setting_t *mqtt_setting = config_setting_get_elem(setting, numInstance);

      if(!(config_setting_lookup_string(mqtt_setting, "topic", &topic)
	   &&(config_setting_lookup_string(mqtt_setting, "name", &name)
           && config_setting_lookup_int(mqtt_setting, "polling_time", &ms->polling_time)
           && config_setting_lookup_int(mqtt_setting, "number", &ms->number)
           ))){
	      memcpy(ms->topic,topic, SIZE_LONG_STRING);
	      memcpy(ms->name,name, SIZE_STRING);
      }

  }

  config_destroy(&cfg); /* Освободить память обязательно, если это не конец программы */
  return(EXIT_SUCCESS);
}
