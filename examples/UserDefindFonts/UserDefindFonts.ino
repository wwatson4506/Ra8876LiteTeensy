// UserDefinedFonts.ino
#include "Arduino.h"
#include "Ra8876_Lite.h"
#include "RA8876_t3.h"

//#include "vt100.h"
#include <math.h>

// This example is a modified version of Sumotoy's guages example
// from his RA8875 driver. Modified to work with the RA8876 TFT controller.
RA8876_t3 tft = RA8876_t3(10, 8, 11, 13, 12);

#include "font8x16.h"

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
	tft.init();
	//initVT100();
	tft.fontLoadMEM((char *)font8x16);
	tft.setFontSize(1,false);
	tft.setTextAt(0,0);
  tft.println("Font Test");
	tft.fillScreen(myColors[0]);
	tft.fillStatusLine(myColors[0]);
	tft.printStatusLine(0,myColors[13],myColors[0],"Example of User Defined Characters. 8x16 font size. Uses tftRawWrite() function.");
	tft.setFontSource(1); //Select UDFont.
	tft.setTextColorFG(myColors[1] , myColors[0]);
	tft.setFontSize(0,true); // Set to smallest font scale.
	tft.printf("Hello Teensy!, font scaling = 0\n"); 
	tft.setFontSize(1,true); // Set to 1X font scale.
	tft.printf("Hello Teensy!, font scaling = 1\n"); 
	tft.setFontSize(2,true); // Set to 2X font scale.
	tft.printf("Hello Teensy!, font scaling = 2\n\n"); 
	tft.setFontSize(1,true); // Set to smallest font scale.
	tft.setTextColorFG(myColors[6] , myColors[0]);
	// Send raw characters to screen. Does not process ASCII control codes.
	for(uint8_t i = 0; i < 255; i++)
		tft.rawPrint(i);
}

void loop() {
}