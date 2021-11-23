/*
  needs libmosquitto-dev
  $ gcc -o libmosq libmosq.c -lmosquitto
*/
#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mhz19c.h>
#include <mqtt.h>

int fd;
struct mosquitto *mosq = NULL;
void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
	printf("connect callback, rc=%d\n", result);
}

void message_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
	bool match = 0;
	int num=0;
	printf("got message '%.*s' for topic '%s'\n", message->payloadlen, (char*) message->payload, message->topic);

	mosquitto_topic_matches_sub("uart1/MHZ19C/setCalibrateSpan", message->topic, &match );
	if (match) {
	    num = atoi(message->payload);
	    calibrateSpan(fd,num);
	}

	mosquitto_topic_matches_sub("uart1/MHZ19C/setCalibrate", message->topic, &match );
	if (match) {
	    num = atoi(message->payload);
	    cntrlCalibrate(fd, ( eMhz19_calibrate )num );
	}

}

void mosq_log_callback(struct mosquitto *mosq, void *userdata, int level, const char *str)
{
	/* Pring all log messages regardless of level. */


  switch(level){
    //case MOSQ_LOG_DEBUG:
    //case MOSQ_LOG_INFO:
    //case MOSQ_LOG_NOTICE:
    case MOSQ_LOG_WARNING:
    case MOSQ_LOG_ERR: {
      printf("%i:%s\n", level, str);
    }
  }


}

void mqtt_setup(mqtt_setting_t *ms){
  char *host = ms->host;
  int port = ms->port;
  int keepalive = ms->keepalive;
  bool clean_session = true;

  mosquitto_lib_init();
  mosq = mosquitto_new(NULL, clean_session, NULL);
  if(!mosq){
		fprintf(stderr, "Error: Out of memory.\n");
		exit(1);
	}
  else{
	mosquitto_connect_callback_set(mosq, connect_callback);
	mosquitto_message_callback_set(mosq, message_callback);
  }

  mosquitto_log_callback_set(mosq, mosq_log_callback);

  if(mosquitto_connect(mosq, host, port, keepalive)){
		fprintf(stderr, "Unable to connect.\n");
		exit(1);
	}
  int loop = mosquitto_loop_start(mosq);
  if(loop != MOSQ_ERR_SUCCESS){
    fprintf(stderr, "Unable to start loop: %i\n", loop);
    exit(1);
  }
}

int mqtt_sub( char *topic ) {
    return mosquitto_subscribe(mosq, NULL,topic, 0);
}

int mqtt_send(char *msg , char *topic){
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}
