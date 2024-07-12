//-------------------------------------------------------------------
// Kurt's Frame buffer and clip tests
//
// This test program is a set of random tests that have been done
// over time to test out the different functions to make sure they
// are working with the clipping functions as well as the frame
// buffer.  So you can for example test to see if you get the
// same results with the frame buffer turned on or off
//
// this sketch is in the public domain.
//
// This sketch depends on the fonts that are contained in the library
//     https://github.com/mjs513/ILI9341_fonts
//-------------------------------------------------------------------
// Set which Display we are using and at what speed
// Currently I have options for both MICROMOD and T42 to make it
// easier for testing

//#define use_spi

#include <MemoryHexDump.h>
#include <Adafruit_GFX.h>  // Core graphics library
#if defined(use_spi)
#include "SPI.h"
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif

//#include "RA8876_t41_p.h"
#include "font_Arial.h"
#include "font_ArialBold.h"
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeSerif12pt7b.h>

#define ROTATION 0
#define MAROON 0x7800      /* 128,   0,   0 */
#define PURPLE 0x780F      /* 128,   0, 128 */
#define OLIVE 0x7BE0       /* 128, 128,   0 */
#define LIGHTGREY 0xC618   /* 192, 192, 192 */
#define DARKGREY 0x7BEF    /* 128, 128, 128 */
#define ORANGE 0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5 /* 173, 255,  47 */
#define PINK 0xF81F


//Adafruit_GFX_Button button;

// Let's allocate the frame buffer ourself.
//DMAMEM uint16_t tft_frame_buffer[TFTWIDTH * TFTHEIGHT];

uint8_t use_dma = 0;
uint8_t use_clip_rect = 0;
uint8_t use_set_origin = 0;
uint8_t use_fb = 0;

#define ORIGIN_TEST_X 50
#define ORIGIN_TEST_Y 50


#if defined(use_spi)
#define TFT_CS 30
#define TFT_RST 28
#define TFT_BL 29
RA8876_t3 tft = RA8876_t3(TFT_CS, TFT_RST);
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
RA8876_t41_p tft = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif


void setup() {
    pinMode(2, OUTPUT);
    pinMode(3, OUTPUT);
    while (!Serial && (millis() < 4000))
        ;
    Serial.begin(115200);

    if (CrashReport) {
        Serial.print(CrashReport);
        WaitForUserInput();
    }
    //Serial.printf("Begin: CS:%d, DC:%d, MOSI:%d, MISO: %d, SCK: %d, RST: %d\n", TFT_CS, TFT_DC, TFT_MOSI, TFT_MISO, TFT_SCK, TFT_RST);
    Serial.println("\n*** Sketch Startup ***");
#ifdef TFT_TOUCH_CS
    pinMode(TFT_TOUCH_CS, OUTPUT);
    digitalWrite(TFT_TOUCH_CS, HIGH);
#endif

/*
   * begin(Dispalay type, baud)
   * Display type is associated with the the diplay
   * init configurations:
   * Baud can be as defined in the readme.
   * begin defaults to ILI9488 and 20Mhz:
   *     lcd.begin();
  */
#if defined(TFT_BL)
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
#endif

#if defined(use_spi)
    tft.begin();
    tft.setBusWidth(8);
#else
    tft.begin(20);
#endif

    //  tft.setFrameBuffer(tft_frame_buffer);
    tft.backlight(true);

    tft.setRotation(ROTATION);
    tft.fillScreen(BLACK);

    delay(500);
    tft.fillScreen(RED);
    delay(500);
    tft.fillScreen(GREEN);
    delay(500);
    tft.fillScreen(BLUE);

    //
    //  button.initButton(&tft, 200, 125, 100, 40, GREEN, YELLOW, RED, "UP", 1, 1);

    drawTestScreen();

}

