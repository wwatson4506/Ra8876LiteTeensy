/*File Name : tft.cpp                                   
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
#include <time.h>
#include "Arduino.h"
#include "Ra8876_Lite.h"
#include "tft.h"
//#include "XPT2046.h"
#include "TimeLib.h"
#include "IntervalTimer.h"

#define RA8876_CS 10 //see below...
#define RA8876_RESET 21//any pin or nothing!
#define RA8876_MOSI 11
#define RA8876_SCLK 13
#define RA8876_MISO 12
#define RA8876_BACKLITE  20

// RA8876 Interrupt Pin
//#define RA8876_INT  2

// Touch Screen is not enabled at this time.
// Uses a modified version of XPT2046. WIP
// Touch Screen CS Pin
#define CS_PIN	31
// Touch Screen Interrupt Pin
#define TIRQ_PIN	24

// Used for Status Line Time Display
IntervalTimer timeDisplay;
static time_t timeval;
static char datestr[80];
struct tm *time_p;

// Create a parameter save structure for all 10 screen pages	
tftSave_t	screenSave1,
			screenSave2,
			screenSave3,
			screenSave4,
			screenSave5,
			screenSave6,
			screenSave7,
			screenSave8,
			screenSave9,
			screenSave10;
// Create pointers to each screen save structs
tftSave_t *screenPage1 = &screenSave1;
tftSave_t *screenPage2 = &screenSave2;
tftSave_t *screenPage3 = &screenSave3;
tftSave_t *screenPage4 = &screenSave4;
tftSave_t *screenPage5 = &screenSave5;
tftSave_t *screenPage6 = &screenSave6;
tftSave_t *screenPage7 = &screenSave7;
tftSave_t *screenPage8 = &screenSave8;
tftSave_t *screenPage9 = &screenSave9;
//tftSave_t *screenPage10 = &screenSave10; // Not used at this time

// Create RA8876 driver instance
Ra8876_Lite tft = Ra8876_Lite(RA8876_CS, RA8876_RESET, RA8876_MOSI, RA8876_SCLK, RA8876_MISO);

// Initialize Touch Screen driver
//XPT2046 touch(CS_PIN, TIRQ_PIN);

boolean tft_init(void) {
	// Setup Touch Screen Pin as Output
//	pinMode(CS_PIN,OUTPUT);
//	digitalWrite(CS_PIN, HIGH);
	// Setup Backlite enable pin and turn backlite on
	pinMode(RA8876_BACKLITE, OUTPUT);    //backlight 
	digitalWrite(RA8876_BACKLITE, HIGH); //on

	// Initialize RA8876 driver
	if(!tft.Ra8876_begin())
		return false;

	tft.displayOn(true);	// Turn on TFT display
	tft.UDFont = false;     // Turn off user define fonts
	tft._scaleX = 1;	// Init Font Scale (x,y)
	tft._scaleY = 1;
	tft._FNTwidth = 8;   //8 or 12 or 16; // Default font width
	tft._FNTheight = 16; // 16 or 24 or 32; // Default font height;

	// Setup graphic cursor
	tft.Set_Graphic_Cursor_Color_1(0xff); // Foreground color
	tft.Set_Graphic_Cursor_Color_2(0x00); // Outline color
	tft.Graphic_cursor_initial();  // Initialize Graphic Cursor
    tft.Select_Graphic_Cursor_2(); // Select Arrow Graphic Cursor
	// Set default foreground and background colors
	setTextColorFG(tft._TXTForeColor, tft._TXTBackColor);	
	// Position text cursor to default
	tft.setTextCursor(tft._scrollXL, tft._scrollYT);
	// Setup Text Cursor
	cursorInit();
	// Set Margins to default settings
	setTMargins(0, 0, 0, 0); // Left Side, Top Side, Right Side, Bottom Side

	// Save Default Screen Parameters
	saveTFTParams(screenPage1);
	saveTFTParams(screenPage2);
	saveTFTParams(screenPage3);
	saveTFTParams(screenPage4);
	saveTFTParams(screenPage5);
	saveTFTParams(screenPage6);
	saveTFTParams(screenPage7);
	saveTFTParams(screenPage8);
	saveTFTParams(screenPage9);
//	saveTFTParams(screenPage10);
	// Initialize all screen colors to default values
	tft.currentPage = 999; // Don't repeat screen page 1 init.
	selectScreen(PAGE1_START_ADDR);	// Init page 1 screen
	tft_cls(COLOR65K_DARKBLUE);     // Not sure why we need to clear the screen twice
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE2_START_ADDR);	// Init page 2 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE3_START_ADDR);	// Init page 3 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE4_START_ADDR);	// Init page 4 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE5_START_ADDR);	// Init page 5 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE6_START_ADDR);	// Init page 6 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE7_START_ADDR);	// Init page 7 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE8_START_ADDR);	// Init page 8 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE9_START_ADDR);	// Init page 9 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE10_START_ADDR);	// Init page 10 screen
	tft_cls(COLOR65K_DARKBLUE);
	tft_slcls(COLOR65K_DARKBLUE);
	selectScreen(PAGE1_START_ADDR); // back to page 1 screen
	
	//start Touch Screen
//	touch.begin(getGwidth(),getGheight());

	// Set graphic mouse cursor to center of screen
	gcursorxy(getGwidth() / 2, getGheight() / 2);
	return true;
}

// Select a screen page (Buffer) 1 to 9.
// ALT + (F1 to F9) using USBHost_t36 Keyboard Driver
// and my USBKeyboard host driver.
// Also, STBASIC Commands: screen 0 to screen 8
void selectScreen(uint32_t screenPage) {
	tftSave_t *tempSave, *tempRestore;
	// Don't Select the current screen page
	if(screenPage == tft.currentPage)
		return;
	switch(tft.currentPage) {
		case PAGE1_START_ADDR:
			tempSave = screenPage1; 
			break;
		case PAGE2_START_ADDR:
			tempSave = screenPage2; 
			break;
		case PAGE3_START_ADDR:
			tempSave = screenPage3; 
			break;
		case PAGE4_START_ADDR:
			tempSave = screenPage4; 
			break;
		case PAGE5_START_ADDR:
			tempSave = screenPage5; 
			break;
		case PAGE6_START_ADDR:
			tempSave = screenPage6; 
			break;
		case PAGE7_START_ADDR:
			tempSave = screenPage7; 
			break;
		case PAGE8_START_ADDR:
			tempSave = screenPage8; 
			break;
		case PAGE9_START_ADDR:
			tempSave = screenPage9; 
			break;
//		case PAGE10_START_ADDR:
//			tempSave = screenPage10; 
//			break;
		default:
			tempSave = screenPage1; 
	}
	// Copy back selected screen page parameters
	switch(screenPage) {
		case PAGE1_START_ADDR:
			tempRestore = screenPage1; 
			break;
		case PAGE2_START_ADDR:
			tempRestore = screenPage2; 
			break;
		case PAGE3_START_ADDR:
			tempRestore = screenPage3; 
			break;
		case PAGE4_START_ADDR:
			tempRestore = screenPage4; 
			break;
		case PAGE5_START_ADDR:
			tempRestore = screenPage5; 
			break;
		case PAGE6_START_ADDR:
			tempRestore = screenPage6; 
			break;
		case PAGE7_START_ADDR:
			tempRestore = screenPage7; 
			break;
		case PAGE8_START_ADDR:
			tempRestore = screenPage8; 
			break;
		case PAGE9_START_ADDR:
			tempRestore = screenPage9; 
			break;
//		case PAGE10_START_ADDR:
//			tempRestore = screenPage10; 
//			break;
		default:
			tempRestore = screenPage1; 
	}
	// Save current screen page parameters 
	saveTFTParams(tempSave);
	// Restore selected screen page parameters
	restoreTFTParams(tempRestore);
	// Setup params to rebuild selected screen
	tft.pageOffset = screenPage;
	tft.currentPage = screenPage;
	tft.displayImageStartAddress(tft.currentPage);
	tft.displayImageWidth(SCREEN_WIDTH);
	tft.displayWindowStartXY(0,0);
	tft.canvasImageStartAddress(tft.currentPage);
	tft.canvasImageWidth(SCREEN_WIDTH);
	tft.activeWindowXY(0,0);
	tft.activeWindowWH(SCREEN_WIDTH,600); 
	tft.setTextCursor(tft._cursorX, tft._cursorY);
	tft.textColor(tft._TXTForeColor,tft._TXTBackColor);
	// Rebuild the display
	tft.buildTextScreen();
	
}

// Save current screen page parameters 
void saveTFTParams(tftSave_t *screenSave) {
	screenSave->width			= tft._width;
	screenSave->height			= tft._height;
	screenSave->cursorX			= tft._cursorX;
	screenSave->cursorY			= tft._cursorY;
	screenSave->scaleX			= tft._scaleX;
	screenSave->scaleY			= tft._scaleY;
	screenSave->FNTwidth		= tft._FNTwidth;
	screenSave->FNTheight		= tft._FNTheight;
	screenSave->fontheight		= tft._fontheight;
	screenSave->fontSource      = tft.currentFont;
	screenSave->cursorXsize		= tft._cursorXsize;
	screenSave->cursorYsize		= tft._cursorYsize;
// Text Sreen Vars
	screenSave->prompt_size		= tft.prompt_size; // prompt ">"
	screenSave->prompt_line		= tft.prompt_line; // current text prompt row
	screenSave->vdata			= tft.vdata;
	screenSave->leftmarg		= tft.leftmarg;
	screenSave->topmarg			= tft.topmarg;
	screenSave->rightmarg		= tft.rightmarg;
	screenSave->bottommarg		= tft.bottommarg;
	screenSave->tab_size		= tft.tab_size;
	screenSave->CharPosX		= tft.CharPosX;
	screenSave->CharPosY		= tft.CharPosY;
	screenSave->UDFont  		= tft.UDFont;

//scroll vars ----------------------------
	screenSave->scrollXL		= tft._scrollXL;
	screenSave->scrollXR		= tft._scrollXR;
	screenSave->scrollYT		= tft._scrollYT;
	screenSave->scrollYB		= tft._scrollYB;
	screenSave->TXTForeColor	= tft._TXTForeColor;
    screenSave->TXTBackColor	= tft._TXTBackColor;
}

// Restore selected screen page parameters
void restoreTFTParams(tftSave_t *screenSave) {
 tft._width = screenSave->width;
 tft._height = screenSave->height;
 tft._cursorX = screenSave->cursorX;
 tft._cursorY = screenSave->cursorY;
 tft._scaleX = screenSave->scaleX;
 tft._scaleY = screenSave->scaleY;
 tft._FNTwidth = screenSave->FNTwidth;
 tft._FNTheight = screenSave->FNTheight;
 tft._fontheight = screenSave->fontheight;
 tft.currentFont = screenSave->fontSource;
 tft._cursorXsize = screenSave->cursorXsize;		
 tft._cursorYsize = screenSave->cursorYsize;		
// Text Sreen Vars
 tft.prompt_size = screenSave->prompt_size; // prompt ">"
 tft.prompt_line = screenSave->prompt_line; // current text prompt row
 tft.vdata = screenSave->vdata;			
 tft.leftmarg = screenSave->leftmarg;		
 tft.topmarg = screenSave->topmarg;		
 tft.rightmarg = screenSave->rightmarg;
 tft.bottommarg = screenSave->bottommarg;		
 tft.tab_size =	screenSave->tab_size;		
 tft.CharPosX = screenSave->CharPosX;		
 tft.CharPosY = screenSave->CharPosY;		
 tft.UDFont = screenSave->UDFont;		
//scroll vars -----------------
 tft._scrollXL = screenSave->scrollXL;
 tft._scrollXR = screenSave->scrollXR;
 tft._scrollYT = screenSave->scrollYT;
 tft._scrollYB = screenSave->scrollYB;
 tft._TXTForeColor = screenSave->TXTForeColor;
 tft._TXTBackColor = screenSave->TXTBackColor;
}

// Setup text cursor
void cursorInit(void)
{
	tft._cursorXsize = tft._FNTwidth;
	tft._cursorYsize = tft._FNTheight-1;
	tft.Text_Cursor_H_V(tft._cursorXsize,tft._cursorYsize); 		// Block cusror
	tft.Blinking_Time_Frames(20);		// Set blink rate
	tft.Enable_Text_Cursor_Blinking();	// Turn blinking cursor on
}

// Linear video 8 bit data read
uint8_t vmemReadData(uint32_t addr)	{
	 return tft.vmemReadData(addr);	
}

// Linear video 8 bit data write
void vmemWriteData(uint32_t addr, uint8_t vmemData)	{
	tft.vmemWriteData(addr, vmemData);	
}

// Linear video 16 bit data read
uint16_t vmemReadData16(uint32_t addr)	{
	 return tft.vmemReadData16(addr);	
}

// Linear video 16 bit data write
void vmemWriteData16(uint32_t addr, uint16_t vmemData)	{
	tft.vmemWriteData16(addr, vmemData);	
}

// Get left text margin
uint16_t leftmarg(void) {
	return tft.leftmarg;
}

// Get top text margin
uint16_t topmarg(void) {
	return tft.topmarg;
}

// Get right text margin
uint16_t rightmarg(void) {
	return tft.rightmarg;
}

// Get bottom text margin
uint16_t bottommarg(void) {
	return tft.bottommarg;
}

//Set Graphic Mode Margins (pixel based)
void setMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb) {
	tft._scrollXL = xl;
	tft._scrollYT = yt;
	tft._scrollXR = xr;
	tft._scrollYB = yb;
	tft.buildTextScreen();	
	tft.setTextCursor(tft._scrollXL,tft._scrollYT);
}

//Set Text Mode Margins (font size based)
void setTMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb) {
	tft._scrollXL = xl*tft._FNTwidth;
	tft._scrollYT = yt*tft._FNTheight;
	tft._scrollXR = SCREEN_WIDTH-(xr*tft._FNTwidth);
	tft._scrollYB = SCREEN_HEIGHT-(yb*tft._FNTheight);
	tft.buildTextScreen();	
	tft.setTextCursor(tft._scrollXL,tft._scrollYT);
}

// Set text prompt size (font size based)
void setPromptSize(uint16_t ps) {
	tft.prompt_size = ps*(tft._FNTwidth * tft._scaleX) + tft._scrollXL; // Default prompt ">"
}

// Clear current screen to background 'color'
void tft_cls(uint16_t color) {
	tft.drawSquareFill(tft._scrollXL, tft._scrollYT, tft._scrollXR, tft._scrollYB, color);
	tft.textColor(tft._TXTForeColor,tft._TXTBackColor);
	tft.setTextCursor(tft._scrollXL,tft._scrollYT);
}

// Clear Status Line to background 'color'
void tft_slcls(uint16_t color) {
	uint16_t temp = tft._TXTBackColor;
	tft.clearStatusLine(color);
	tft._TXTBackColor = temp;
	tft.textColor(tft._TXTForeColor,temp);
}

//**************************************************************//
//**************************************************************//
void setTextColor(uint16_t color)
{
	tft.foreGroundColor16bpp(color);
}

