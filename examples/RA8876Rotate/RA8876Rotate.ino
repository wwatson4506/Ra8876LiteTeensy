#include <SPI.h>
#include "RA8876_t3.h"
#define RA8876_CS 10
#define RA8876_RESET 8
#define BACKLITE 5 //External backlight control connected to this Arduino pin

RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
  
void setup() {
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("Setup");
  tft.begin(20000000);
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
  tft.graphicMode(true);
  tft.Rotate();
  tft.fillScreen(BLACK);
  //tft.textRotate(false);
  uint16_t x0, y0, x1, y1, x2, y2, c;
  tft.fillRect(0,0,400,200, RED);

}

void loop() {
  // put your main code here, to run repeatedly:

}