void SetupOrClearClipRectAndOffsets() {
    if (use_clip_rect) {
        tft.setClipRect();  // make sure we clear the whole screen
        tft.setOrigin();    // make sure none are set yet

        tft.fillScreen(LIGHTGREY);

        // Now lets set origin.
        if (use_set_origin)
            tft.setOrigin(ORIGIN_TEST_X, ORIGIN_TEST_Y);
        int x = tft.width() / 4;
        int y = tft.height() / 4;
        int w = tft.width() / 2;
        int h = tft.height() / 2;
        tft.drawRect(x, y, w, h, ORANGE);
        if(use_fb) tft.updateScreen();
        tft.setClipRect(x + 1, y + 1, w - 2, h - 2);
        delay(250);

    } else {
        tft.setClipRect();
        if (use_set_origin)
            tft.setOrigin(ORIGIN_TEST_X, ORIGIN_TEST_Y);
        else
            tft.setOrigin();
    }
}


uint16_t palette[256];  // Should probably be 256, but I don't use many colors...
uint16_t pixel_data[2500];
const uint8_t pict1bpp[] = { 0xff, 0xff, 0xc0, 0x03, 0xa0, 0x05, 0x90, 0x9, 0x88, 0x11, 0x84, 0x21, 0x82, 0x41, 0x81, 0x81,
                             0x81, 0x81, 0x82, 0x41, 0x84, 0x21, 0x88, 0x11, 0x90, 0x09, 0xa0, 0x05, 0xc0, 0x03, 0xff, 0xff };
const uint8_t pict2bpp[] = {
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
    0xff,
    0xff,
    0x00,
    0x00,
    0x55,
    0x55,
    0xaa,
    0xaa,
};
const uint8_t pict4bpp[] = {
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x22,
    0x22,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x22,
    0x22,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x33,
    0x33,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x33,
    0x33,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x33,
    0x33,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x33,
    0x33,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x22,
    0x22,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x22,
    0x22,
    0x22,
    0x22,
    0x11,
    0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x00,
    0x00,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x11,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};



