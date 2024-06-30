/*
  An example of the RA8876 screen with the capacitive touch screen that use FT5316
  Such as BuyDisplay ER-TFTM070-6
  The FT5206 is normally used with the RA8875 screens but it seems all the FT chips work the same...
  Get the RA8876_t3 library from: https://github.com/mjs513/Ra8876LiteTeensy/tree/RA8876-redo
  Get the FT5206 library from: https://github.com/sumotoy/FT5206

  Also demonstrates use of the "graphic cursor" mouse cursor and a simple animated cursor.
*/
#define use_spi
#if defined(use_spi)
#include <SPI.h>
#include <RA8876_t3.h>
#else
#include <RA8876_t41_p.h>
#endif

#if defined(use_spi)
#define RA8876_CS 10
#define RA8876_RESET 9
#define BACKLITE 7 //External backlight control connected to this Arduino pin
RA8876_t3 tft = RA8876_t3(RA8876_CS, RA8876_RESET); //Using standard SPI pins
#else
uint8_t dc = 13;
uint8_t cs = 11;
uint8_t rst = 12;
RA8876_t41_p tft = RA8876_t41_p(dc,cs,rst); //(dc, cs, rst)
#endif

#define CTP_INT           6    // Use an interrupt capable pin such as pin 2 (any pin on a Teensy)
#define MAXTOUCHLIMIT 1
uint16_t new_coordinates[5][2];
uint16_t old_coordinates[5][2];
uint8_t current_touches = 0;
uint8_t old_touches = 0;

int currentCursor;
int cursorOffsetX = 15;
int cursorOffsetY = 15;

//Custom cursor is a spinning circle which can be mirrored and rotated for an 8-frame animation cycle
PROGMEM unsigned char customCursor[256] = {
  0b10101010, 0b10101010, 0b10101010, 0b10010101, 0b01011010, 0b10101010, 0b10101010, 0b10101010,
  0b10101010, 0b10101010, 0b10010101, 0b01000000, 0b00011010, 0b10101010, 0b10101010, 0b10101010,
  0b10101010, 0b10101010, 0b01010000, 0b00000000, 0b00011010, 0b10101010, 0b10101010, 0b10101010,
  0b10101010, 0b10100101, 0b01000000, 0b00000000, 0b00011010, 0b10101010, 0b10101010, 0b10101010,
  0b10101010, 0b10010100, 0b00000000, 0b00000101, 0b01011010, 0b10101010, 0b10100110, 0b10101010,
  0b10101010, 0b01010000, 0b00000001, 0b01011010, 0b10101010, 0b10101010, 0b10010101, 0b10101010,
  0b10101001, 0b01000000, 0b00010101, 0b10101010, 0b10101010, 0b10101010, 0b01000001, 0b01101010,
  0b10101001, 0b00000000, 0b01011010, 0b10101010, 0b10101010, 0b10101001, 0b00000000, 0b01101010,
  0b10100101, 0b00000001, 0b01101010, 0b10101010, 0b10101010, 0b10101001, 0b01000000, 0b01011010,
  0b10010100, 0b00000110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b01010000, 0b00010110,
  0b10010000, 0b00000110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10010000, 0b00000110,
  0b10010000, 0b00010110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10010100, 0b00000110,
  0b10010000, 0b00011010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10100100, 0b00000110,
  0b01000000, 0b01011010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10100101, 0b00000001,
  0b01000000, 0b01101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101001, 0b00000001,
  0b01000000, 0b01101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101001, 0b00000001,
  0b01000000, 0b01101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101001, 0b00000001,
  0b01000000, 0b01101010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10101001, 0b00000001,
  0b01000000, 0b01011010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10100101, 0b00000001,
  0b10010000, 0b00011010, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10100100, 0b00000110,
  0b10010000, 0b00010110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10010100, 0b00000110,
  0b10010000, 0b00000110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b10010000, 0b00000110,
  0b10010100, 0b00000110, 0b10101010, 0b10101010, 0b10101010, 0b10101010, 0b01010000, 0b00010110,
  0b10100101, 0b00000001, 0b01101010, 0b10101010, 0b10101010, 0b10101001, 0b01000000, 0b01011010,
  0b10101001, 0b00000000, 0b01011010, 0b10101010, 0b10101010, 0b10100101, 0b00000000, 0b01101010,
  0b10101001, 0b01000000, 0b00010101, 0b10101010, 0b10101010, 0b01010100, 0b00000001, 0b01101010,
  0b10101010, 0b01010000, 0b00000001, 0b01011010, 0b10100101, 0b01000000, 0b00000101, 0b10101010,
  0b10101010, 0b10010100, 0b00000000, 0b00000101, 0b01010100, 0b00000000, 0b00010110, 0b10101010,
  0b10101010, 0b10100101, 0b01000000, 0b00000000, 0b00000000, 0b00000001, 0b01011010, 0b10101010,
  0b10101010, 0b10101010, 0b01010000, 0b00000000, 0b00000000, 0b00000101, 0b10101010, 0b10101010,
  0b10101010, 0b10101010, 0b10010101, 0b01000000, 0b00000001, 0b01010110, 0b10101010, 0b10101010,
  0b10101010, 0b10101010, 0b10101010, 0b10010101, 0b01010110, 0b10101010, 0b10101010, 0b10101010,

};


