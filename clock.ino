/*
 *  Clock with WS8212 60 LED Ring
 *  
 *  MAWISoft 2020
 *  
 *  functions
 *  
 */
 

#include "clock.h"


void showClock(){
    // if necessary:reverse
#ifdef REVERSE
    for(uint16_t i = 0; i < NUM_LEDS; i++) clock_leds[NUM_LEDS-1-i] = leds[i];
#else
    for(uint16_t i = 0; i < NUM_LEDS; i++) clock_leds[i] = leds[i];
#endif
    
    yield();                       // let the ESP do his stuff and then
    FastLED.show();                // show all
}

void drawClock_classic(int h_m, byte m, byte s){

  // first draw clockface
  clearAll();
  drawClockface();

  // then the hands
  drawHourhand(h_m);
  leds[m] = rgbMinute;  // draw the Minute hand
  leds[s] = rgbSecond;  // draw the Second hand 

}


void drawClock_minarc(int h_m, byte m, byte s){

 // draw minutes in arc
 for (byte i = 0; i < NUM_LEDS; i++) {
   if (i <= m) {
      leds[i] = rgbMinute;
    }
    else {
      leds[i] = rgbBackground;
    }
  }
  
  // draw divisions
  drawClockface();
  
  // draw seconds and hour as hands
  leds[s] = rgbSecond;
  drawHourhand(h_m);

}


void drawClock_arc(int h_m, byte m, byte s){

  int r, g, b;
  for (byte i = 0; i < NUM_LEDS; i++) {

    // draw minute arc in green component 
    if (i <= m) {
       led.g = GREEN;
     }
     else {
       led.g = 0;
     }
  
    // draw hour arc in red component
    if (i <= h_m) {
       led.r = RED;
    }
    else {
       led.r = 0;
    }
  
    // draw second hand in blue component
    if (i == s) {
       led.b = BLUE;       
    }
    else {
        led.b = 0;
    }

    // draw the composite pixel
    leds[i] = led;
  }

}


void drawClockface(){

// draw a regular Clockface
  
    for (int i = 0; i < NUM_LEDS; i += 5) {
      leds[i] = rgbDivisions;       // draw 5 minute divisions 
    }

}


void drawHourhand(int h_m){
  
// draw a wide Hour hand with edges

    leds[h_m] = rgbHour;               // draw the Hour hand 
    if(h_m > 1) { leds[h_m-1] = rgbHour; leds[h_m-1].nscale8_video(64);}        
    if(h_m < 59) { leds[h_m+1] = rgbHour; leds[h_m+1].nscale8_video(64);}   

}


void clearAll(void){

// clear all LEDs

   for (int i = 0; i < NUM_LEDS; i++) {
      clock_leds[i] = rgbBackground;  
      leds[i] = rgbBackground;                               
    }

}

void colorWipe(CRGB Colour, int SpeedDelay) {

// draw LED colours in a wipe    
  
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      leds[i]= Colour;
      showClock();
      FastLED.delay(SpeedDelay);
  }
#ifdef DEBUG
      Serial.println("Show colorWipe");
#endif
}


void blackWipe(int SpeedDelay) {
  
// clear all LEDs in a wipe

  for(byte i=0; i<NUM_LEDS; i++) {
      leds[i]= CRGB::Black;
      showClock();
      FastLED.delay(SpeedDelay);
  }
#ifdef DEBUG
      Serial.println("Show blackWipe");
#endif

}

void drawRainbow() {

// cycle rainbow colors without block

   static byte startHue = 0;

   fill_rainbow(leds, NUM_LEDS, startHue, deltaHue);

   if(startHue++ > 255) startHue = 0;

}

void rainbowCycle(int SpeedDelay) {

// cycle Rainbow colors blocking
  
  byte *c;
  uint16_t i, j;

  for(j=0; j<256*3; j++) {              // 3 cycles of all colors on wheel
    for(i=0; i< NUM_LEDS; i++) {
      c = Wheel(((i * 256 / NUM_LEDS) + j) & 255);
      leds[i]=CRGB(*c, *(c+1), *(c+2));
    }
  showClock();
  FastLED.delay(SpeedDelay);
  }
#ifdef DEBUG
  Serial.println("Show Rainbow.");
#endif
}


byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;

}



byte adjustBrightness() {
  
// adjust Brightness according to value of LDR

  int a = analogRead(LDR);
#ifdef DEBUG
  Serial.print("LDR: ");  
  Serial.println(a);
#endif    
  int Brightness = map(a, 712, 72, 246, 48);  // map LDR-Values to Byte
#ifdef DEBUG
  Serial.print("Brightness: ");
  Serial.println(Brightness);
#endif

 return constrain(Brightness,48,246);

}


bool checkDst(byte day, byte month, byte dow)  {

// if Daylight Savings Time, return true  
  
     if (month < 3 || month > 10)  return false; 
     if (month > 3 && month < 10)  return true; 
     int previousSunday = day - dow;
     if (month == 3) return previousSunday >= 25;
     else return previousSunday < 25;

}