void drawTestScreen() {
    Serial.printf("Use FB: %d ", use_fb);
    Serial.flush();
    //tft.useFrameBuffer(use_fb);
    tft.useCanvas(use_fb);
    SetupOrClearClipRectAndOffsets();
    uint32_t start_time = millis();
    tft.fillScreen(use_fb ? RED : BLACK);
    //tft.setFont(Inconsolata_60);
    tft.setFont(Arial_24_Bold);
    tft.setTextColor(WHITE);
    tft.setCursor(0, 0);
    tft.println("Test");
    tft.setTextColor(WHITE, RED);
    tft.println("text");
    tft.setCursor(85, 65);
    tft.print("XYZ");
#if 1
    tft.setFontAdafruit();
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.println("01234");
    tft.setTextColor(WHITE, GREEN);
    tft.println("56789!@#$%");
#endif
    tft.drawRect(0, 150, 100, 50, WHITE);
    tft.drawLine(0, 150, 100, 50, GREEN);
    tft.fillRectVGradient(125, 150, 50, 50, GREEN, YELLOW);
    tft.fillRectHGradient(200, 150, 50, 50, YELLOW, GREEN);

// Try a read rect and write rect
#define BAND_WIDTH 8
#define BAND_HEIGHT 20
#define BAND_START_X 200
#define BAND_START_Y 259
    uint16_t band_colors[] = { RED, GREEN, PINK, DARKGREY, WHITE, YELLOW, CYAN, BLUE };
#define COUNT_BANDS (sizeof(band_colors) / sizeof(band_colors[0]))
    Serial.println("Band colors: ");
    for (uint8_t i = 0; i < COUNT_BANDS; i++) {
        tft.fillRect(BAND_START_X + BAND_WIDTH * i, BAND_START_Y, BAND_WIDTH, BAND_HEIGHT, band_colors[i]);
        Serial.printf(" %x",  band_colors[i]);
    }
    Serial.println();
    memset(pixel_data, 0, sizeof(pixel_data));
    tft.readRect(BAND_START_X, BAND_START_Y, BAND_WIDTH * 8, BAND_HEIGHT, pixel_data);
    Serial.printf("%04X %04X %04X %04X %04X %04X %04X %04X\n",
                  RED, GREEN, BLUE, BLACK, WHITE, YELLOW, CYAN, PINK);
    MemoryHexDump(Serial, pixel_data, BAND_WIDTH * 8 * 2, true, "\nColor bars:\n");

    tft.writeRect(BAND_START_X, BAND_START_Y + BAND_HEIGHT, BAND_WIDTH * 8, BAND_HEIGHT, pixel_data);
    //WaitForUserInput();

    tft.readRect(0, 0, 50, 50, pixel_data);
    //    MemoryHexDump(Serial, pixel_data, 1024, true);
    // For heck of it lets make sure readPixel and ReadRect
    // give us same data, maybe check along diagnal?
    for (uint i = 0; i < 50; i++) {
        uint16_t pixel_color = tft.readPixel(i, i);
        if (pixel_color != pixel_data[i * 50 + i]) {
            Serial.printf("Read rect/pixel mismatch: %d %x %x\n", i, pixel_color, pixel_data[i * 50 + i]);
        }
    }

#ifdef DEBUG_PIN
    digitalWrite(DEBUG_PIN, LOW);
#endif
    tft.writeRect(250, 0, 50, 50, pixel_data);

    // Lets try to pack this rectangle of data into 8 byte
    tft.readRect(85, 65, 50, 50, pixel_data);
    uint16_t *ppd16 = pixel_data;
    uint8_t *ppd8 = (uint8_t *)pixel_data;
    uint8_t palette_cnt = 0;
    int palette_index;
    for (int i = 0; i < 2500; i++) {
        for (palette_index = 0; palette_index < palette_cnt; palette_index++) {
            if (*ppd16 == palette[palette_index])
                break;
        }
        if (palette_index >= palette_cnt) {
            palette[palette_cnt++] = *ppd16;  // save away the color
        }
        *ppd8++ = palette_index;
        ppd16++;
    }
    tft.writeRect8BPP(200, 50, 50, 50, (uint8_t *)pixel_data, palette);

    palette[0] = RED;
    palette[1] = GREEN;
    palette[2] = BLUE;
    palette[3] = YELLOW;
    uint8_t *pb = (uint8_t *)pixel_data;

#define MEMSET_CNT 1000
    memset(pb + MEMSET_CNT * 0, 0, MEMSET_CNT);
    memset(pb + MEMSET_CNT * 1, 1, MEMSET_CNT);
    memset(pb + MEMSET_CNT * 2, 2, MEMSET_CNT);
    memset(pb + MEMSET_CNT * 3, 3, MEMSET_CNT);
    tft.writeRect8BPP(100, 400, 100, (4 * MEMSET_CNT) / 100, pb, palette);


    tft.writeRect8BPP(250, 400, 40, 25,  pb, palette);


    for (int i = 0; i < 1000; i++) pixel_data[i] = BLUE;
    tft.writeRect(400, 400, 40, 25,  pixel_data);




    tft.writeRect1BPP(75, 100, 16, 16, pict1bpp, palette);
    tft.writeRect1BPP(320 - 90, 75, 16, 16, pict1bpp, palette);

    tft.writeRect2BPP(75, 125, 32, 16, pict2bpp, palette);

    tft.writeRectNBPP(15, 125, 32, 16, 2, pict2bpp, palette);
    tft.writeRectNBPP(75, 150, 16, 16, 4, pict4bpp, palette);

    // Try drawing button
    //tft.setFontAdafruit();
    //button.drawButton();
    // Lets fill up some more of the larger screen.

    tft.fillCircle(380, 220, 80, GREEN);
    tft.fillCircle(380, 220, 60, BLUE);
    tft.drawCircle(380, 220, 40, PINK);
    tft.drawCircle(380, 220, 20, YELLOW);

    tft.fillTriangle(20, 300, 170, 300, 95, 240, GREEN);
    tft.fillTriangle(40, 280, 150, 280, 95, 220, PINK);
    tft.drawTriangle(60, 260, 130, 260, 95, 200, YELLOW);
    tft.drawTriangle(80, 240, 110, 240, 95, 180, BLUE);

    tft.setFont(&FreeMonoBoldOblique12pt7b);
    tft.setCursor(250, 50);
    tft.setTextColor(WHITE);
    tft.println("Adafruit");
    tft.setCursor(250, tft.getCursorY());
    tft.setTextColor(WHITE, GREEN);
    tft.println("MonoBold");


//    if (use_dma) {
//        tft.updateScreenAsync();
//    } else {
      if(use_fb)
        tft.updateScreen();
//    }

    Serial.println(millis() - start_time, DEC);


    use_fb = use_fb ? 0 : 1;
    Serial.println(use_fb, DEC);
}

