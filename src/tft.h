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
 */#ifndef TFT_H__
#define TFT_H__
#include "Arduino.h"

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

#ifdef __cplusplus
extern "C" {
#endif
boolean tft_init(void);
uint8_t vmemReadData(uint32_t addr);
void vmemWriteData(uint32_t addr, uint8_t vmemData);
uint16_t vmemReadData16(uint32_t addr);
void vmemWriteData16(uint32_t addr, uint16_t vmemData);
void selectScreen(uint32_t screenPage);
void saveTFTParams(tftSave_t *screenSave);
void restoreTFTParams(tftSave_t *screenSave);
void tft_print(uint8_t text);
void tftRawWrite(uint8_t data);
void tftPrintStr(const char * str);
void tft_slprint(uint16_t x0,uint16_t fgColor,uint16_t bgColor, const char *text);
void tft_cls(uint16_t color);
void tft_slcls(uint16_t color);
void setTextColor(uint16_t color);
void setBackGroundColor(uint16_t color);
void setTextColorFG(uint16_t fgc, uint16_t bgc);
int16_t getTextX(void);
uint16_t leftmarg(void);
uint16_t topmarg(void);
uint16_t rightmarg(void);
uint16_t bottommarg(void);
void setTMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb);
void setPromptSize(uint16_t ps);
uint8_t fontLoad(char *fontfile);
uint8_t fontLoadMEM(char *fontsrc);
void setFontSource(uint8_t source);
boolean setFontSize(uint8_t scale, boolean runflag);
void setTextAt(int16_t x, int16_t y);
int16_t getTextY(void);
int16_t getTwidth(void);
int16_t getTheight(void);
void setCursorMode(boolean mode);
void setCursorType(uint8_t type);
void setCursorBlink(boolean onOff);
uint16_t getTextFGC(void);
uint16_t getTextBGC(void);
int16_t getGwidth(void);
int16_t getGheight(void);
uint8_t getFontHeight(void);
uint8_t getFontWidth(void);
int16_t getPixel_16bpp(int16_t x, int16_t y);
void setPixel(int16_t x, int16_t y, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void cursorInit(void);

void gCursorSet(boolean gcursoronoff, uint8_t gcursortype, uint8_t gcursorcolor1, uint8_t gcursorcolor2);
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

boolean TStouched(void);
void getTSpoint(uint16_t *x, uint16_t *y);
void putPicture_16bppData8(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *data);
void scroll_down(void);
void scroll_up(void);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

void clreol(void);
void clreos(void);
void clrbol(void);
void clrbos(void);
void clrlin(void);
void timeOn(void);
void timeOff(void);
uint32_t tft_boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);
uint32_t tft_boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);
void tft_PIP (
 unsigned char On_Off // 0 : disable PIP, 1 : enable PIP, 2 : To maintain the original state
,unsigned char Select_PIP // 1 : use PIP1 , 2 : use PIP2
,unsigned long PAddr //start address of PIP
,unsigned short XP //coordinate X of PIP Window, It must be divided by 4.
,unsigned short YP //coordinate Y of PIP Window, It must be divided by 4.
,unsigned long ImageWidth //Image Width of PIP (recommend = canvas image width)
,unsigned short X_Dis //coordinate X of Display Window
,unsigned short Y_Dis //coordinate Y of Display Window
,unsigned short X_W //width of PIP and Display Window, It must be divided by 4.
,unsigned short Y_H //height of PIP and Display Window , It must be divided by 4.
);
#ifdef __cplusplus
}
#endif

boolean tft_init(void);
uint8_t vmemReadData(uint32_t addr);
void vmemWriteData(uint32_t addr, uint8_t vmemData);
uint16_t vmemReadData16(uint32_t addr);
void vmemWriteData16(uint32_t addr, uint16_t vmemData);
//void selectScreen(uint32_t *screenpage);
void selectScreen(uint32_t screenPage);
void saveTFTParams(tftSave_t *screenSave);
void restoreTFTParams(tftSave_t *screenSave);
void tft_print(uint8_t text);
void tftRawWrite(uint8_t data);
void tftPrintStr(const char * str);
void tft_slprint(uint16_t x0,uint16_t fgColor,uint16_t bgColor, const char *text);
void tft_cls(uint16_t color);
void tft_slcls(uint16_t color);
void setTextColor(uint16_t color);
void setBackGroundColor(uint16_t color);
void setTextColorFG(uint16_t fgc, uint16_t bgc);
uint16_t leftmarg(void);
uint16_t topmarg(void);
uint16_t rightmarg(void);
uint16_t bottommarg(void);
void setMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb);
void setPromptSize(uint16_t ps);
uint8_t fontLoad(char *fontfile);
uint8_t fontLoadMEM(char *fontsrc);
void setFontSource(uint8_t source);
boolean setFontSize(uint8_t scale, boolean runflag);
void setTextAt(int16_t x, int16_t y);
int16_t getTextX(void);
int16_t getTextY(void);
int16_t getTwidth(void);
int16_t getTheight(void);
void setCursorMode(boolean mode);
void setCursorType(uint8_t type);
void setCursorBlink(boolean onOff);
uint16_t getTextFGC(void);
uint16_t getTextBGC(void);
int16_t getGwidth(void);
int16_t getGheight(void);
uint8_t getFontHeight(void);
uint8_t getFontWidth(void);
int16_t getPixel_16bpp(int16_t x, int16_t y);
void setPixel(int16_t x, int16_t y, uint16_t color);
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color);
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
void cursorInit(void);

void gCursorSet(boolean gcursoronoff, uint8_t gcursortype, uint8_t gcursorcolor1, uint8_t gcursorcolor2);
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

boolean TStouched(void);
void getTSpoint(uint16_t *x, uint16_t *y);
void putPicture_16bppData8(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *data); 

void scroll_down(void);
void scroll_up(void);
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

void clreol(void);
void clreos(void);
void clrbol(void);
void clrbos(void);
void clrlin(void);
void timeOn(void);
void timeOff(void);
uint32_t tft_boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);
uint32_t tft_boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0);
void tft_PIP (
 unsigned char On_Off // 0 : disable PIP, 1 : enable PIP, 2 : To maintain the original state
,unsigned char Select_PIP // 1 : use PIP1 , 2 : use PIP2
,unsigned long PAddr //start address of PIP
,unsigned short XP //coordinate X of PIP Window, It must be divided by 4.
,unsigned short YP //coordinate Y of PIP Window, It must be divided by 4.
,unsigned long ImageWidth //Image Width of PIP (recommend = canvas image width)
,unsigned short X_Dis //coordinate X of Display Window
,unsigned short Y_Dis //coordinate Y of Display Window
,unsigned short X_W //width of PIP and Display Window, It must be divided by 4.
,unsigned short Y_H //height of PIP and Display Window , It must be divided by 4.
);
#endif