void setup()
{
  Serial.begin(38400);
  long unsigned debug_start = millis ();
  while (!Serial && ((millis () - debug_start) <= 500)) ;
  Serial.println("RA8876 touchscreen test starting!");
  Serial.print("Compiled ");
  Serial.print(__DATE__);
  Serial.print(" at ");
  Serial.println(__TIME__);

  //I'm guessing most copies of this display are using external PWM
  //backlight control instead of the internal RA8876 PWM.
  //Connect a Teensy pin to pin 14 on the display.
  //Can use analogWrite() but I suggest you increase the PWM frequency first so it doesn't sing.
#if defined(use_spi)
  pinMode(BACKLITE, OUTPUT);
  digitalWrite(BACKLITE, HIGH);
  tft.backlight(true);
#endif

#if defined(use_spi)
  tft.begin(); 
#else
  tft.begin(20);// 20 is working in 8bit and 16bit mode on T41
#endif
  tft.useCapINT(CTP_INT);//we use the capacitive chip Interrupt out!
  //the following set the max touches (max 5)
  //it can be placed inside loop but BEFORE touched()
  //to limit dinamically the touches (for example to 1)
  tft.setTouchLimit(MAXTOUCHLIMIT);
  tft.enableCapISR(true);//capacitive touch screen interrupt it's armed

  tft.setTextColor(0xFFFF, 0x0000);
  Serial.println("inited...");

  //begin method of RA8876_t3 will initialize the graphic cursor images from the default ones in the library
  //and select the arrow one (but does not enable the cursor as visible)

  //Test uploading our own custom cursor (will be animated later)
  tft.Upload_Graphic_Cursor(4, customCursor);
  tft.Select_Graphic_Cursor_4();
  currentCursor = 4;

  tft.Enable_Graphic_Cursor(); // Turn on selected graphic cursor.
  // You can play with the cursor colors in the following two lines.
  tft.Set_Graphic_Cursor_Color_1(0xff); // White forground Color. (8 bit RRRGGBB)
  tft.Set_Graphic_Cursor_Color_2(0x00); // Black outline Color. (8 bit RRRGGBB)
  tft.Graphic_Cursor_XY(tft.width() / 2 - cursorOffsetX, tft.height() / 2 - cursorOffsetY); // Center cursor on screen.

  tft.fillRect(0, 0, 215, 215, ORCHID);
  tft.fillRect(400, 0, 172, 172, BLUEVIOLET);
}



