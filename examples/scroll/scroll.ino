// scroll.ino
#include "Arduino.h"
#include "Ra8876_Lite.h"
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

int i = 0, j = 0;
int color = 1;
void setup() {
	tft_init();
	initVT100();
    setTextAt(0,0);
	tft_cls(myColors[11]);
	setFontSize(1,false);
}

void loop() {

	tft_slcls(myColors[0]);
	tft_cls(myColors[0]);
	tft_slprint(0,myColors[13],myColors[0],"scroll_down(), scroll_up()");
	setFontSource(0);
	setFontSize(1,true);
	setTextAt(0,0);
	for(i = 32; i < 256; i++) {
		if(color == 22) color = 1;
		setTextColorFG(myColors[color++] , myColors[0]);
		tft_print(i);
	}
	setTextColorFG(myColors[1] , myColors[0]);
	while(j++ < 10) {
		for(i = 0; i < 21; i++) {
			scroll_down();
//			delay(50);
		}
		for(i = 0; i < 21; i++) {
			scroll_up();
//			delay(50);
		}
	}
	j = 0;
	delay(2000);
}
	