//**************************************************************//
//**************************************************************//
void setBackGroundColor(uint16_t color)
{
	tft.backGroundColor16bpp(color);
}

// Set text foreground/background colors
void setTextColorFG(uint16_t fgc, uint16_t bgc) {
	tft.textColor(fgc,bgc);
}

// Send a character to the current text position. (processes ASCII control chars).
void tft_print(uint8_t text) {
	tft.tftPrint(text);
}

// Send a string to the current text position. (proccesses ASCII control chars).
void tftPrintStr(const char * str) {
  while(*str != '\0')
  {
  tft_print(*str);
  ++str; 
  } 
}

// Send a character directly to video memory (No control ASCII codes are processed).
void tftRawWrite(uint8_t data) {
	tft.tftRawPrint(data);
}
	
// Send a string to the status line
void tft_slprint(uint16_t x0,uint16_t fgColor,uint16_t bgColor, const char *text) {
	tft.writeStatusLine(x0*(tft._FNTwidth*tft._scaleX), fgColor, bgColor, text);	
}

// Load a user defined font from memory to RA8876 Character generator RAM
uint8_t fontLoadMEM(char *fontsrc) {
	tft.CGRAM_initial(PATTERN1_RAM_START_ADDR, (uint8_t *)fontsrc, 16*256);
	return 0;
}

