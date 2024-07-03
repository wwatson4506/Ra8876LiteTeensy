// UserDefinedFonts.ino
#include "Arduino.h"
#if defined(use_spi)
#include <SPI.h>
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif
#include <math.h>
#include "font8x16.h"

#if defined(use_spi)
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t41_p tft = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif

// Array of Simple RA8876 Basic Colors
PROGMEM uint16_t myColors[] = {
	0x0000,
	0xffff,
	0xf800,
	0xfc10,
	0x8000,
	0x07e0,
	0x87f0,
	0x0400,
	0x001f,
	0x051f,
	0x841f,
	0x0010,
	0xffe0,
	0xfff0,
	0x8400,
	0x07ff,
	0x87ff,
	0x0410,
	0xf81f,
	0xfc1f,
	0x8010,
	0xA145
};

void setup() {
  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
#if defined(BACKLITE)
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
#endif

#if defined(use_spi)
  tft.begin(); 
#else
  tft.begin(20);// 20 is working in 8bit and 16bit mode on T41
#endif
  //initVT100();
  tft.fontLoadMEM((char *)font8x16);
  tft.setFontSize(1,false);
  tft.setCursor(0,0);
  tft.println("Font Test");
  tft.fillScreen(myColors[0]);
  tft.fillStatusLine(myColors[0]);
  tft.printStatusLine(0,myColors[13],myColors[0],"Example of User Defined Characters. 8x16 font size. Uses tftRawWrite() function.");
  tft.setFontSource(1); //Select UDFont.
  tft.setTextColor(myColors[1] , myColors[0]);
  tft.setFontSize(0,true); // Set to smallest font scale.
  tft.printf("Hello Teensy!, font scaling = 0\n"); 
  tft.setFontSize(1,true); // Set to 1X font scale.
  tft.printf("Hello Teensy!, font scaling = 1\n"); 
  tft.setFontSize(2,true); // Set to 2X font scale.
  tft.printf("Hello Teensy!, font scaling = 2\n\n"); 
  tft.setFontSize(1,true); // Set to smallest font scale.
  tft.setTextColor(myColors[6] , myColors[0]);
  // Send raw characters to screen. Does not process ASCII control codes.
  for(uint8_t i = 0; i < 255; i++)
    tft.rawPrint(i);
}

void loop() {
}
