/*
 *   Clock with WS8212 60 LED Ring
 *  
 *   MAWISoft 2020
 *   
 *   defines, constants and clock functions
 * 
*/

#ifndef _clock_h_
#define _clock_h_

//#define DEBUG                       // uncomment for debugging purposes
#define REVERSE                     // if necessary reverse LED show

// ESP Wifi Library //

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "credentials.h"

// choose ESP WiFi mode:
//#define WIFIMODE_AP                // client connects directly to ESP
#define WIFIMODE_STA                 // ESP connects to router


#ifdef WIFIMODE_AP
// prepare AP mode:
const char *ssid = "MWClock";     // You will connect your phone to this Access Point
const char *pw = "node1234";      // and this is the password
IPAddress ip(192, 168, 4, 1);     // connect to this IP
IPAddress netmask(255, 255, 255, 0);
const int port = 80;              // and this port
#endif


#ifdef WIFIMODE_STA
// prepare STATION mode:
#include "credentials.h"                // load SSID and Password of home-network
const int port = 80;
#endif

WiFiServer server(port);
WiFiClient wifiClient;

// OTA settings
#include <ArduinoOTA.h>
#define OTAHOSTNAME "LEDCLOCK" 

// UDP and NTP settings
#include <WiFiUdp.h>
#include <NTPClient.h>

#define NTPSERVER "de.pool.ntp.org"
#define TZ 3600                         // TimeZone MET (+1 h in seconds)

WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, NTPSERVER, TZ);

// FastLED Library //

#define _FastLED_
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
#define FASTLED_ALLOW_INTERRUPTS 0 
#include <FastLED.h>

#define NUM_LEDS 60           // the Ring has 60 LEDs
#define LED_TYPE WS2812B
#define CORRECTION TypicalSMD5050
#define COLOR_ORDER GRB
#define BRIGHTNESS 192
#define DATA 7                // datapin
#define LDR A0
#define VOLT 5
#define MAX_MILLIAMP 3600     // uncomment if necessary depending on the Powerinput

CRGB leds[NUM_LEDS], clock_leds[NUM_LEDS];
CRGB led = CRGB(0,0,0);

// MQTT functions
#include "clock_mqtt.h" 

// Wire and RTC Library //

#include <Wire.h>
#include <RTClib.h>
#define SDA D1                           // pins for I2C 
#define SCL D2

RTC_DS3231 Rtc;


// User Variables and Definitions //

#include "clock_schemes.h"

#define LDR A0                    // input pin for photoresistor

#define ON 1
#define OFF 0
#define INSTANTOFF 10

#define CLASSIC 164               // the different clock modes
#define MINARC 165
#define ARC 166

#define RAINBOW 167
#define UPDATE 168
const byte deltaHue = (byte)(255/NUM_LEDS);  // hue for rainbow_fill

byte mode = CLASSIC;               // standard clock mode

// settings for timed shutoff
#define PIR D6                    // Pin with PIR Sensor
#define SHUTOFF {clearAll();FastLED.show();state=OFF;}  
byte state = ON;                  // clock state
const byte m_delta = 15;           // timespan in minutes before shutoff
int m_pir = m_delta;

// arrays for months and days
char days[7][12] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
char months[13][12] = {" ", "Januar", "Februar", "März", "April", "Mai", "Juni", "Juli", "August", "September", "Oktober", "November", "Dezember"};

// subroutines to draw different clocks 
void drawClock_classic(int h_m, byte m, byte s);  // set clock with hands for hour, minute, second
void drawClock_minarc(int h_m, byte m, byte s);   // set clock with hands for hour and second and an arc for minutes
void drawClock_arc(int h_m, byte m, byte s);      // set clock with arcs in pure RGB for hour and minute and a secondhand
void showClock(void);

// subroutine to check for Daylight Saving Time in Germany
bool checkDst(byte day, byte month, byte dow);

// subroutine to adjust the overall brightness of LEDs
byte adjustBrightness();

// misc. subroutines for drawing
void drawClockface(void);
void drawHourhand(int h_m);
void colorWipe(CRGB Colour, int SpeedDelay);
void blackWipe(int SpeedDelay);
void drawRainbow(void);
void rainbowCycle(int SpeedDelay);
byte * Wheel(byte WheelPos);
void clearAll(void);

// standard functions
void setup();
void loop();

#endif