// Set USE_FF_FONTLOAD in tft.h to 1 to use fontload function.
// Requires FatFS or Can be modified to use SDFat or SD.
#if USE_FF_FONTLOAD == 1
#include "ff.h"
// Load a user defined font from a file to RA8876 Character generator RAM
uint8_t fontLoad(char *fontfile) {
    FIL fsrc;      /* File object */
    FRESULT fresult = FR_OK;          /* FatFs function common result code */
    uint br = 0;
	uint8_t fontdata[4096];	// Buffer is setup to use 8x16 fonts.
							// Bigger fonts can be used.
    /* Open source file */
    fresult = f_open(&fsrc, fontfile, FA_READ);
    if (fresult)
		return 1; /* return any error */
    for (;;) {
        fresult = f_read(&fsrc, fontdata, sizeof fontdata, &br);  /* Read a chunk of source file */
        if (fresult || br == 0) break; /* error or eof */
    }
    /* Close open file */
    f_close(&fsrc);
	// Initialize CGRAM with font loaded into fontdata buffer.
	tft.CGRAM_initial(PATTERN1_RAM_START_ADDR, fontdata, 16*256);
	return 0;
}
#endif

// Select internal fonts or user defined fonts (0 for internal or 1 for user defined)
void setFontSource(uint8_t source) {
	switch(source) {
	case 0:
		tft._scaleX = 1;
		tft._scaleY = 1;
		break;
	case 1:
		if((tft._FNTheight != 16) && (tft._FNTwidth != 8)) {
			tft._FNTheight = 16;
			tft._FNTwidth  = 8;
		}
		break;
	}
	tft.setFontSource(source);
	// Rebuild current screen page
	tft.buildTextScreen();
}

