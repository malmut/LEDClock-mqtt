# LEDClock-mqtt

## ESP 8266 operated wall clock built from a classical SMD5050 RGB-LED ring with the following features:

  - a high precision real-time clock
  - exchangeable clock faces: 
      a classic face with single LEDs as hands, 
      an elegant face which shows the expired minutes as expand and 
      a modern face that shows expired minutes and hours as RGB-compounds
  - changeable colormodes
  - user blended RGB-Colors
  - automatic switch to daylight-saving-time
  - regular update of time through NTP-Server
  - brightness control through measurement of ambient light 
  - power saving with passive Infrared-Module and Person Detection

  **_NEW! Built as MQTT-Client using pubsubclient by Nick O'Leary._**


**Hardware:**

- ESP8266 module (e.g. NodeMCU or WeMos D1 mini)
- WS 2812(b ) SMD5050 LED ring
- RTC DS 3231 or similar
- PIR module like HC-sr501
- LDR
- 3A power adaptor
  
**Software:**

- Arduino IDE with Package for ESP8266 boards
- libraries: Fabrice Weinberg's 'NTPClient', Nick O'Leary's 'pubsubclient', Daniel Garcia's 'FastLED'


#### Good luck, have fun!
