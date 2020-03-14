/*
    Clock with WS8212 60 LED Ring

    MAWISoft 2020

    Thanks to Daniel Garcia for FastLED,
    to Fabrice Weinberg for his 'NTPClient'
    to Nick O'Leary for his 'pubsubclient'
    and to the whole team of arduino.cc
    I found some inspiration by wol clock from Jon Fuge.

    This Program ist published under the terms of the GNU Lesser General Public License as
    published by the Free Software Foundation, either version 3 of
    the License, or (at your option) any later version.
    It is distrubited in the hope that it will be useful,
    but without any warranty,
    see <http://www.gnu.org/licenses/>.

*/


#include "clock.h"



void setup() {

  // start serial if in debug mode
#ifdef DEBUG
  Serial.begin(115200);
  while (!Serial);                                 // wait for serial attach
  Serial.println("Configuring WiFi,RTC, NTP and WS8212 ...");
#endif

  // Setup FastLED Lib
  FastLED.addLeds<LED_TYPE, DATA, COLOR_ORDER>(clock_leds, NUM_LEDS).setCorrection(TypicalSMD5050);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.setMaxPowerInVoltsAndMilliamps(VOLT, MAX_MILLIAMP);

  // start WiFi Server
  IPAddress wifi_ip;

#ifdef WIFIMODE_AP
  WiFi.softAPConfig(ip, ip, netmask);       // configure ip address for softAP
  if (WiFi.softAP(ssid, pwd)) wifi_ip = WiFi.softAPIP();
#endif

#ifdef WIFIMODE_STA
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  wifi_ip = WiFi.localIP();
  WiFi.setSleepMode(WIFI_LIGHT_SLEEP);        // this works only in station mode
#endif

  // start TCP server
  server.begin();   // start TCP server

#ifdef DEBUG
  Serial.println("TCP Server started.");
  Serial.print("Connect to ");
  Serial.println(ssid);
  Serial.println(wifi_ip);
  Serial.println(port);
#endif

  // setup OTA
  ArduinoOTA.setHostname(OTAHOSTNAME);
  
  ArduinoOTA.onStart([]() {
    colorWipe(CRGB::Yellow,4);
#ifdef DEBUG
    Serial.println("\nStartOTA");
#endif  
    });
  ArduinoOTA.onEnd([]() {
    colorWipe(CRGB::Green,4);
#ifdef DEBUG
    Serial.println("\nEndOTA");
#endif
  });
  ArduinoOTA.onError([](ota_error_t error) {
    colorWipe(CRGB::Red,4);
#ifdef DEBUG
    Serial.printf("OTAError[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
#endif
  });
  ArduinoOTA.begin();
#ifdef DEBUG
  Serial.println("OTA ready. ");
#endif  

  // start NTP client
  ntpClient.begin();

  // start MQTT client
  mqttClient.setServer(mqttServer, mqttPort);     //start mqtt-server
  mqttClient.setCallback(callbackMQTT);

  // start I2C
  Wire.begin(SDA, SCL);

  // start RTC
  if (!Rtc.begin()) {
#ifdef DEBUG
    Serial.println("Error: Could not find RTC!");
#endif
  }

  if (Rtc.lostPower()) {
#ifdef DEBUG
    Serial.println("RTC lost power, call NTP ...");
#endif
    if (ntpClient.update()) Rtc.adjust(ntpClient.getEpochTime());
  }

  // set PIR pin
  pinMode(PIR, INPUT_PULLUP);

  // show some fancy
  rainbowCycle(8);
  
  // wait 2 seconds to settle
  delay(2000);

  // set colour scheme
  defineColorScheme(scheme);

  blackWipe(5);

#ifdef DEBUG
  Serial.println("Setup ready. Start loop ...");
#endif

  delay(1000);
}


void loop() {

  // handle OTA request
  ArduinoOTA.handle();
  delay (300);

  // handle mqtt Input
  if (!mqttClient.connected())  {
     reconnectMQTT();
  }
  mqttClient.loop();
  delay (300);
  
  // get time from RTC
  DateTime now = Rtc.now();

  byte s = now.second();  	      // get seconds
  byte m = now.minute();  	      // get minutes
  int h = now.hour();    	        // get hours
  int y = now.year();     	      // get year
  byte mo = now.month();  	      // get month
  byte d = now.day();     	      // get day
  byte dow = now.dayOfTheWeek(); 	// get day of week

  // Calculate Hour
  if (checkDst(d, mo, dow)) h++;  // Add 1 hour due to detected DST
  if (h == 24) h = 0;             // show 24h as 0h
  if (h > 11) h -= 12;            // this clock shows only 12 hours

#ifdef DEBUG
  // monitor DateTime
  Serial.print(days[dow]);
  Serial.print(", ");
  Serial.print(d, DEC);
  Serial.print(". ");
  Serial.print(months[mo]);
  Serial.print(" ");
  Serial.print(y);
  Serial.println();
  Serial.print(h);
  Serial.print(" Uhr ");
  Serial.print(m, DEC);
  Serial.print(" und ");
  Serial.print(s, DEC);
  Serial.println(" Sekunden");
  Serial.println();
  yield();
#endif

  int h_m = (int)(h * 5 + m / 12);                        // transform hours to minutes to show hour hand on clockface

  if (d == 8 && h_m == 8 && m == 8 && s == 8) {           // adjust RTC time once a month
      if (ntpClient.update()) {                           // according to NTP server
          Rtc.adjust(ntpClient.getEpochTime());
#ifdef DEBUG
          Serial.println("Clock updated through NTP.");
#endif
      }
  }

  // check for presence of person
  if (digitalRead(PIR) == HIGH) {                          // detect person
    m_pir = m_delta;
#ifdef DEBUG2
    Serial.println("Person detected.");
#endif
  }

  if (s == 20) {
    if (m_pir > 0) {
      if(state != INSTANTOFF) state = ON;
      m_pir--;                                            // decrease minute counter
    }
    else {                                                // save electricity
      if (state == ON) SHUTOFF
      }
  }

#ifdef DEBUG2
  Serial.printf("Minutes until shutoff: %i \n", m_pir);
#endif


  if (state == ON) {                   // show clockface only if state == ON

    if (s == 40) {                // adjust brightness once every minute
      FastLED.setBrightness(adjustBrightness());
    }

    if (m == 0 && s == 0)  {      // fancy wipe every full hour
      if (mode == CLASSIC) colorWipe(rgbMinute, 5);
      blackWipe(3);
    }

    // draw clock according to choice
    if (mode == CLASSIC) drawClock_classic (h_m, m , s);
    else if (mode == MINARC) drawClock_minarc (h_m, m , s);
    else if (mode == ARC) drawClock_arc (h_m, m, s);
    else if (mode == RAINBOW) drawRainbow();

    showClock();
  }

  delay(300);
}
