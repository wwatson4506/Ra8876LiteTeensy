/*
  Memory Transfer RA8876

  M Sandercock May 15 2020

  Test memory transfer to send a picture to the RA8876 screen, then use BTE memory copy to move it around

  Most of the time you would want to just put an image onto the screen, so use the library putPicture() function.

  Another common operation is "chroma key" that allows one (and only one) color in the source
  to be designated as transparent. Chroma key can not be combined with a ROP operation.

  Window Alpha is a more natural method of blending two images together, like for a fade effect.

  The "Raster Operation" ROP codes cover all 16 possible ways of combining individual pixels in binary.
  For example, you can have a binary-AND or binary-OR operation from two sources.
  One source might be the destination: if you want to lighten an image already on the screen.
  Or we just ignore the second source and put one source into the destination.
  Some codes are not useful: code 0 and 15 are just black and white and BTE is slightly slower than
  using the drawing functions to make a black or white rectangle.

  Explanation of ROP codes:
  ROP Number  Operation Description
  0           0         Pure black
  1           ~(S0+S1)  OR both (makes a bright image) then invert (image mostly dark)
  2           ~S0.S1    Invert source 0 then AND with source 1. Where S1 is white, you see the pure inverse of S0
  3           ~S0       Invert source 0. White becomes black, blue becomes orange, green becomes magenta...
  4           S0.~S1    AND source 0 with the inverse of source 1. Where S1 is black, you see S0
  5           ~S1       Ignores source 0, inverts source 1.
  6           S0^S1     XOR. Anything on white makes black. Anything on black is normal. This operation is special because it's reversible.
  7           ~(S0.S1)  AND both (makes darker image) then invert (output is mostly bright)
  8           S0.S1     AND. Where S1 is white, you see S0 unchanged. Generally makes the entire image darker.
  9           ~(S0^S1)  Inverse of XOR. Where S1 was white you see S0 unchanged
  10          S1        Ignores S0. Note that early tests show this may be significantly faster than its pair operation #12.
  11          ~S0+S1    S1 ORed with the inverse of S0.
  12          S0        Ignores S1. Simply copies S0 to the output.
  13          S0+~S1    S0 ORed wit the inverse of S1
  14          S0+S1     OR both. Generally makes entire image lighter as everything gets pushed closer to full white.
  15          1         Pure white.

  Using 16-bit data instead of 8-bit doesn't double the speed. It turns out that 16 bit is slower
  because the built-in byte-reversal SPI transfer is not yet available on the Teensy 4.x

  If SPI DMA is available, it will be used, so putPicture() and the other operations sending lots of SPI data will return very quickly.
  You can send more instructions to the LCD immediately and they will wait for the DMA to finish.
*/
#include "Cube_172.h"

//#define use_spi
#if defined(use_spi)
#include <SPI.h>
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif
#include <math.h>

#if defined(use_spi)
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t41_p tft = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif



void writeImage(int x, int y, int w, int h, const unsigned char *image) {
  //copy from the PROGMEM array to the screen, at the specified x/y location
  //This code is identical to what's inside tft.putPicture()
  
  //Sending bytes individually, in normal byte order
  if(tft.getBusWidth() == 8) {
    tft.bteMpuWriteWithROPData8(tft.currentPage, tft.width(), x, y,  //Source 1 is ignored for now
                                tft.currentPage, tft.width(), x, y, w, h,     //destination address, pagewidth, x/y, width/height
                                RA8876_BTE_ROP_CODE_12,
                                image);
  } else {
    tft.bteMpuWriteWithROPData16(tft.currentPage, tft.width(), x, y,  //Source 1 is ignored for now
                                 tft.currentPage, tft.width(), x, y, w, h,     //destination address, pagewidth, x/y, width/height
                                 RA8876_BTE_ROP_CODE_12,
                                 (uint16_t *)image);
  }	  
}