void fillScreenTest() {
    //tft.useFrameBuffer(;0);
    SetupOrClearClipRectAndOffsets();

    tft.fillScreen(RED);
    WaitForUserInput();
    tft.fillScreen(GREEN);
    WaitForUserInput();
    tft.fillScreen(WHITE);
    WaitForUserInput();
    tft.fillScreen(BLACK);
}
void printTextSizes(const char *sz) {
    Serial.printf("%s(%d,%d): SPL:%u ", sz, tft.getCursorX(), tft.getCursorY(), tft.strPixelLen(sz));
    int16_t x, y;
    uint16_t w, h;
    tft.getTextBounds(sz, tft.getCursorX(), tft.getCursorY(), &x, &y, &w, &h);
    Serial.printf(" Rect(%d, %d, %u %u)\n", x, y, w, h);
    tft.drawRect(x, y, w, h, GREEN);
}


void drawTextScreen(bool fOpaque) {
    SetupOrClearClipRectAndOffsets();
    tft.setTextSize(1);
    uint32_t start_time = millis();
    //tft.useFrameBuffer(use_fb);
    tft.useCanvas(use_fb);
    tft.fillScreen(use_fb ? RED : BLACK);
    tft.setFont(Arial_28_Bold);
    //t  tft.setFont(Arial_40_Bold);
    if (fOpaque)
        tft.setTextColor(WHITE, use_fb ? BLACK : RED);
    else
        tft.setTextColor(WHITE);
    tft.setCursor(0, 5);
    tft.println("AbCdEfGhIj");
#if 1
    tft.setFont(Arial_28_Bold);
    tft.println("0123456789!@#$");
    tft.setFont(Arial_20_Bold);
    tft.println("abcdefghijklmnopq");
    tft.setFont(Arial_14_Bold);
    tft.println("ABCDEFGHIJKLMNOPQRST");
    tft.setFont(Arial_10_Bold);
    tft.println("0123456789zyxwvutu");
#endif
    tft.setFont(&FreeMonoBoldOblique12pt7b);
    printTextSizes("AdaFruit_MB_12");
    if (fOpaque) {
        tft.setTextColor(RED, BLUE);
        tft.print("A");
        tft.setTextColor(WHITE, GREEN);
        tft.print("d");
        tft.setTextColor(RED, BLUE);
        tft.print("a");
        tft.setTextColor(WHITE, GREEN);
        tft.print("F");
        tft.setTextColor(RED, BLUE);
        tft.print("R");
        tft.setTextColor(WHITE, GREEN);
        tft.print("u");
        tft.setTextColor(RED, BLUE);
        tft.print("i");
        tft.setTextColor(WHITE, GREEN);
        tft.print("t");
        tft.setTextColor(RED, BLUE);
        tft.print("_");
        tft.setTextColor(WHITE, GREEN);
        tft.print("M");
        tft.setTextColor(RED, BLUE);
        tft.print("B");
        tft.setTextColor(WHITE, GREEN);
        tft.print("_");
        tft.setTextColor(RED, BLUE);
        tft.print("1");
        tft.setTextColor(WHITE, GREEN);
        tft.println("2");
        tft.setTextColor(WHITE, use_fb ? BLACK : RED);
    } else tft.println("AdaFruit_MB_12");
    tft.setFont(&FreeSerif12pt7b);
    printTextSizes("FreeSan12");
    tft.println("FreeSan12");
    tft.println();
    tft.setTextSize(1, 3);
    printTextSizes("Size 1,3");
    tft.println("Size 1,3");
    tft.setFont();
    tft.setCursor(0, 190);
    tft.setTextSize(1, 2);
    printTextSizes("Sys(1,2)");
    tft.println("Sys(1,2)");
    tft.setTextSize(1);
    printTextSizes("System");
    tft.println("System");
    tft.setTextSize(1);


    if(use_fb) tft.updateScreen();
    Serial.printf("Use FB: %d OP: %d, DT: %d OR: %d\n", use_fb, fOpaque, use_set_origin, millis() - start_time);
}


