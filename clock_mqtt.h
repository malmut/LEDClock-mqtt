/*
 *   Clock with WS8212 60 LED Ring
 *  
 *   MAWISoft 2020
 *   
 *   clock mqtt functions and defines
 * 
*/

#ifndef _clock_mqtt_h_
#define _clock_mqtt_h_

#include "credentials.h"
#include <PubSubClient.h>
PubSubClient mqttClient(wifiClient);
#define DEVICENAME "LEDClock"
#define CMND_TOPIC "cmnd/ledclock/#"
#define CMND_TOPIC_POWER "cmnd/ledclock/power"
#define CMND_TOPIC_MODE "cmnd/ledclock/mode"
#define CMND_TOPIC_SCHEME "cmnd/ledclock/scheme"
#define CMND_TOPIC_HELP "cmnd/ledclock/help"
#define TELE_TOPIC_STATUS "tele/ledclock/status/"
#define TELE_TOPIC_STATUS_MODE "tele/ledclock/status/mode"
#define TELE_TOPIC_STATUS_SCHEME "tele/ledclock/status/scheme"
#define TELE_TOPIC_STATUS_USAGE "tele/ledclock/status/usage"

void callbackMQTT(char* topic, byte* payload, unsigned int length);
void reconnectMQTT();
void publishState(); 

#endif