void writeImage16(int x, int y, int w, int h, const unsigned char *image) {
  //copy from the PROGMEM array to the screen, at the specified x/y location

  //Cast the data array pointer to insist that it contains 16-bit unsigned integers
  //The only benefit of this is if you already had your data in byte-reversed 16-bit form.
  //It's actually slower than the 8 bit version.
  if(tft.getBusWidth() == 16) {
    tft.bteMpuWriteWithROPData16(tft.currentPage, tft.width(), x, y,  //Source 1 is ignored for now
                                 tft.currentPage, tft.width(), x, y, w, h,     //destination address, pagewidth, x/y, width/height
                                 RA8876_BTE_ROP_CODE_12,
                                 (uint16_t *)image);
  } else {
    tft.bteMpuWriteWithROPData8(tft.currentPage, tft.width(), x, y,  //Source 1 is ignored for now
                                tft.currentPage, tft.width(), x, y, w, h,     //destination address, pagewidth, x/y, width/height
                                RA8876_BTE_ROP_CODE_12,
                                image);
  }
}

void writeImageChromakey(int x, int y, int w, int h, ru16 chromakeyColor, const unsigned char *image) {
  //copy from the PROGMEM array to the screen, at the specified x/y location with one color transparent
  if(tft.getBusWidth() == 16) {
    tft.bteMpuWriteWithChromaKeyData16(//no source 1 for this operation
      tft.currentPage, tft.width(), x, y, w, h,     //destination address, x/y, width/height
      chromakeyColor,
      (uint16_t *)image);
  } else {
    tft.bteMpuWriteWithChromaKeyData8(//no source 1 for this operation
      tft.currentPage, tft.width(), x, y, w, h,     //destination address, x/y, width/height
      chromakeyColor,
      image);
  }
}

void copyImageROP(int x, int y, int w, int h, uint8_t rop, const unsigned char *image) {
  //copy from existing area on screen - you will see this is much faster than loading data over SPI

  tft.bteMemoryCopyWithROP(tft.currentPage, tft.width(), 20, 5, //source 0 hardcoded to come from the top-left square, which is the "normal"
                           tft.currentPage, tft.width(), 20 + 4 * (20 + IMG_WIDTH), 5 + 2 * (24 + IMG_HEIGHT), //source 1 (I'm calling it background) is hardcoded to the bottom-right square
                           tft.currentPage, tft.width(), x, y, w, h,     //destination address, x/y, width/height
                           rop);
}

void drawBG(int x, int y, int w, int h) {
  //Draw a simple geometric background to overlay the different ROP effects
  //Do the timing printout only once, to compare the time taken for a rectangle fill
  unsigned long startTime = micros();
  tft.fillRect(x, y, w, h, BLACK);
  tft.check2dBusy();
  unsigned long endTime = micros();
  static bool toPrint = true;
  if (toPrint) {
    Serial.print("fillRect operation takes ");
    Serial.print((float)(endTime - startTime) / 1000.0, 3);
    Serial.println(" microseconds to fill the image area.");
    toPrint = false;
  }
  tft.fillRect(x + 3, y + 3, w - 6, h / 3, WHITE);
  tft.fillRect(x + 3, y + 3 + h / 3, w - 6, h / 3, SKYBLUE);
  tft.fillCircle(x + w / 2, y + h / 2, h / 4, CRIMSON);
  tft.fillTriangle(x + 2 * w / 8, y + 3 * h / 8, x + 1 * w / 8, y + 5 * h / 8, x + 3 * w / 8, y + 5 * h / 8, DARKGREEN);
  tft.fillRect(x + 5 * w / 8, y + 3 * h / 8, w / 4, h / 4, DARKYELLOW);
}

//The function below is not called in the default code, but it is useful to demonstrate the "write data yourself" ability...
void writeImageChromakeyZoom(int x, int y, int w, int h, ru16 chromakeyColor, int magnification, const unsigned char *image) {
  //copy from the PROGMEM array to the screen, at the specified x/y location
  //zooms in on the image by 2X by simply repeating each pixel 2^2 times, 3X uses 3^2 repeats
  //magnification should be a small integer like 2 or 3.

  //I'm looking for incorrect chromakey pixels, so I want to put this on a plain background
  tft.fillRect(x, y, w * magnification, h * magnification, MAGENTA);

  tft.bteMpuWriteWithChromaKey(//no source 1 for this operation
    tft.currentPage, tft.width(), x, y,           //destination address, x/y
    w * magnification, h * magnification,         //width/height of output
    chromakeyColor);

  for (int sourceRow = 0; sourceRow < h; sourceRow++) {
    for (int destRow = 0; destRow < magnification; destRow++) {
      for (int sourceCol = 0; sourceCol < w; sourceCol++) {
        for (int destCol = 0; destCol < magnification; destCol++) {
          //Remember each pixel is 2 bytes
          tft.lcdDataWrite(image[sourceRow * w * 2 + sourceCol * 2]);
          tft.lcdDataWrite(image[sourceRow * w * 2 + sourceCol * 2 + 1]);
        }
      }
    }
  }
}

