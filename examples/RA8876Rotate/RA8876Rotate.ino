#include <SPI.h>
#include "RA8876_t3.h"
#define RA8876_CS 10
#define RA8876_RESET 9
#define PINK        0xFC18
#include "font_Arial.h"
#define BACKLITE 5 //External backlight control connected to this Arduino pin
#define REG_DUMP_CNT 0x70
uint8_t reg_values[REG_DUMP_CNT];

RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins

void setup() {
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 5000)) ;
  Serial.println("Setup");
  tft.begin(20000000);
  for (uint8_t reg = 0; reg < REG_DUMP_CNT; reg++) {
    reg_values[reg] = tft.lcdRegDataRead(reg);
  }

  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
  tft.backlight(true);
  tft.graphicMode(true);
  Serial.printf("Before W: %d H: %d\n", tft.width(), tft.height());
  tft.fillScreen(LIGHTYELLOW);
  drawTestScreen();
}

void drawTestScreen() {
  tft.fillRect(0, 0, 50, 50, RED);
  tft.fillRect(tft.width() - 50, 0, 50, 50, GREEN);
  tft.fillRect(tft.width() - 50, tft.height() - 50 , 50, 50, BLUE);
  tft.fillRect(0, tft.height() - 50 , 50, 50, ORCHID);
  tft.drawLine(0, 0, tft.width(), tft.height(), WHITE);
  tft.drawLine(0, tft.height(), tft.width(), 0, BLACK);
  tft.drawTriangleFill(512, 300, 475, 400, 575, 400, PINK);
  tft.fillCircle(tft.width() / 2, tft.height() / 2, 50, ORCHID);
  tft.drawCircle(tft.width() / 2, tft.height() / 2, 25, BLACK);
  tft.drawTriangle(50, 50, 50, tft.height() - 50, tft.width() / 2 - 50, tft.height() / 2, BLUE);

  tft.setFont(Arial_14);
  tft.setTextColor(BLACK);
  tft.setCursor(25, tft.height() / 2);
  tft.printf("Rotation: %d", tft.getRotation());
  tft.setCursor(50, 50);
  tft.write('0');
  tft.setCursor(tft.width() - 50, 50);
  tft.write('1');
  tft.setCursor(50, tft.height() - 50);
  tft.write('2');
  tft.setCursor(tft.width() - 50, tft.height() - 50);
  tft.write('3');

  Serial.print("LCD Register dump:");
  for (uint8_t reg = 0; reg < REG_DUMP_CNT; reg++) {
    uint8_t r_value = tft.lcdRegDataRead(reg);
    if ((reg & 0xf) == 0) Serial.printf("\n%02x -", reg);
    Serial.printf(" %c%02x",
                  (r_value == reg_values[reg]) ? ' ' : '*',
                  r_value);
    reg_values[reg] = r_value;
  }
  Serial.print("\n");
}

void loop() {
  // put your main code here, to run repeatedly:
  wait_for_keyboard();  // see if all yellow
  uint8_t rotation = (tft.getRotation() + 1) & 0x3;
  tft.setRotation(rotation);

  Serial.printf("Rotation: %d After W: %d H: %d\n", tft.getRotation(), tft.width(), tft.height());
  tft.fillScreen(YELLOW);
  //wait_for_keyboard();  // see if all yellow
  drawTestScreen();
  //tft.textRotate(false);
  Serial.printf("MACR and End: %x\n", tft.lcdRegDataRead(RA8876_MACR));

}
void wait_for_keyboard() {
  Serial.println("Wait for any key to continue");
  while (Serial.read() == -1);
  while (Serial.read() != -1);
}