// Set fontsize for fonts, currently 0 to 2 (Internal and User Defined)
boolean setFontSize(uint8_t scale, boolean runflag) {
	switch(scale) {
		
		case 0:
			if(tft.UDFont) { // User Defined Fonts
				tft._scaleY = 1;
				tft._scaleX = 1;
			} else {
				tft._FNTheight = 16;
				tft._FNTwidth  = 8;
			}
			break;
		case 1:
			if(tft.UDFont) {
				tft._FNTheight = 16;
				tft._FNTwidth  = 8;
				tft._scaleY = 2;
				tft._scaleX = 2;
			} else {
				tft._FNTheight = 24;
				tft._FNTwidth  = 12;
			}
			break;
		case 2:
			if(tft.UDFont) {
				tft._FNTheight = 16;
				tft._FNTwidth  = 8;
				tft._scaleY = 3;
				tft._scaleX = 3;
			} else {
				tft._FNTheight = 32;
				tft._FNTwidth  = 16;
			}
			break;
		default:
			return true;
			break;
		tft.setTextCursor(tft._scrollXL,tft._scrollYT);
	}
	// Rebuild current screen page
	tft.buildTextScreen();
	cursorInit();
	if(runflag == false) {
		tft.drawSquareFill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, tft._TXTBackColor);
		tft.textColor(tft._TXTForeColor,tft._TXTBackColor);
		tft.setTextCursor(0,0);
	}
	tft.rightmarg = (uint8_t)(tft._width / (tft._FNTwidth * tft._scaleX));
	tft.bottommarg = (uint8_t)(tft._height / (tft._FNTheight * tft._scaleY));
	
	return false;
}