void setup() {
  unsigned long startTime, endTime, end2Time;
  while (!Serial && millis() < 5000) {} //wait for Serial Monitor

  Serial.printf("%cLCD Memory Transfer test starting!",12);
  Serial.print("Compiled ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.println(__TIME__);

  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
  pinMode(BACKLITE, OUTPUT);
//  analogWriteFrequency(BACKLITE, 1000000);
  digitalWrite(BACKLITE, HIGH);
//  analogWrite(BACKLITE, 256);

#if defined(use_spi)
  tft.begin(); 
#else
  tft.begin(20);// 20 is working in 8bit and 16bit mode on T41
#endif

  tft.fillScreen(DARKBLUE);
  tft.setFontSize(1, false);

  //draw some background images, to try out different ROPs
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 5; i++) {
      drawBG(20 + i * (20 + IMG_WIDTH), 5 + j * (24 + IMG_HEIGHT), IMG_HEIGHT, IMG_WIDTH);
    }
  }
  tft.setCursor(20 + 4 * (20 + IMG_WIDTH), 5 + 2 * (24 + IMG_HEIGHT) + IMG_HEIGHT);
  tft.setTextColor(WHITE, DARKBLUE);
  tft.print("Background");

  //This example sends the image three times, so you can see the
  //different trade-offs using either put-picture, 8-bit or 16-bit transfers...

  startTime = micros();
  tft.putPicture(20, 5, IMG_WIDTH, IMG_HEIGHT, image_565);  //basic send, using 8-bit data
  endTime = micros();
  tft.setCursor(20 , 5 + IMG_HEIGHT);
  tft.setTextColor(WHITE, DARKBLUE);
  tft.print("Normal");
  end2Time = micros();
  Serial.print("Put-picture from PROGMEM to display took ");
  Serial.print((float)(endTime - startTime) / 1000.0, 3);
  Serial.print("us to begin the operation.\n  But the next LCD operation was delayed by ");
  Serial.print((float)(end2Time - endTime) / 1000.0, 3);
  Serial.println("us because data transfer was still underway");

  if(tft.getBusWidth() == 16) {
    startTime = micros();
    writeImage16(20, 5, IMG_WIDTH, IMG_HEIGHT, image_565);  //basic send, using 16-bit byte-swapped data
    endTime = micros();
    Serial.print("16-bit copy from PROGMEM to display took ");
    Serial.print((float)(end2Time - startTime) / 1000.0, 3);
    Serial.println("us to begin the transfer (data is on its way while you read this.)");
  } else {
    startTime = micros();
    writeImage(20, 5, IMG_WIDTH, IMG_HEIGHT, image_565);  //Duplicate of basic send
    endTime = micros();
    Serial.print("8-bit Copy from PROGMEM to display took ");
    Serial.print((float)(endTime - startTime) / 1000.0, 3);
    Serial.println("us to begin the transfer (data is on its way while you read this.)");
  }

  //Chromakey can also be done as 16-bit or 8-bit but the time taken is identical to the normal write
  //It's actually the same operation underneath, just with the background color set to the chromakey
  startTime = micros();
  writeImageChromakey(20 + 1 * (IMG_WIDTH + 20), 5, IMG_WIDTH, IMG_HEIGHT, 0xffdf, image_565);
  end2Time = micros();
  tft.setCursor(20 + 1 * (IMG_WIDTH + 20), 5 + IMG_HEIGHT);
  tft.setTextColor(WHITE, DARKBLUE);
  tft.print("Chromakey");
  Serial.print("Chromakey copy from PROGMEM to display took ");
  Serial.print((float)(end2Time - startTime) / 1000.0, 3);
  Serial.println("us to run to completion.");

  //Now run through all the ROP options to see what they look like...

  uint8_t rop = 15;
  int i = 2;
  int j = 0;
  do {
    startTime = micros();
    copyImageROP(20 + i * (IMG_WIDTH + 20), 5 + j * (24 + IMG_HEIGHT), IMG_WIDTH, IMG_HEIGHT, rop, image_565);
    endTime = micros();

    //at this point, we can keep working but the BTE operation is ongoing, inside the RAiO chip
    tft.check2dBusy(); //wait until chip is not busy
    unsigned long end2 = micros();

    tft.setCursor(20 + i * (IMG_WIDTH + 20), 5 + IMG_HEIGHT + j * (24 + IMG_HEIGHT));
    tft.setTextColor(WHITE, DARKBLUE);
    tft.print("ROP ");
    tft.print(rop);

    Serial.print("ROP ");
    Serial.print(rop);
    Serial.print("  BTE copy took ");
    Serial.print((float)(endTime - startTime), 3);
//    Serial.print((float)(endTime - startTime) / 1000.0, 3);
    Serial.print("us, followed by ");
    Serial.print((float)(end2 - endTime), 3);
//    Serial.print((float)(end2 - endTime) / 1000.0, 3);
    Serial.println("us internal processing in the RAiO chip.");

    //Some of the ROP operations are not necessary to display
    //  so we will "skip" them by not moving the graphics pointer forwards to the next square
    //But the Serial output will show you how long they took,
    //  since you're probably most interested in the speed of memory-to-memory ROP10 or ROP12.
    switch (rop) {
      case 0:
      case 15:
        //ROP 0 is plain black
        //ROP 15 (plain white) is the same speed and equally useless
        break;
      case 10:
        //leaves secound source unchanged
        break;
      case 12:
        //we already saw this operation in the normal write for the top-left square
        break;
      default:
        //move on to the next space for display
        i++;
        if (i >= 5) {
          i = 0;
          j++;
        }
    }
    rop++;
    if(rop>15) rop = 0;
  } while (rop != 15 && j < 3);

  //If you need to examine your chromakey zoomed-in, looking for errant pixels, try this...
  //writeImageChromakeyZoom(20 + 2*(IMG_WIDTH + 20), 5, IMG_WIDTH, IMG_HEIGHT, 0xffdf, 3, image_565);

  Serial.println("\n\nFirst Page Finished, PRESS ANY KEY...");
  while (Serial.available() > 0) {
    Serial.read(); //clear input buffer
  }
  while (Serial.available() == 0) {
    //wait forever for any key press on Serial Monitor
  }
  Serial.println("Test Alpha...");

  //Now I want to try some different usage of the BTE functions but I've run out of space on this page
  //Jump to 2nd page, but we'll be using the data (image and background) off the first page...

  tft.selectScreen(SCREEN_2);
  tft.fillScreen(DARKGREEN);
  tft.setCursor(400, 180);
  tft.setTextColor(WHITE, DARKGREEN);
  tft.printf(" Test Alpha...");
}

void loop() {
  static uint8_t alpha = 0;
  static int8_t increment = 1;

  alpha += increment;

  tft.setCursor(400, 200 + IMG_HEIGHT);
  tft.setTextColor(WHITE, DARKGREEN);
  tft.print(alpha);
  tft.print("  ");

  tft.bteMemoryCopyWindowAlpha(SCREEN_1, tft.width(), 20, 5,  //source 0, our cube image
                               SCREEN_1, tft.width(), 20 + 4 * (20 + IMG_WIDTH), 5 + 2 * (24 + IMG_HEIGHT), //source 1 (I'm calling it background) from the bottom-right square
                               tft.currentPage, tft.width(), 400, 200, IMG_WIDTH, IMG_HEIGHT,     //destination address, x/y, width/height
                               alpha);

  delay(100);

  if (alpha >= 32) {
    increment = -1;
    delay(800);
  }
  if (alpha == 0) {
    increment = 1;
    delay(800);
  }
}

void waitforInput()
{
  Serial.println("Press anykey to continue");
  while (Serial.read() == -1) ;
  while (Serial.read() != -1) ;
}
