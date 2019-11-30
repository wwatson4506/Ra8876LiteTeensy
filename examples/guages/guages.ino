// guages.ino
#include "Arduino.h"
#include "Ra8876_Lite.h"
#include "tft.h"
#include "vt100.h"
#include <math.h>



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

volatile int16_t curVal[6] = {-1, -1, -1, -1, -1, -1};
volatile int16_t oldVal[6] = {0, 0, 0, 0, 0, 0};
const int16_t posx[6] = {63, 193, 323, 453, 583, 713};
const int16_t posy[6] = {63, 63, 63, 63, 63, 63};
const int16_t radius[6] = {63, 63, 63, 63, 63, 63};
const uint8_t analogIn[6] = {A0, A1, A2, A3, A8, A9};
const uint16_t needleColors[6] = {myColors[5], myColors[15], myColors[18], myColors[12], 0xFC80, myColors[3]};
const uint8_t degreesVal[6][2] = {
{150,240},
{150,240},
{150,240},
{150,240},
{150,240},
{150,240},
};

void setup() {
	
//	Serial.begin(115200);
//	while(!Serial);
	tft_init();
	initVT100();
    setTextAt(0,0);
	tft_cls(myColors[11]);
	setFontSize(1,false);

	for (uint8_t i = 0; i < 6; i++) {
		drawGauge(posx[i], posy[i], radius[i]);
	}
	tft_slcls(myColors[11]);
	tft_slprint(0,myColors[1],myColors[11],"Sumotoy's guages sketch on the T4");
}

void loop(void) {
  for (uint8_t i = 0; i < 6; i++) {
    curVal[i] = random(255);
    //curVal[i] =255;
    drawNeedle(i,myColors[11]);
    //delay(1000);
    /*
    curVal[i] = map(analogRead(analogIn[i]), 0, 1024, 1, 255);
    drawNeedle(i,RA8875_BLACK);
    */
  }
}


void drawGauge(uint16_t x, uint16_t y, uint16_t r) {
  drawCircle(x, y, r, myColors[1]); //draw instrument container
  faceHelper(x, y, r, 150, 390, 1.3); //draw major ticks
  if (r > 15) faceHelper(x, y, r, 165, 375, 1.1); //draw minor ticks

}

void faceHelper(uint16_t x, uint16_t y, uint16_t r, int from, int to, float dev) {
  float dsec,rdev;
  uint16_t w,h,nx,ny;
  int i;
  rdev = r / dev;
  for (i = from; i <= to; i += 30) {
    dsec = i * (PI / 180.0);
    nx = (uint16_t)(1 + x + (cos(dsec) * rdev));
    ny = (uint16_t)(1 + y + (sin(dsec) * rdev));
    w =  (uint16_t)(1 + x + (cos(dsec) * r));
    h =  (uint16_t)(1 + y + (sin(dsec) * r));
    drawLine(nx, ny, w, h, myColors[1]);
  }
}

void drawNeedle(uint8_t index, uint16_t bcolor) {
  uint16_t i;
  if (oldVal[index] != curVal[index]) {
  if (curVal[index] > oldVal[index]) {
    for (i = oldVal[index]; i <= curVal[index]; i++) {
      if (i > 0) drawPointerHelper(index,i - 1, posx[index], posy[index], radius[index], bcolor);
      drawPointerHelper(index,i, posx[index], posy[index], radius[index], needleColors[index]);
      if ((curVal[index] - oldVal[index]) < (128)) delay(1);//ballistic
    }
  }
  else {
    for (i = oldVal[index]; i >= curVal[index]; i--) {
      drawPointerHelper(index,i + 1, posx[index], posy[index], radius[index], bcolor);
      drawPointerHelper(index,i, posx[index], posy[index], radius[index], needleColors[index]);
      //ballistic
      if ((oldVal[index] - curVal[index]) >= 128) {
        delay(1);
      } else {
        delay(3);
      }
    }
  }
  oldVal[index] = curVal[index];
  }
  //oldVal[index] = curVal[index];
}

void drawPointerHelper(uint8_t index,int16_t val, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
  float dsec;
  const int16_t minValue = 0;
  const int16_t maxValue = 255;
  if (val > maxValue) val = maxValue;
  if (val < minValue) val = minValue;
  dsec = (((float)(uint16_t)(val - minValue) / (float)(uint16_t)(maxValue - minValue) * degreesVal[index][1]) + degreesVal[index][0]) * (PI / 180);
  uint16_t w = (uint16_t)(1 + x + (cos(dsec) * (r / 1.35)));
  uint16_t h = (uint16_t)(1 + y + (sin(dsec) * (r / 1.35)));
  /*
  min: x:713 | y:63 | w:673 | h:87
  min: x:713 | y:63 | w:754 | h:87
  */
  /*
  if (index == 5){
  Serial.print("x:");
  Serial.print(x);
  Serial.print(" | y:");
  Serial.print(y);
  Serial.print(" | w:");
  Serial.print(w);
  Serial.print(" | h:");
  Serial.print(h);
  Serial.print("\n");
  }
  */
  drawLine(x, y, w, h, color);
  fillCircle(x, y, 2, color);
}