void loop()
{
  //cts.setTouchLimit(1);//from 1 to 5
  if (tft.touched()) { //if touched(true) detach isr
    //at this point we need to fill the FT5206 registers...
    tft.updateTS();//now we have the data inside library
    Serial.print(">> touches:");
    Serial.print(tft.getTouches());
    Serial.print(" | gesture:");
    Serial.print(tft.getGesture(), HEX);
    Serial.print(" | state:");
    Serial.print(tft.getTouchState(), HEX);
    uint16_t coordinates[MAXTOUCHLIMIT][2];//to hold coordinates
    tft.getTScoordinates(coordinates);//done
    //now coordinates has the x,y of all touches
    for (uint8_t i = 0; i <= tft.getTouches(); i++) {
      Serial.printf(" (%d,%d)", coordinates[i][0], coordinates[i][1]);
    }
    tft.enableCapISR();//rearm ISR if needed (touched(true))
    Serial.println();
  }
#if 0
  if (cts.touched()) {
    uint8_t i;
    uint16_t x, y;
    cts.getTSregisters(registers);
    current_touches = cts.getTScoordinates(new_coordinates, registers);
    if (current_touches < 1) return;

    for (i = 1; i <= old_touches; i++) { // remove previous touches on screen
      //tft.fillCircle(old_coordinates[i - 1][0], old_coordinates[i - 1][1], 70, BLACK);
    }

    Serial.print(current_touches);
    Serial.print(" touches: ");

    for (i = 1; i <= current_touches; i++) { // mark touches on screen
      //Touch panel is upside-down to the screen coordinates, so we must subtract
      x = SCREEN_WIDTH - new_coordinates[i - 1][0] - 1;
      y = SCREEN_HEIGHT - new_coordinates[i - 1][1] - 1;

      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print("  ");

      // Mark touches on screen
      //tft.fillCircle(x, y, 70, BLUE);
      //tft.fillCircle(x, y, 50, WHITE);
      //tft.fillCircle(x, y, 30, WHITE);

      //demonstrate the mouse cursor (yes, it won't be very visible under your finger, that's why it is left on the screen after you lift up
      tft.Graphic_Cursor_XY(x - cursorOffsetX, y - cursorOffsetY);


      old_coordinates[i - 1][0] = x;
      old_coordinates[i - 1][1] = y;
    }

    old_touches = current_touches;
    Serial.println();
  }

  static unsigned long lastCursorSwap = 0;
  if (millis() - lastCursorSwap > 3000) {
    //select the next cursor
    //Due to different cursor shapes, the "visual center" of each cursor is different, so we need different offsets.
    lastCursorSwap = millis();
    currentCursor++;
    if (currentCursor > 4) currentCursor = 1;
    switch (currentCursor) {
      case 1:
        tft.Select_Graphic_Cursor_1();
        updateOffsets(0, 0);
        break;
      case 2:
        tft.Select_Graphic_Cursor_2();
        updateOffsets(0, 0);
        break;
      case 3:
        tft.Select_Graphic_Cursor_3();
        updateOffsets(15, 10);
        break;
      case 4:
        tft.Select_Graphic_Cursor_4();
        updateOffsets(15, 10);
        break;
    }
  }

  static unsigned long lastRotate = 0;
  static int rotation = 0;
  if (currentCursor == 4 && millis() - lastRotate > 100) {
    lastRotate = millis();
    rotation++;
    if (rotation >= 8) rotation = 0;
    //rotate our custom cursor through 8 orientations
    rotateCursor(rotation);
  }
#endif
}

void updateOffsets(int x, int y) {
  //change the graphic cursor offset
  int oldOffsetX = cursorOffsetX;
  int oldOffsetY = cursorOffsetY;
  cursorOffsetX = x;
  cursorOffsetY = y;
  tft.Graphic_Cursor_XY(tft.GetGCursorX() + oldOffsetX - cursorOffsetX, tft.GetGCursorY() + oldOffsetY - cursorOffsetY);
}

