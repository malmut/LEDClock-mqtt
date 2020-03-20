/* 
 *   Clock with WS8212 60 LED Ring
 *  
 *   MAWISoft 2020
 *   
 *   different color schemes for clock
 *
 */

#include "clock_schemes.h"


void defineColorScheme(byte scheme) {

  // define color schemes

  switch (scheme) {

    default:

    case VIOLET:
      rgbDivisions = RGB(112, 180, 242); // light blue
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(245, 216, 0);        // dark yellow
      rgbMinute = RGB(0, 240, 12);       // dark green
      rgbSecond = RGB(53, 53, 98);       // violet blue
      break;

    case SPRING:

      rgbDivisions = RGB(115, 158, 5);   // moose green
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(180, 100, 0);        // dark orange
      rgbMinute = RGB(96, 248, 0);       // dark green
      rgbSecond = RGB(250, 230, 10);     // gold
      break;

    case NIGHT:

      rgbDivisions = RGB(48, 48, 48);    // lilac
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(168, 2, 46);         // purple
      rgbMinute = RGB(51, 22, 155);      // violett
      rgbSecond = RGB(218, 127, 0);      // light orange
      break;

    case AQUA:

      rgbDivisions = RGB(5, 248, 127);   // turqoise  
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(250, 120, 4);        // dark orange
      rgbMinute = RGB(215, 188, 0);      // light orange
      rgbSecond = RGB(24, 208, 248);     // dark blue
      break;

    case CRIMSON:

      rgbDivisions = RGB(248,248,22);    // yellow
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(248, 22, 22);        // crimson
      rgbMinute = RGB(22, 248, 22);      // green
      rgbSecond = RGB(248, 128, 22);     // light orange
      break;


    case DAY:

      rgbDivisions = RGB(0, 32, 0);      // light green
      rgbBackground = RGB(0, 0, 0);      // black
      rgbHour = RGB(0, 0, 236);          // dark blue
      rgbMinute = RGB(180, 100, 0);      // gold
      rgbSecond = RGB(240, 26, 0);       // dark crimson
      break;

    case USER:

      if (flagUserColorsSet) {
        rgbDivisions = rgbUserDivisions;
        rgbBackground =  rgbUserBackground;
        rgbHour = rgbUserHour;
        rgbMinute = rgbUserMinute;
        rgbSecond = rgbUserSecond;
      }
      else {
        rgbDivisions = RGB(36, 0, 36);  // light purple
        rgbBackground = RGB(0, 0, 0);   // black
        rgbHour = RGB(196, 64, 0);      // dark organge
        rgbMinute = RGB(36, 178, 34);   // smaragd
        rgbSecond = RGB(248, 178, 0);   // blue green
      }
      break;
  }
}


byte setCustomColors(char *strColor) {

  byte intColor[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  const char delim[2] = ",";              //char *token = "";
  int i = 0;
  // get the first token
  char *token = strtok(strColor, delim);
  // walk through other tokens
  while ( token != NULL && i < 12 ) {
    intColor[i] = atoi(token);
    token = strtok(NULL, delim);
    i++;
  }

#ifdef DEBUG
  Serial.println("Custom Colors set: ");
  for(i=0;i<12;i++){
          Serial.printf(" %i,", intColor[i]);
  }
  Serial.println(" \n");  
#endif


  // gamma corrected RGB
  rgbUserDivisions = RGB(pgm_read_byte(&gamma_R[intColor[0]]), pgm_read_byte(&gamma_G[intColor[1]]), pgm_read_byte(&gamma_B[intColor[2]])); // brightness correction through table
  rgbUserHour = RGB(pgm_read_byte(&gamma_R[intColor[3]]), pgm_read_byte(&gamma_G[intColor[4]]), pgm_read_byte(&gamma_B[intColor[5]]));
  rgbUserMinute = RGB(pgm_read_byte(&gamma_R[intColor[6]]), pgm_read_byte(&gamma_G[intColor[7]]), pgm_read_byte(&gamma_B[intColor[8]]));
  rgbUserSecond = RGB(pgm_read_byte(&gamma_R[intColor[9]]), pgm_read_byte(&gamma_G[intColor[10]]), pgm_read_byte(&gamma_B[intColor[11]]));
  rgbUserBackground = RGB(0, 0, 0);            
  
  flagUserColorsSet = true;

  return 1;									                // success
}

void setCustomElement(RGB clockfaceElement, byte r, byte g, byte b) {

  clockfaceElement = RGB(r, g, b);

}