void drawGFXTextScreen(bool fOpaque) {
    SetupOrClearClipRectAndOffsets();
    tft.setTextSize(1);
    tft.setCursor(0, 10);
    if (fOpaque)
        tft.setTextColor(WHITE, use_fb ? BLACK : RED);
    else
        tft.setTextColor(WHITE);
    uint32_t start_time = millis();
    //tft.useFrameBuffer(use_fb);
    tft.useCanvas(use_fb);
    tft.fillScreen(use_fb ? RED : BLACK);
    tft.setFont(&FreeMonoBoldOblique12pt7b);
    tft.println("MonoBold");
    tft.println("ABCDEFGHIJKLMNO");
    tft.println("abcdefghijklmno");
    tft.println("0123456789!@#$%^&*()_");
    tft.setFont(&FreeSerif12pt7b);
    tft.println("Serif12");
    tft.println("ABCDEFGHIJKLMNO");
    tft.println("abcdefghijklmno");
    tft.println("0123456789!@#$%^&*()_");
    if (use_fb) tft.updateScreen();
    tft.setTextSize(1);
    tft.setFont();
    Serial.printf("Use FB: %d OP: %d, DT: %d\n", use_fb, fOpaque, millis() - start_time);
}
//=============================================================================
// Wait for user input
//=============================================================================
void WaitForUserInput() {
    Serial.println("Hit Enter to continue");
    Serial.flush();
    while (Serial.read() == -1)
        ;
    while (Serial.read() != -1)
        ;
}


void loop(void) {
    // See if any text entered
    int ich;
    if ((ich = Serial.read()) != -1) {
        while (Serial.read() != -1) delay(1);

        // See if We have a dma operation in progress?

        if (ich == 'c') {
            use_clip_rect = !use_clip_rect;
            if (use_clip_rect) Serial.println("Clip Rectangle Turned on");
            else Serial.println("Clip Rectangle turned off");
            return;
        }
        if (ich == 'd') {
            use_dma = !use_dma;
            if (use_dma) Serial.println("DMA Turned on");
            else Serial.println("DMA turned off");
            return;
        }

        if (ich == 's') {
            use_set_origin = !use_set_origin;
            if (use_set_origin) Serial.printf("Set origin to %d, %d\n", ORIGIN_TEST_X, ORIGIN_TEST_Y);
            else Serial.println("Clear origin");
            return;
        }
        if (ich == 'o')
            drawTextScreen(1);
        else if (ich == 'f')
            fillScreenTest();
        else if (ich == 't')
            drawTextScreen(0);
        else if (ich == 'g')
            drawGFXTextScreen(0);
        else
            drawTestScreen();
    }
}
