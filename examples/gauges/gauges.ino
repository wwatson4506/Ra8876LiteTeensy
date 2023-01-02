/***************************************************************
 * gauges.ino
 *
 * This example is a modified version of Sumotoy's guages example
 * from his RA8875 driver. Modified to work with the RA8876 TFT controller.
 ***************************************************************/
#include "Arduino.h"
#include "RA8876_t3.h"
#include <math.h>

#define RA8876_CS 10
#define RA8876_RESET 8
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins

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

volatile int16_t curVal[6] = { -1, -1, -1, -1, -1, -1};
volatile int16_t oldVal[6] = {0, 0, 0, 0, 0, 0};
const int16_t posx[6] = {63, 193, 323, 453, 583, 713};
const int16_t posy[6] = {63, 63, 63, 63, 63, 63};
const int16_t radius[6] = {63, 63, 63, 63, 63, 63};
const uint8_t analogIn[6] = {A0, A1, A2, A3, A8, A9};
const uint16_t needleColors[6] = {myColors[5], myColors[15], myColors[18], myColors[12], 0xFC80, myColors[3]};
const uint8_t degreesVal[6][2] = {
  {150, 240},
  {150, 240},
  {150, 240},
  {150, 240},
  {150, 240},
  {150, 240},
};

void setup() {
  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);

  Serial.begin(115200);
  while (!Serial);
  tft.begin();
  tft.graphicMode(true);
  tft.setTextCursor(0, 0);
  tft.fillScreen(myColors[11]);
  tft.setFontSize(1, false);
  for (uint8_t i = 0; i < 6; i++) {
    drawGauge(posx[i], posy[i], radius[i]);
  }
  tft.fillStatusLine(myColors[11]);
  tft.printStatusLine(0, myColors[1], myColors[11], "Sumotoy's gauges sketch on the T4");
  tft.setMargins(0, 0, tft.width(), tft.height()); //so fillscreen doesn't erase the status bar
}

void loop(void) {
  for (uint8_t i = 0; i < 6; i++) {
    curVal[i] = random(254);
    if (curVal[i] == 0) Serial.println("found a zero");
    drawNeedle(i, myColors[11]);
  }
}


void drawGauge(uint16_t x, uint16_t y, uint16_t r) {
  tft.drawCircle(x, y, r, myColors[1]); //draw instrument container
  faceHelper(x, y, r, 150, 390, 1.3); //draw major ticks
  if (r > 15) faceHelper(x, y, r, 165, 375, 1.1); //draw minor ticks
}

void faceHelper(uint16_t x, uint16_t y, uint16_t r, int from, int to, float dev) {
  float dsec, rdev;
  uint16_t w, h, nx, ny;
  int i;
  rdev = r / dev;
  for (i = from; i <= to; i += 30) {
    dsec = i * (PI / 180.0);
    nx = (uint16_t)(1 + x + (cos(dsec) * rdev));
    ny = (uint16_t)(1 + y + (sin(dsec) * rdev));
    w =  (uint16_t)(1 + x + (cos(dsec) * r));
    h =  (uint16_t)(1 + y + (sin(dsec) * r));
    tft.drawLine(nx, ny, w, h, myColors[1]);
  }
}

void drawNeedle(uint8_t index, uint16_t bcolor) {
  int16_t i;
  if (curVal[index] == 0) Serial.println("drawing a zero");
  if (oldVal[index] != curVal[index]) {
    if (curVal[index] > oldVal[index]) {
      for (i = oldVal[index]; i <= curVal[index]; i++) {
        if (i > 0) drawPointerHelper(index, i - 1, posx[index], posy[index], radius[index], bcolor);
        drawPointerHelper(index, i, posx[index], posy[index], radius[index], needleColors[index]);
        if ((curVal[index] - oldVal[index]) < (128)) delay(1);//ballistic
      }
    }
    else {
      for (i = oldVal[index]; i >= curVal[index]; i--) {
        drawPointerHelper(index, i + 1, posx[index], posy[index], radius[index], bcolor);
        drawPointerHelper(index, i, posx[index], posy[index], radius[index], needleColors[index]);
        //ballistic
        if ((oldVal[index] - curVal[index]) >= 128) {
          delay(1);
        } else {
          delay(3);
        }
      }
    }
    oldVal[index] = curVal[index];
  if (curVal[index] == 0) Serial.println("FINISHED drawing a zero");
  }
}

void drawPointerHelper(uint8_t index, int16_t val, uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
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
  tft.drawLine(x, y, w, h, color);
  tft.fillCircle(x, y, 2, color);
}