// Text cursor on or off
void setCursorMode(boolean mode) {

	if(mode == false)
		tft.Disable_Text_Cursor();	// No cursor
	else
		tft.Enable_Text_Cursor();	// Blinking block cursor
}

// Set text cursor type
void setCursorType(uint8_t type) {

	switch(type) {
		case 0:
			tft._cursorYsize = tft._FNTheight;
			tft.Text_Cursor_H_V(tft._cursorXsize,tft._cursorYsize); 		// Block cursor
			break;
		case 1:
			tft._cursorYsize = 2;
			tft.Text_Cursor_H_V(tft._cursorXsize,tft._cursorYsize); 		// Underline cursor
			break;
		case 2:
			tft._cursorYsize = tft._FNTheight;
			tft.Text_Cursor_H_V(2,tft._cursorYsize); 						// I-beam cursor
			break;
		default:
			break;
	}
}

// Set text cursor blink mode (On or Off)
void setCursorBlink(boolean onOff) {
	if(onOff)
		tft.Enable_Text_Cursor_Blinking();
	else
		tft.Disable_Text_Cursor_Blinking();
}
	
// Position text cursor
void setTextAt(int16_t x, int16_t y) {
	tft.textxy((uint16_t)x,(uint16_t)y);
}

// Get cursor X position
int16_t getTextX(void) {
	return (tft._cursorX / tft._FNTwidth); //tft.getCursorX() / tft.getFontWidth();
}	
	