void rotateCursor(int rotation) {
  //flip or rotate the cursor image into a temporary array then reload that array onto the RA8876
  unsigned char data[256];
  int outx = 0; //horizontal (byte) in output array
  int outy = 0; //vertical coordinate in output
  int out = 0; //byte index into output array
  int inx, iny, in;
  while (out < 256) {
    unsigned char tmp = 0;
    switch (rotation) {
      case 0: //no rotation, out = in
        tmp = customCursor[out];
        break;
      case 1: //flip on diagonal from bottom left to top right (or rotate right 90, then flip on horizontal axis)
        inx = 7 - outy / 4;
        iny = 31 - outx * 4;
        in = iny * 8 + inx;
        tmp = reverseByte(rotateByte(customCursor, in, 3 - outy % 4));
        break;
      case 2: //rotate 90 right
        inx = outy / 4;
        iny = 31 - outx * 4;
        in = iny * 8 + inx;
        tmp = reverseByte(rotateByte(customCursor, in, outy % 4));
        break;
      case 3: //flip on horizontal axis: top to bottom
        inx = outx;
        iny = 31 - outy;
        in = iny * 8 + inx;
        tmp = customCursor[in];
        break;
      case 4: //flip both H and V
        inx = 7 - outx;
        iny = 31 - outy;
        in = iny * 8 + inx;
        tmp = reverseByte(customCursor[in]);
        break;
      case 5: //rotate 90 left then flip on horizontal axis
        inx = outy / 4;
        iny = outx * 4 + 3;
        in = iny * 8 + inx;
        tmp = rotateByte(customCursor, in, outy % 4);
        break;
      case 6: //rotate 90 left
        inx = 7 - outy / 4;
        iny = outx * 4 + 3;
        in = iny * 8 + inx;
        tmp = rotateByte(customCursor, in, 3 - outy % 4);
        break;
      case 7: //flip on vertical axis: left to right
        inx = 7 - outx;
        iny = outy;
        in = iny * 8 + inx;
        tmp = reverseByte(customCursor[in]);
        break;
      default: //no rotation, out = in
        tmp = customCursor[out];
    }

    data[out] = tmp;
    outx++;
    if (outx >= 8) {
      outx = 0;
      outy++;
    }
    out++;
  }

  //Yes, we really can just upload a new cursor while it's currently displayed.
  //This does occasionally display a glitch but only for a very short time as
  //the upload will finish before the next "frame" of the LCD is displayed.
  tft.Upload_Graphic_Cursor(4, data);
}

unsigned char reverseByte(unsigned char b) {
  //swap the order of pixels in a byte
  return (b & 0b00000011) << 6 | (b & 0b00001100) << 2 | (b & 0b00110000) >> 2 | (b & 11000000) >> 6;
}

unsigned char rotateByte(unsigned char input[], int in, int column) {
  //return a byte transposed from a column
  //eg. "in" may be 248 which is the bottom-left byte in the 8x32 byte array
  //then in-8 is the row above this, in-16 is the row above that and in-24 is the row above that.
  //columns are numbered left-to right in the byte with 0 being the left or MSB side
  switch (column) {
    case 3: //select right-hand pixel out of each byte
      return (input[in] & 0b00000011) | (input[in - 8] & 0b00000011) << 2 | (input[in - 16] & 0b00000011) << 4 | (input[in - 24] & 0b00000011) << 6;
    case 2:
      return (input[in] & 0b00001100) >> 2 | (input[in - 8] & 0b00001100) | (input[in - 16] & 0b00001100) << 2 | (input[in - 24] & 0b00001100) << 4;
    case 1:
      return (input[in] & 0b00110000) >> 4 | (input[in - 8] & 0b00110000) >> 2 | (input[in - 16] & 0b00110000) | (input[in - 24] & 0b00110000) << 2;
    case 0: //left hand pixel in each byte
      return (input[in] & 0b11000000) >> 6 | (input[in - 8] & 0b11000000) >> 4 | (input[in - 16] & 0b11000000) >> 2 | (input[in - 24] & 0b11000000);
  }
  return 0;
}
