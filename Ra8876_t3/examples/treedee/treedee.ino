// treedee.ino example
// Taken from sumotoy's RA8875 library
// and modified for use with the RA8876.
#include "Arduino.h"
#include "RA8876_t3.h"
#include <math.h>

#define RA8876_CS 10
#define RA8876_RESET 9
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

float sin_d[] = { 
  0,0.17,0.34,0.5,0.64,0.77,0.87,0.94,0.98,1,0.98,0.94,
  0.87,0.77,0.64,0.5,0.34,0.17,0,-0.17,-0.34,-0.5,-0.64,
  -0.77,-0.87,-0.94,-0.98,-1,-0.98,-0.94,-0.87,-0.77,
  -0.64,-0.5,-0.34,-0.17 };
float cos_d[] = { 
  1,0.98,0.94,0.87,0.77,0.64,0.5,0.34,0.17,0,-0.17,-0.34,
  -0.5,-0.64,-0.77,-0.87,-0.94,-0.98,-1,-0.98,-0.94,-0.87,
  -0.77,-0.64,-0.5,-0.34,-0.17,0,0.17,0.34,0.5,0.64,0.77,
  0.87,0.94,0.98};
float d = 30;
float px[] = { 
  -d,  d,  d, -d, -d,  d,  d, -d };
float py[] = { 
  -d, -d,  d,  d, -d, -d,  d,  d };
float pz[] = { 
  -d, -d, -d, -d,  d,  d,  d,  d };

float p2x[] = {
  0,0,0,0,0,0,0,0};
float p2y[] = {
  0,0,0,0,0,0,0,0};

int r[] = {0,0,0};

uint8_t ch = 13; // Default line color
uint16_t ccolor = myColors[ch];

void setup() {
  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
    
  tft.begin();
  tft.setCursor(0,0);
  tft.fillScreen(myColors[11]);
  tft.setFontSize(1,false);
  tft.fillStatusLine(myColors[11]);
  tft.printStatusLine(0,myColors[1],myColors[11],"Sumotoy's treedee sketch on the T4.");
  tft.setMargins(0, 0, tft.width(), tft.height()); //so fillscreen doesn't erase the status bar
}

//delay between interations 
uint8_t speed = 20; // Change this setting to go from reasonable to rediculous .

void loop() {
  tft.fillScreen(myColors[11]);
  r[0] = r[0] + 1;
  r[1] = r[1] + 1;
  if (r[0] == 36) r[0] = 0;
  if (r[1] == 36) r[1] = 0;
  if (r[2] == 36) r[2] = 0;
  for (int i = 0; i < 8; i++)
  {
    float px2 = px[i];
    float py2 = cos_d[r[0]] * py[i] - sin_d[r[0]] * pz[i];
    float pz2 = sin_d[r[0]] * py[i] + cos_d[r[0]] * pz[i];

    float px3 = cos_d[r[1]] * px2 + sin_d[r[1]] * pz2;
    float py3 = py2;
    float pz3 = -sin_d[r[1]] * px2 + cos_d[r[1]] * pz2;

    float ax = cos_d[r[2]] * px3 - sin_d[r[2]] * py3;
    float ay = sin_d[r[2]] * px3 + cos_d[r[2]] * py3;
    float az = pz3 - 190;

    p2x[i] = ((tft.width()) / 2) + ax * 500 / az;
    p2y[i] = ((tft.height()) / 2) + ay * 500 / az;
  }
  for (int i = 0; i < 3; i++) {
    tft.drawLine(p2x[i], p2y[i], p2x[i + 1], p2y[i + 1], ccolor);
    tft.drawLine(p2x[i + 4], p2y[i + 4], p2x[i + 5], p2y[i + 5], ccolor);
    tft.drawLine(p2x[i], p2y[i], p2x[i + 4], p2y[i + 4], ccolor);
  }
  tft.drawLine(p2x[3], p2y[3], p2x[0], p2y[0], ccolor);
  tft.drawLine(p2x[7], p2y[7], p2x[4], p2y[4], ccolor);
  tft.drawLine(p2x[3], p2y[3], p2x[7], p2y[7], ccolor);
  delay(speed); // Wanna see how fast this runs? modify 'speed' above! (Or comment out)
// Uncomment for random colored frames
//  if (ch >= 22) {
//    ch = 1;
//    ccolor = myColors[ch];
//    ccolor = myColors[random(1, 22)];
//  }
//  else {
//    ch++;
//  }
}