// Get cursor Y position
int16_t getTextY(void) {
	return (tft._cursorY / tft._FNTheight);//tft.getCursorY() / tft.getFontHeight();
}	

// Get current text screen width (pixel * current font width)
int16_t getTwidth(void) {
	return (rightmarg()-leftmarg())-1;
}

// Get current text screen height (pixel * current font height)
int16_t getTheight(void) {
	return (bottommarg()-topmarg())-1;
}

// Get current screen foreground color
uint16_t getTextFGC(void) {
	return tft._TXTForeColor;
}

// Get current screen backround color
uint16_t getTextBGC(void) {
	return tft._TXTBackColor;
}

// Get Graphic width size (in pixels)
int16_t getGwidth(void) {
	return tft._width;
//	return tft._width-1;
}

// Get Graphic vertical size (in pixels)
int16_t getGheight(void) {
	return tft._height;
}

// Get font vertical size (in pixels)
uint8_t getFontHeight(void) {
	return tft._FNTheight * tft._scaleY;
}

// Get font width size (in pixels)
uint8_t getFontWidth(void) {
	return tft._FNTwidth * tft._scaleX;
}

// Get 16 bit pixel value from RA8876 current screen page (x,y coords)
int16_t getPixel_16bpp(int16_t x, int16_t y) {
	return tft.getPixel_16bpp(x, y);
}

// Set 16 bit pixel value in RA8876 current screen page (x,y coords)
void setPixel(int16_t x, int16_t y, uint16_t color) {
	tft.putPixel_16bpp(x, y, color);
}
// Draw a line. TODO: Add brush size
void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {
	tft.drawLine(x0, y0, x1, y1, color);
}

// Draw a vline. TODO: Add brush size, do fastHline()
void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
	tft.drawFastVLine(x, y, h, color);
}

// Draw a rectangle. TODO: Add line thickness size
void drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	uint16_t tempColor = tft._TXTForeColor;
	tft.drawSquare(x, y, w, h, color);
	tft._TXTForeColor = tempColor;
	setTextColorFG(tft._TXTForeColor,tft._TXTBackColor);
}

// Draw a filled rectangle.
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	uint16_t tempColor = tft._TXTForeColor;
	tft.drawSquareFill(x, y, w, h, color);
	tft._TXTForeColor = tempColor;
	setTextColorFG(tft._TXTForeColor,tft._TXTBackColor);
}

// Draw a round rectangle. TODO: Add line thickness size
void drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color) {
	tft.drawCircleSquare(x, y, w, h, xr, yr, color);
}

// Draw a filed round rectangle.
void fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color) {
	tft.drawCircleSquareFill(x, y, w, h, xr, yr, color);
}

// Enable Touch Screen.
void touchEnable(boolean enabled) {
//		tft.touchEnable(enabled);
}

// Return true if screen touched else false
boolean touchDetect(boolean autoclear) {
//	return tft.touchDetect(autoclear);
return false; // return false for now
}

// Draw a circle TODO: Add circle thickness size
void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	tft.drawCircle(x0, y0, r, color);
}

