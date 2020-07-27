// vt100Demo.ino
// Same as the scroll.ino example but uses
// VT100 command strings to scroll up and down.
#include "Arduino.h"
//#include "Ra8876_Lite.h"
#include "tft.h"
#include "vt100.h"

// Array of RA8876 Basic Colors
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

// VT100 string commands defs.
#define CLS "\033[2J"
#define HOMECURSOR "\033[f"
#define SCROLLUP "\033D"
#define SCROLLDN "\033M"

int i = 0, j = 0;
int color = 1;

void setup() {
	tft_init();
	initVT100();
	VideoPrintString(CLS);
	VideoPrintString(HOMECURSOR);
	setFontSize(1,false);
	setTextColorFG(1,0);
	tft_slcls(myColors[0]);
	tft_slprint(0,myColors[13],myColors[0],"Scroll Down, Scroll Up: Using VT100 Commands");
	setFontSource(0);
	setFontSize(1,false);
	setTextAt(0,0);
	for(i = 0; i < 256; i++) {
		if(color == 22) color = 1;
		setTextColorFG(myColors[color++] , myColors[0]);
		tft_print(i);
	}
	setTextColorFG(myColors[1] , myColors[0]);
}

void loop() {
	while(j++ < 10) {
		for(i = 0; i < 21; i++) {
			VideoPrintString(SCROLLDN);
		}
		for(i = 0; i < 21; i++) {
			VideoPrintString(SCROLLUP);
		}
	}
	j = 0;
	// Delay to show internal character set.
	delay(2000);
}
	
