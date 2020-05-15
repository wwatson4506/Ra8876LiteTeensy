/*File Name : tft.h                                   
 *          : For Teensy 3.x and T4
 *          : By Warren Watson
 *          : 06/07/2018 - 11/31/2019
 *          : Copyright (c) 2017-2019 Warren Watson.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef RA8876_T3_H__
#define RA8876_T3_H__
#include "Arduino.h"
#include "SPI.h"
#include "Ra8876_Lite.h"

// Define USE_FF_FONTLOAD to 1 if using FatFS to load user defined fonts
// from a disk drive. Needs FatFS, SDFat or SD. fontLoad() is currently
// setup to use FatFS.
// Else, set to 0
#define USE_FF_FONTLOAD 0

/* Screen Page Addresses */
#define SCREEN_1  0
#define SCREEN_2  1024*600*2
#define SCREEN_3  1024*600*2*2
#define SCREEN_4  1024*600*2*3
#define SCREEN_5  1024*600*2*4
#define SCREEN_6  1024*600*2*5
#define SCREEN_7  1024*600*2*6
#define SCREEN_8  1024*600*2*7
#define SCREEN_9  1024*600*2*8
//#define SCREEN_10  1024*600*2*9 // Used for CGRAM at this time

typedef struct boxSaveGet boxSaveGet_t;

/* Struct used for BTE block xfers */
struct boxSaveGet {
	uint8_t id;
	uint8_t id_next;
	uint16_t x0;
	uint16_t y0;
	uint16_t x1;
	uint16_t y1;
	uint32_t vpage;
};

typedef struct tftSave tftSave_t;

/* Struct for saving and retrieving screen page parameters */
struct tftSave {
	int16_t	 width;
	int16_t  height;
	int16_t	 cursorX;
	int16_t  cursorY;
	uint8_t  scaleX;
	uint16_t scaleY;
	uint8_t	 FNTwidth;
	uint8_t  FNTheight;
	uint8_t  fontheight;
	uint8_t  fontSource;
	uint8_t  TXTparameters;
	uint8_t  cursorXsize;
	uint8_t  cursorYsize;
	uint32_t currentPage;
	uint32_t pageOffset;
// Text Sreen Vars
	uint16_t  prompt_size; // prompt ">"
	uint16_t prompt_line; // current text prompt row
	uint8_t	 vdata;
	uint8_t  leftmarg;
	uint8_t  topmarg;
	uint8_t  rightmarg;
	uint8_t  bottommarg;
	uint8_t  tab_size;
	uint16_t CharPosX;
	uint16_t CharPosY;
	boolean	 UDFont;
//scroll vars ----------------------------
	uint16_t  scrollXL;
	uint16_t  scrollXR;
	uint16_t  scrollYT;
	uint16_t  scrollYB;
// Color vars ----------------------------
	uint16_t TXTForeColor;
    uint16_t TXTBackColor;
};

typedef struct Gbuttons gbuttons_t;
/* Struct for graphic buttons */
/* Based on Adafruit graphic buttons */
/* Not completly implemented at this time */
struct Gbuttons {
  boolean initialzed;
  uint16_t x;
  uint16_t y;
  uint16_t w;
  uint16_t h;
  uint16_t outlinecolor;
  uint16_t fillcolor;
  uint16_t textcolor;
  uint16_t textsize;
  boolean currstate;
  boolean laststate;
  char     label[10];
};

//https://i.pinimg.com/736x/4b/50/12/4b5012fc9d868d0394da7fa8217d7f92.jpg
#define BLACK		0x0000
#define WHITE		0xffff
#define RED			0xf800
#define LIGHTRED	0xfc10
#define CRIMSON		0x8000
#define GREEN		0x07e0
#define PALEGREEN	0x87f0
#define DARKGREEN	0x0400
#define BLUE		0x001f
#define LIGHTBLUE	0x051f
#define SKYBLUE		0x841f
#define DARKBLUE	0x0010
#define YELLOW		0xffe0
#define LIGHTYELLOW	0xfff0
#define DARKYELLOW	0x8400 // mustard
#define CYAN		0x07ff
#define LIGHTCYAN	0x87ff
#define DARKCYAN	0x0410
#define MAGENTA		0xf81f
#define VIOLET		0xfc1f
#define BLUEVIOLET	0x8010
#define ORCHID		0xA145 