// Draw a filled circle
void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	tft.drawCircleFill(x0, y0, r, color);
}

// Draw an ellipse. TODO: Add line thickness size
void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color) {
	tft.drawEllipse(xCenter, yCenter, longAxis, shortAxis, color);
}

// Draw a filled ellipse.
void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color) {
	tft.drawEllipseFill(xCenter, yCenter, longAxis, shortAxis, color);
}

// Draw a triangle. TODO: Add line thickness size
void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	tft.drawTriangle(x0, y0, x1, y1, x2, y2, color);
}

// Draw a filled triangle
void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	tft.drawTriangleFill(x0, y0, x1, y1, x2, y2, color);
}

// Read touch screen ADC
void readTouchADC(uint16_t *x, uint16_t *y) {
//	tft.touchReadAdc(x, y);//we using 10bit adc data here
}

// Setup graphic mouse cursor// Draw an ellipse. TODO: Add line thickness size
void gCursorSet(boolean gcursoronoff, uint8_t gcursortype, uint8_t gcursorcolor1, uint8_t gcursorcolor2) {
	if (gcursoronoff == 0)
		tft.Disable_Graphic_Cursor();
	else
		tft.Enable_Graphic_Cursor();
	switch(gcursortype) {
		case 1:
			tft.Select_Graphic_Cursor_1(); // PEN
			break;
		case 2:
			tft.Select_Graphic_Cursor_2(); // ARROW
			break;
		case 3:
			tft.Select_Graphic_Cursor_3(); // HOUR GLASS
			break;
		case 4:
			tft.Select_Graphic_Cursor_4(); // ERROR SYMBOL
			break;
		default:
			tft.Select_Graphic_Cursor_2();
	}
	tft.Set_Graphic_Cursor_Color_1(gcursorcolor1);
	tft.Set_Graphic_Cursor_Color_2(gcursorcolor2);
}

// Set graphic cursor position on screen
void gcursorxy(uint16_t gcx, uint16_t gcy) {
	tft.Graphic_Cursor_XY(gcx, gcy);
}

//==========================================================================================
//= The following Graphic Button functions are based on Adafruits Graphic button libraries =
//==========================================================================================
// Initialize a graphic button. 
void initButton(struct Gbuttons *buttons, uint16_t x, uint16_t y, uint8_t w, uint8_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 char *label, uint8_t textsize)
{
	buttons->x = x;
	buttons->y = y;
	buttons->w = w;
	buttons->h = h;
	buttons->outlinecolor = outline;
	buttons->fillcolor = fill;
	buttons->textcolor = textcolor;
	buttons->textsize  = textsize;
	strncpy(buttons->label, label, 9);
	buttons->label[9] = 0;
}

// Draw graphic button. (not completely implemented yet)
void drawButton(struct Gbuttons *buttons, boolean inverted) {
  uint16_t fill, outline;
//  uint16_t text;
  uint16_t fgcolor = tft._TXTForeColor;	
  uint16_t bgcolor = tft._TXTBackColor;	

  if(!inverted) {
    fill    = buttons->fillcolor;
    outline = buttons->outlinecolor;
//    text    = buttons->textcolor;
  } else {
    fill    = buttons->textcolor;
    outline = buttons->outlinecolor;
//    text    = buttons->fillcolor;
  }
  tft.drawCircleSquareFill(buttons->x, buttons->y, buttons->w+buttons->x, buttons->h+buttons->y,
					   min(buttons->w,buttons->h)/4, min(buttons->w,buttons->h)/4, fill);

  tft.drawCircleSquare(buttons->x, buttons->y, buttons->w+buttons->x, buttons->h+buttons->y,
				   min(buttons->w,buttons->h)/4, min(buttons->w,buttons->h)/4, outline);

//  _gfx->setCursor(_x - strlen(_label)*3*_textsize, _y-4*_textsize);
//  _gfx->setTextColor(text);
//  _gfx->setTextSize(_textsize);
//  _gfx->print(_label);
  tft.textColor(fgcolor,bgcolor);	// restore colors	
}

