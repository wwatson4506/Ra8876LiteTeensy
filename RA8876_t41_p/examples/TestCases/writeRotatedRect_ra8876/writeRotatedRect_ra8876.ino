// Set which Display we are using and at what speed
// Currently I have options for both MICROMOD and T42 to make it
// easier for testing

#include "Arduino.h"

//#define use_spi
#if defined(use_spi)
#include <SPI.h>
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif
#include "font_Arial.h"
#include "flexio_teensy_mm.c"
#include "T41_top_card.c"

#if defined(use_spi)
#define TFT_CS 10
#define TFT_RST 9
RA8876_t3 tft = RA8876_t3(TFT_CS, TFT_RST);
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
RA8876_t41_p tft = RA8876_t41_p(dc, cs, rst);  //(dc, cs, rst)
#endif

void setup() {
  Serial.begin(115200);
  delay(1000);
  if (CrashReport) {
    Serial.print(CrashReport);
  }

#if defined(use_spi)
  tft.begin();
#else
  tft.begin(20);                               // 20 is working in 8bit and 16bit mode on T41
#endif
  tft.graphicMode(true);
  tft.setTextCursor(0, 0);
  tft.setFont(Arial_14);


  for (uint8_t rotation = 0; rotation < 4; rotation++) {
    tft.setRotation(rotation);
    displayRotation(rotation);
    delay(1000);
    tft.fillScreen(BLACK);
  }
  tft.setRotation(0);
}

void loop() {
  for (uint8_t rotation = 0; rotation < 4; rotation++) {
    tft.setRotation(rotation);
    displayRotation(rotation);
    nextPage();

    uint16_t *rotatedImage = tft.rotateImageRect(480, 320, flexio_teensy_mm, rotation);
    tft.writeRotatedRect(CENTER, CENTER, 480, 320, rotatedImage);  // 480x320
    free(rotatedImage);
    nextPage();
    rotatedImage = tft.rotateImageRect(400, 272, td_t4_top, rotation);
    tft.writeRotatedRect(CENTER, CENTER, 400, 272, rotatedImage);  // 480x320
    free(rotatedImage);
    nextPage();

    tft.writeRect(CENTER, CENTER, 480, 320, flexio_teensy_mm);  // 480x320
    nextPage();

    draw_8bit_bmp(rotation);
    nextPage();
  }
}

void nextPage() {
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1)
    ;
  while (Serial.read() != -1)
    ;

  tft.fillScreen(BLACK);
}

void displayRotation(uint8_t rotation) {
  tft.fillScreen(RED);
  tft.setCursor(tft.width() / 2, tft.height() / 2, true);
  tft.printf("Rotation: %d", rotation);
  Serial.printf("WIDTH: %d, HEIGHT: %d\n", tft.width(), tft.height());
  tft.setCursor(200, 300);
  Serial.printf("  Set cursor(200, 300), retrieved(%d %d)",
                tft.getCursorX(), tft.getCursorY());
  tft.setCursor(50, 50);
  tft.print("0");
  tft.setCursor(tft.width() - 50, 50);
  tft.print("1");
  tft.setCursor(50, tft.height() - 50);
  tft.print("2");
  tft.setCursor(tft.width() - 50, tft.height() - 50);
  tft.print("3");
}

void draw_8bit_bmp(uint8_t rotation) {
  uint16_t palette[] = { RED, GREEN, BLUE, YELLOW, CYAN, WHITE, BLACK, MAGENTA };

#define BAND_WIDTH 200
#define BAND_HEIGHT 50
#define BANDS (sizeof(palette) / sizeof(palette[0]))
  uint8_t *buffer = (uint8_t *)malloc(BAND_WIDTH * BAND_HEIGHT * BANDS);
  if (buffer == nullptr) return;

  for (uint8_t i = 0; i < BANDS; i++) {
    memset(buffer + (i * BAND_WIDTH * BAND_HEIGHT), i, (BAND_WIDTH * BAND_HEIGHT));
  }
  tft.writeRect8BPP(CENTER, CENTER, BAND_WIDTH, BAND_HEIGHT * BANDS, buffer, palette);

// Second one to make sure X's and Ys are OK
#define BAND_W2 100
#define BAND_H2 50
  uint8_t *pb = buffer;
  int x;
  for (uint8_t i = 0; i < BANDS; i += 2) {
    for (uint8_t j = 0; j < BAND_H2; j++) {
      for (x = 0; x < BAND_W2 / 2; x++) *pb++ = i;
      for (; x < BAND_W2; x++) *pb++ = i + 1;
    }
  }
  tft.writeRect8BPP(50, 50, BAND_W2, BAND_H2 * BANDS / 2, buffer, palette);

  // Try 4 bit
  pb = buffer;
  for (uint8_t i = 0; i < 4; i += 2) {
    for (uint8_t j = 0; j < BAND_H2; j++) {
      for (x = 0; x < BAND_W2 / 2; x += 2) *pb++ = (i << 4) | i;
      for (; x < BAND_W2; x += 2) *pb++ = ((i + 1) << 4) | (i + 1);
    }
  }

  tft.writeRect4BPP(tft.width() - (50 + BAND_W2), tft.height() - (50 + BAND_H2 * 4 / 2), BAND_W2, BAND_H2 * 4 / 2, buffer, palette);

  // Lets put a White rectangle as a marker at 20, 20
  tft.fillRect(10, 10, 20, 20, WHITE);

  uint16_t *pb16 = (uint16_t *)buffer;
  for (int i = 0; i < 400; i++) *pb16++ = RED;
  tft.writeRect(tft.width() - 30, tft.height() - 30, 20, 20, (uint16_t *)buffer);


  free(buffer);
}