class RA8876_t3 : public Ra8876_Lite
{
public:
	RA8876_t3(const uint8_t CSp = 10, const uint8_t RSTp = 8, const uint8_t mosi_pin = 11, const uint8_t sclk_pin = 13, const uint8_t miso_pin = 12);

	boolean begin(void);
	uint8_t vmemReadData(uint32_t addr);
	void vmemWriteData(uint32_t addr, uint8_t vmemData);
	uint16_t vmemReadData16(uint32_t addr);
	void vmemWriteData16(uint32_t addr, uint16_t vmemData);
	void selectScreen(uint32_t screenPage);
	void saveTFTParams(tftSave_t *screenSave);
	void restoreTFTParams(tftSave_t *screenSave);
	
	void useCanvas();
	void updateScreen();
	
	void tftRawWrite(uint8_t data);
	void printStatusLine(uint16_t x0,uint16_t fgColor,uint16_t bgColor, const char *text);
	void fillScreen(uint16_t color);
	void fillStatusLine(uint16_t color);
	void setTextColor(uint16_t color);
	void setBackGroundColor(uint16_t color);
	void setTextColor(uint16_t fgc, uint16_t bgc);
	int16_t getTextX(void);

	void setMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb);
	void setTMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb);
	void setPromptSize(uint16_t ps);
	uint8_t fontLoad(char *fontfile);
	uint8_t fontLoadMEM(char *fontsrc);
	void setFontSource(uint8_t source);
	boolean setFontSize(uint8_t scale, boolean runflag=false);
	void setTextSize(uint8_t scale, boolean runflag=false) { setFontSize(scale, runflag);}
	int16_t getTextY(void);
	int16_t getTwidth(void);
	int16_t getTheight(void);
	void setCursorMode(boolean mode);
	void setCursorType(uint8_t type);
	void setCursorBlink(boolean onOff);
	uint16_t getTextFGC(void);
	uint16_t getTextBGC(void);
	int16_t width(void) { return _width; }
	int16_t height(void) { return _height; }
	uint8_t getFontHeight(void);
	uint8_t getFontWidth(void);

	void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
	void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
	void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
	void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
	void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
	void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
	void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
	void cursorInit(void);
	void setCursor(uint16_t x, uint16_t y);
	void gCursorSet(boolean gCursorEnable, uint8_t gcursortype, uint8_t gcursorcolor1, uint8_t gcursorcolor2);
	void gcursorxy(uint16_t gcx, uint16_t gcy);
	void initButton(struct Gbuttons *button, uint16_t x, uint16_t y, uint8_t w, uint8_t h,
	 uint16_t outline, uint16_t fill, uint16_t textcolor,
	 char *label, uint8_t textsize);

	void drawButton(struct Gbuttons *buttons, boolean inverted);
	boolean buttonContains(struct Gbuttons *buttons,uint16_t x, uint16_t y);
	void buttonPress(struct Gbuttons *buttons, boolean p);
	boolean buttonIsPressed(struct Gbuttons *buttons);
	boolean buttonJustPressed(struct Gbuttons *buttons);
	boolean buttonJustReleased(struct Gbuttons *buttons);

	void touchEnable(boolean enabled);
	void readTouchADC(uint16_t *x, uint16_t *y);

	
	boolean TStouched(void);
	void getTSpoint(uint16_t *x, uint16_t *y);
	void putPicture_16bppData8(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *data);

	void scrollUp(void);
};

#endif

