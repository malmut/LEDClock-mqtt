/*
 *   Clock with WS8212 60 LED Ring
 *  
 *   MAWISoft 2020
 *   
 *   mqtt functions
 * 
*/

#include "clock_mqtt.h"

void mqttCallback(char* topic,byte* payload, unsigned int length) {

  payload[length] = '\0';
  String strValue = String((char*)payload);
  int intValue = strValue.toInt();
  char msg[60];
  sprintf(msg,"Received topic [%s] with payload [%s]", topic, payload);
#ifdef DEBUG  
  Serial.println(msg);
#endif
 
  if(strcmp(topic, CMND_TOPIC_MODE) == 0) {   
      if (intValue == CLASSIC) {
          mode = CLASSIC;
#ifdef DEBUG
          Serial.println("Clock mode CLASSIC");
#endif
       }
       
      else if (intValue == MINARC) {
          mode = MINARC;
#ifdef DEBUG
          Serial.println("Clock mode MINARC");
#endif
       }
       
      else if (intValue == ARC) {
          mode = ARC;
#ifdef DEBUG
          Serial.println("Clock mode ARC");
#endif
      }
       
      else if (intValue == RAINBOW) {
          // show some fancy
          rainbowCycle(20);
          // wait 1 second to settle
          //delay(1000);
          blackWipe(6);      
      }

      else if (intValue == UPDATE) {
      if (ntpClient.update()) {                             // according to NTP server
          Rtc.adjust(ntpClient.getEpochTime());
#ifdef DEBUG
          Serial.println("Clock updated through NTP.");
#endif
          colorWipe(RGB(0,200,0), 1);                        // give the user some feedback in green
          blackWipe(1);
       }  
     }
   }
   
  else if(strcmp(topic, CMND_TOPIC_POWER) == 0) {
       if (intValue == 0) {
            SHUTOFF
            state = INSTANTOFF;
#ifdef DEBUG
            Serial.println("Power off.");
#endif
       }
       else {
            state = ON;
#ifdef DEBUG
            Serial.println("Power on");
#endif
       }
  }

  else if(strcmp(topic, CMND_TOPIC_HELP) == 0) {
          mqttClient.publish(TELE_TOPIC_STATUS_USAGE,"-t power -m 0:instant off 1:toggle"); 
          mqttClient.publish(TELE_TOPIC_STATUS_USAGE,"-t scheme -m 194:crimson 195:day 196:violet 197:spring 198:night 199:aqua"); 
          mqttClient.publish(TELE_TOPIC_STATUS_USAGE,"-t mode -m 164:classic 165:minarc 166:arc 167:rainbow 168:update");         
   
#ifdef DEBUG
          Serial.println("Show usage hints.");
#endif
       
  }

  else if(strcmp(topic, CMND_TOPIC_SCHEME) == 0) {
       if (intValue == DAY) {
          scheme = DAY;
#ifdef DEBUG
          Serial.println("Clock scheme DAY");
#endif
       }
       
       else if (intValue == NIGHT) {
          scheme = NIGHT;
#ifdef DEBUG
          Serial.println("Clock scheme NIGHT");
#endif
       }
       
       else if (intValue == SPRING) {
          scheme = SPRING;
#ifdef DEBUG
          Serial.println("Clock scheme SPRING");
#endif
       }

       else if (intValue == CRIMSON) {
          scheme = CRIMSON;
#ifdef DEBUG
          Serial.println("Clock scheme CRIMSON");
#endif
       }
                  
       else if (intValue == VIOLET) {
          scheme = VIOLET;
#ifdef DEBUG
          Serial.println("Clock scheme VIOLET");
#endif
       }

       else if (intValue == AQUA) {
          scheme = AQUA;
#ifdef DEBUG
          Serial.println("Clock scheme AQUA");
#endif
       }
                   
       else {
           if (scheme < USER) scheme++;
           else scheme = DAY;
#ifdef DEBUG
           Serial.println("Roll Clock Scheme.");
#endif
      }
      
      defineColorScheme(scheme);
#ifdef DEBUG
      Serial.println("Set new Clock Scheme.");
#endif
  }
  
  // show some feedback
  colorWipe(rgbHour, 5);
  mqttClient.publish(TELE_TOPIC_STATUS, msg);
  mqttPublish();
  blackWipe(3);

}

void mqttReconnect() {
//  while (!mqttClient.connected()) {
#ifdef DEBUG
    Serial.println("Connecting to MQTT..."); 
#endif
    // connect as mqtt client without LastWill
    if (mqttClient.connect(DEVICENAME, mqttUser, mqttPassword, TELE_TOPIC_STATUS, 0, 1, "Bye from LEDClock.", 0)) {

#ifdef DEBUG
      Serial.println("MQTT connected");  
#endif
      mqttClient.publish(TELE_TOPIC_STATUS, "Hello from LEDClock");
      mqttPublish();
      mqttClient.subscribe(CMND_TOPIC);
    } 
    else {

#ifdef DEBUG
      Serial.print("failed with state ");
      Serial.println(mqttClient.state());
#endif
      delay(10);
     }
  //}
}

void mqttPublish() {
 char pl[4];
 sprintf(pl,"%i", mode);
 mqttClient.publish(TELE_TOPIC_STATUS_MODE, pl);
 sprintf(pl,"%i", scheme);
 mqttClient.publish(TELE_TOPIC_STATUS_SCHEME, pl);
}