// Check the button to see if it is within the range of selection.
boolean buttonContains(struct Gbuttons *buttons, uint16_t x, uint16_t y) {
  if ((x < buttons->x) || (x > (buttons->x + buttons->w))) return false;
  if ((y < buttons->y) || (y > (buttons->y + buttons->h))) return false;
  return true;
}

// Signal state of buttons: true = pressed, false = released
void buttonPress(struct Gbuttons *buttons, boolean p) {
  buttons->laststate = buttons->currstate;
  buttons->currstate = p;
}

// Check the current state of a button: pressed/released
boolean buttonIsPressed(struct Gbuttons *buttons) {
	return buttons->currstate;
}

// Check if button was just pressed
boolean buttonJustPressed(struct Gbuttons *buttons) {
	return (buttons->currstate && !buttons->laststate);
 }

// Check if button was just released
boolean buttonJustReleased(struct Gbuttons *buttons) {
	return (!buttons->currstate && buttons->laststate);
}
//==========================================================================================

// Check for Touch Screen being touched
boolean TStouched(void) {
//    return touch.isTouching();
return false;
}

// Get Touch Screen x/y coords.
void getTSpoint(uint16_t *x, uint16_t *y) {
//	touch.getPosition(x,y);
}

// Put a picture on the screen using raw picture data
void putPicture_16bppData8(ru16 x, ru16 y, ru16 w, ru16 h, const unsigned char *data) {
	tft.putPicture_16bppData8(x, y, w, h, data);
}

// Scroll the screen down one text line
void scroll_down(void) {
	tft.scroll_down();
}

// Scroll the screen up one text line
void scroll_up(void) {
	tft.scroll();
}
	
// Used by VT100 routines
void clreol(void) {
	tft.clreol();  // clear to end of line
}

void clreos(void) {
	tft.clreos();  // clear to end of screen 
}

void clrbol(void) {
	tft.clrbol();  // clear to begining of line
}

void clrbos(void) {
	tft.clrbos();  // clear to begining of screen
}

void clrlin(void) {
	tft.clrlin();  // clear the current line
}

// Display time/date on the status bar
void displayTime(void)
{
	uint16_t tempFNTHeight = tft._FNTheight;
	uint16_t tempFNTWidth = tft._FNTwidth;
	
	timeval = rtc_get();
//printf("timeval = %lu\n",timeval);
	time_p = localtime( &timeval );
	strftime(datestr,80,"%a %b %d %Y %I:%M:%S %p", time_p);
//	mktime(datestr,80,"%a %b %d %Y %I:%M:%S %p", time_p);
	if(tft.RA8876_BUSY == false) { // do not write to display if SPI is busy
		tft.check2dBusy();
		tft._FNTheight = 24;
		tft._FNTwidth = 16;
		tft.buildTextScreen();
		tft.writeStatusLine((43*(tft._FNTwidth*tft._scaleX))+8, tft._TXTForeColor, tft._TXTBackColor,datestr);
		tft._FNTheight = tempFNTHeight;
		tft._FNTwidth = tempFNTWidth;
		tft.buildTextScreen();
	}
}

// turn on time display on status line
void timeOn(void) {
	//Start a PIT counter to display time on status bar every second
	timeDisplay.begin(displayTime,1000000);
	displayTime();
}

// turn off time display on status line
void timeOff(void) {
	//Stop a PIT counter to display time on status bar every second
	timeDisplay.end();
}

//=======================================================================
//= RA8876 BTE functions (Block Transfer Engine)                        =
//=======================================================================
// Copy box size part of the current screen page to another screen page
uint32_t tft_boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0) {
	return tft.boxPut(vPageAddr, x0, y0, x1, y1, dx0, dy0);
}

// Copy a box size part of another screen page to the current screen page
uint32_t tft_boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0) {
	return tft.boxGet(vPageAddr, x0, y0, x1, y1, dx0, dy0);
}

//=======================================================================
//= PIP window function (Display one of two or both PIP windows)        =
//=======================================================================
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
) {
	tft.PIP (On_Off, Select_PIP, PAddr, XP, YP, ImageWidth, X_Dis, Y_Dis, X_W, Y_H);
}
