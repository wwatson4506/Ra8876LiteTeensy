/*File Name : cpp                                   
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
#include "Arduino.h"
#include "SPI.h"
#include "Ra8876_Lite.h"
#include "RA8876_t3.h"
//#include "XPT2046.h"

// Touch Screen is not enabled at this time.
// Uses a modified version of XPT2046. WIP
// Touch Screen CS Pin
#define CS_PIN	31
// Touch Screen Interrupt Pin
#define TIRQ_PIN	24

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
RA8876_t3::RA8876_t3(const uint8_t CSp, const uint8_t RSTp, const uint8_t mosi_pin, const uint8_t sclk_pin, const uint8_t miso_pin) : Ra8876_Lite( CSp, RSTp, mosi_pin, sclk_pin, miso_pin)
{
}

// Initialize Touch Screen driver
//XPT2046 touch(CS_PIN, TIRQ_PIN);

boolean RA8876_t3::begin(void) {
	// Setup Touch Screen Pin as Output
//	pinMode(CS_PIN,OUTPUT);
//	digitalWrite(CS_PIN, HIGH);
	// Setup Backlite enable pin and turn backlite on - move to sketch

	// Initialize RA8876 driver
	if(!Ra8876_begin())
		return false;

	displayOn(true);	// Turn on TFT display
	UDFont = false;     // Turn off user define fonts
	_scaleX = 1;	// Init Font Scale (x,y)
	_scaleY = 1;
	_FNTwidth = 8;   //8 or 12 or 16; // Default font width
	_FNTheight = 16; // 16 or 24 or 32; // Default font height;

	// Setup graphic cursor
	Set_Graphic_Cursor_Color_1(0xff); // Foreground color
	Set_Graphic_Cursor_Color_2(0x00); // Outline color
	Graphic_cursor_initial();  // Initialize Graphic Cursor
    Select_Graphic_Cursor_2(); // Select Arrow Graphic Cursor
	// Set default foreground and background colors
	setTextColor(_TXTForeColor, _TXTBackColor);	
	// Position text cursor to default
	setTextCursor(_scrollXL, _scrollYT);
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
	currentPage = 999; // Don't repeat screen page 1 init.
	selectScreen(PAGE1_START_ADDR);	// Init page 1 screen
	fillScreen(COLOR65K_DARKBLUE);     // Not sure why we need to clear the screen twice
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE2_START_ADDR);	// Init page 2 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE3_START_ADDR);	// Init page 3 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE4_START_ADDR);	// Init page 4 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE5_START_ADDR);	// Init page 5 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE6_START_ADDR);	// Init page 6 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE7_START_ADDR);	// Init page 7 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE8_START_ADDR);	// Init page 8 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE9_START_ADDR);	// Init page 9 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE10_START_ADDR);	// Init page 10 screen
	fillScreen(COLOR65K_DARKBLUE);
	fillStatusLine(COLOR65K_DARKBLUE);
	selectScreen(PAGE1_START_ADDR); // back to page 1 screen
	
	//start Touch Screen
//	touch.begin(width(),height());

	// Set graphic mouse cursor to center of screen
	gcursorxy(width() / 2, height() / 2);
	return true;
}

// Select a screen page (Buffer) 1 to 9.
// ALT + (F1 to F9) using USBHost_t36 Keyboard Driver
// and my USBKeyboard host driver.
// Also, STBASIC Commands: screen 0 to screen 8
void RA8876_t3::selectScreen(uint32_t screenPage) {
	check2dBusy();
	tftSave_t *tempSave, *tempRestore;
	// Don't Select the current screen page
	if(screenPage == currentPage)
		return;
	switch(currentPage) {
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
	pageOffset = screenPage;
	currentPage = screenPage;
	displayImageStartAddress(currentPage);
	displayImageWidth(SCREEN_WIDTH);
	displayWindowStartXY(0,0);
	canvasImageStartAddress(currentPage);
	canvasImageWidth(SCREEN_WIDTH);
	activeWindowXY(0,0);
	activeWindowWH(SCREEN_WIDTH,600); 
	setTextCursor(_cursorX, _cursorY);
	textColor(_TXTForeColor,_TXTBackColor);
	// Rebuild the display
	buildTextScreen();
	
}

// Save current screen page parameters 
void RA8876_t3::saveTFTParams(tftSave_t *screenSave) {
	screenSave->width			= _width;
	screenSave->height			= _height;
	screenSave->cursorX			= _cursorX;
	screenSave->cursorY			= _cursorY;
	screenSave->scaleX			= _scaleX;
	screenSave->scaleY			= _scaleY;
	screenSave->FNTwidth		= _FNTwidth;
	screenSave->FNTheight		= _FNTheight;
	screenSave->fontheight		= _fontheight;
	screenSave->fontSource      = currentFont;
	screenSave->cursorXsize		= _cursorXsize;
	screenSave->cursorYsize		= _cursorYsize;
// Text Sreen Vars
	screenSave->prompt_size		= prompt_size; // prompt ">"
	screenSave->prompt_line		= prompt_line; // current text prompt row
	screenSave->vdata			= vdata;
	screenSave->leftmarg		= leftmarg;
	screenSave->topmarg			= topmarg;
	screenSave->rightmarg		= rightmarg;
	screenSave->bottommarg		= bottommarg;
	screenSave->tab_size		= tab_size;
	screenSave->CharPosX		= CharPosX;
	screenSave->CharPosY		= CharPosY;
	screenSave->UDFont  		= UDFont;

//scroll vars ----------------------------
	screenSave->scrollXL		= _scrollXL;
	screenSave->scrollXR		= _scrollXR;
	screenSave->scrollYT		= _scrollYT;
	screenSave->scrollYB		= _scrollYB;
	screenSave->TXTForeColor	= _TXTForeColor;
    screenSave->TXTBackColor	= _TXTBackColor;
}

// Restore selected screen page parameters
void RA8876_t3::restoreTFTParams(tftSave_t *screenSave) {
	 _width = screenSave->width;
	 _height = screenSave->height;
	 _cursorX = screenSave->cursorX;
	 _cursorY = screenSave->cursorY;
	 _scaleX = screenSave->scaleX;
	 _scaleY = screenSave->scaleY;
	 _FNTwidth = screenSave->FNTwidth;
	 _FNTheight = screenSave->FNTheight;
	 _fontheight = screenSave->fontheight;
	 currentFont = screenSave->fontSource;
	 _cursorXsize = screenSave->cursorXsize;		
	 _cursorYsize = screenSave->cursorYsize;		
	// Text Sreen Vars
	 prompt_size = screenSave->prompt_size; // prompt ">"
	 prompt_line = screenSave->prompt_line; // current text prompt row
	 vdata = screenSave->vdata;			
	 leftmarg = screenSave->leftmarg;		
	 topmarg = screenSave->topmarg;		
	 rightmarg = screenSave->rightmarg;
	 bottommarg = screenSave->bottommarg;		
	 tab_size =	screenSave->tab_size;		
	 CharPosX = screenSave->CharPosX;		
	 CharPosY = screenSave->CharPosY;		
	 UDFont = screenSave->UDFont;		
	//scroll vars -----------------
	 _scrollXL = screenSave->scrollXL;
	 _scrollXR = screenSave->scrollXR;
	 _scrollYT = screenSave->scrollYT;
	 _scrollYB = screenSave->scrollYB;
	 _TXTForeColor = screenSave->TXTForeColor;
	 _TXTBackColor = screenSave->TXTBackColor;
}

void RA8876_t3::useCanvas()
{
	displayImageStartAddress(PAGE1_START_ADDR);
	displayImageWidth(_width);
	displayWindowStartXY(0,0);
	
	canvasImageStartAddress(PAGE2_START_ADDR);
	canvasImageWidth(_width);
	activeWindowXY(0, 0);
	activeWindowWH(_width, _height);
	check2dBusy();
	ramAccessPrepare();
}

void RA8876_t3::updateScreen() {
	bteMemoryCopy(PAGE2_START_ADDR,_width,0,0,
				  PAGE1_START_ADDR,_width, 0,0,
				 _width,_height);	
}

// Setup text cursor
void RA8876_t3::cursorInit(void)
{
	_cursorXsize = _FNTwidth;
	_cursorYsize = _FNTheight-1;
	Text_Cursor_H_V(_cursorXsize,_cursorYsize); 		// Block cusror
	Blinking_Time_Frames(20);		// Set blink rate
	Enable_Text_Cursor_Blinking();	// Turn blinking cursor on
}

void RA8876_t3::setCursor(uint16_t x, uint16_t y)
{
	setTextCursor(x, y);
}

//Set Graphic Mode Margins (pixel based)
void RA8876_t3::setMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb) {
	_scrollXL = xl;
	_scrollYT = yt;
	_scrollXR = xr;
	_scrollYB = yb;
	buildTextScreen();	
	setTextCursor(    _scrollXL,    _scrollYT);
}

//Set Text Mode Margins (font size based)
void RA8876_t3::setTMargins(uint16_t xl, uint16_t yt, uint16_t xr, uint16_t yb) {
	_scrollXL = xl*    _FNTwidth;
	_scrollYT = yt*    _FNTheight;
	_scrollXR = _width-(xr*    _FNTwidth);
	_scrollYB = _height-(yb*    _FNTheight);
	buildTextScreen();	
	setTextCursor(    _scrollXL,    _scrollYT);
}

// Set text prompt size (font size based)
void RA8876_t3::setPromptSize(uint16_t ps) {
	prompt_size = ps*(    _FNTwidth *     _scaleX) +     _scrollXL; // Default prompt ">"
}

// Clear current screen to background 'color'
void RA8876_t3::fillScreen(uint16_t color) {
	drawSquareFill(_scrollXL, _scrollYT, _scrollXR, _scrollYB, color);
	check2dBusy();  //must wait for fill to finish before setting foreground color
	textColor(_TXTForeColor,_TXTBackColor);
	setTextCursor(_scrollXL,_scrollYT);
}

// Clear Status Line to background 'color'
void RA8876_t3::fillStatusLine(uint16_t color) {
	uint16_t temp = _TXTBackColor;
	clearStatusLine(color);
	_TXTBackColor = temp;
	textColor(_TXTForeColor,temp);
}

//**************************************************************//
// Note that, unlike the RA88875, this does not set BG transparent!
//**************************************************************//
void RA8876_t3::setTextColor(uint16_t color)
{
	foreGroundColor16bpp(color);
}

//**************************************************************//
//**************************************************************//
void RA8876_t3::setBackGroundColor(uint16_t color)
{
	backGroundColor16bpp(color);
}

// Set text foreground + background colors
void RA8876_t3::setTextColor(uint16_t fgc, uint16_t bgc) {
	textColor(fgc,bgc);
}


// Send a character directly to video memory (No control ASCII codes are processed).
void RA8876_t3::tftRawWrite(uint8_t data) {
	rawPrint(data);
}
	
// Send a string to the status line
void RA8876_t3::printStatusLine(uint16_t x0,uint16_t fgColor,uint16_t bgColor, const char *text) {
	writeStatusLine(x0*(_FNTwidth*_scaleX), fgColor, bgColor, text);	
}

// Load a user defined font from memory to RA8876 Character generator RAM
uint8_t RA8876_t3::fontLoadMEM(char *fontsrc) {
	CGRAM_initial(PATTERN1_RAM_START_ADDR, (uint8_t *)fontsrc, 16*256);
	return 0;
}

// Set USE_FF_FONTLOAD in h to 1 to use fontload function.
// Requires FatFS or Can be modified to use SDFat or SD.
#if USE_FF_FONTLOAD == 1
#include "ff.h"
// Load a user defined font from a file to RA8876 Character generator RAM
uint8_t RA8876_t3::fontLoad(char *fontfile) {
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
	CGRAM_initial(PATTERN1_RAM_START_ADDR, fontdata, 16*256);
	return 0;
}
#endif

// Select internal fonts or user defined fonts (0 for internal or 1 for user defined)
void RA8876_t3::setFontSource(uint8_t source) {
	switch(source) {
	case 0:
		_scaleX = 1;
		_scaleY = 1;
		break;
	case 1:
		if((_FNTheight != 16) && (_FNTwidth != 8)) {
			_FNTheight = 16;
			_FNTwidth  = 8;
		}
		break;
	}
	Ra8876_Lite::setFontSource(source);
	// Rebuild current screen page
	buildTextScreen();
}

// Set fontsize for fonts, currently 0 to 2 (Internal and User Defined)
boolean RA8876_t3::setFontSize(uint8_t scale, boolean runflag) {
	switch(scale) {
		
		case 0:
			if(UDFont) { // User Defined Fonts
				_scaleY = 1;
				_scaleX = 1;
			} else {
				_FNTheight = 16;
				_FNTwidth  = 8;
			}
			break;
		case 1:
			if(UDFont) {
				_FNTheight = 16;
				_FNTwidth  = 8;
				_scaleY = 2;
				_scaleX = 2;
			} else {
				_FNTheight = 24;
				_FNTwidth  = 12;
			}
			break;
		case 2:
			if(UDFont) {
				_FNTheight = 16;
				_FNTwidth  = 8;
				_scaleY = 3;
				_scaleX = 3;
			} else {
				_FNTheight = 32;
				_FNTwidth  = 16;
			}
			break;
		default:
			return true;
			break;
		setTextCursor(_scrollXL,_scrollYT);
	}
	// Rebuild current screen page
	buildTextScreen();
	cursorInit();
	if(runflag == false) {
		drawSquareFill(0, 0, _width-1, _height-1, _TXTBackColor);
		//have to wait for fill to finish before changing the foreground color...
		check2dBusy();
		textColor(_TXTForeColor,_TXTBackColor);
		setTextCursor(0,0);
	}
	rightmarg = (uint8_t)(_width / (_FNTwidth * _scaleX));
	bottommarg = (uint8_t)(_height / (_FNTheight * _scaleY));
	
	return false;
}

// Text cursor on or off
void RA8876_t3::setCursorMode(boolean mode) {

	if(mode == false)
		Disable_Text_Cursor();	// No cursor
	else
		Enable_Text_Cursor();	// Blinking block cursor
}

// Set text cursor type
void RA8876_t3::setCursorType(uint8_t type) {

	switch(type) {
		case 0:
			_cursorYsize = _FNTheight;
			Text_Cursor_H_V(_cursorXsize, _cursorYsize); 		// Block cursor
			break;
		case 1:
			_cursorYsize = 2;
			Text_Cursor_H_V(_cursorXsize, _cursorYsize); 		// Underline cursor
			break;
		case 2:
			_cursorYsize = _FNTheight;
			Text_Cursor_H_V(2, _cursorYsize); 						// I-beam cursor
			break;
		default:
			break;
	}
}

// Set text cursor blink mode (On or Off)
void RA8876_t3::setCursorBlink(boolean onOff) {
	if(onOff)
		Enable_Text_Cursor_Blinking();
	else
		Disable_Text_Cursor_Blinking();
}
	
// If you're looking to set text write position in characters within the current scroll window then use textxy()

// Get cursor X position
int16_t RA8876_t3::getTextX(void) {
	return (_cursorX / _FNTwidth); //getCursorX() / getFontWidth();
}	
	
// Get cursor Y position
int16_t RA8876_t3::getTextY(void) {
	return (_cursorY / _FNTheight);//getCursorY() / getFontHeight();
}	

// Get current text screen width (pixel * current font width)
int16_t RA8876_t3::getTwidth(void) {
	return (rightmarg - leftmarg)-1;
}

// Get current text screen height (pixel * current font height)
int16_t RA8876_t3::getTheight(void) {
	return (bottommarg - topmarg)-1;
}

// Get current screen foreground color
uint16_t RA8876_t3::getTextFGC() {
	return _TXTForeColor;
}

// Get current screen backround color
uint16_t RA8876_t3::getTextBGC(void) {
	return _TXTBackColor;
}


// Get font vertical size (in pixels)
uint8_t RA8876_t3::getFontHeight(void) {
	return _FNTheight * _scaleY;
}

// Get font width size (in pixels)
uint8_t RA8876_t3::getFontWidth(void) {
	return _FNTwidth * _scaleX;
}

// Draw a rectangle. Note: damages text color register
void RA8876_t3::drawRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	drawSquare(x, y, x+w-1, y+h-1, color);
}

// Draw a filled rectangle. Note: damages text color register
void RA8876_t3::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color) {
	drawSquareFill(x, y, x+w-1, y+h-1, color);
}

// Draw a round rectangle. 
void RA8876_t3::drawRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color) {
	drawCircleSquare(x, y, x+w-1, y+h-1, xr, yr, color);
}

// Draw a filed round rectangle.
void RA8876_t3::fillRoundRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t xr, uint16_t yr, uint16_t color) {
	drawCircleSquareFill(x, y, x+w-1, y+h-1, xr, yr, color);
}

// Enable Touch Screen.
void RA8876_t3::touchEnable(boolean enabled) {
//		touchEnable(enabled);
}

// Return true if screen touched else false
boolean touchDetect(boolean autoclear) {
//	return touchDetect(autoclear);
return false; // return false for now
}

// Draw a filled circle
void RA8876_t3::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	drawCircleFill(x0, y0, r, color);
}

// Draw a filled ellipse.
void RA8876_t3::fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color) {
	drawEllipseFill(xCenter, yCenter, longAxis, shortAxis, color);
}

// Draw a filled triangle
void RA8876_t3::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
	drawTriangleFill(x0, y0, x1, y1, x2, y2, color);
}

// Read touch screen ADC
void RA8876_t3::readTouchADC(uint16_t *x, uint16_t *y) {
//	touchReadAdc(x, y);//we using 10bit adc data here
}

// Setup graphic mouse cursor
void RA8876_t3::gCursorSet(boolean gCursorEnable, uint8_t gcursortype, uint8_t gcursorcolor1, uint8_t gcursorcolor2) {
	if (!gCursorEnable)
		Disable_Graphic_Cursor();
	else
		Enable_Graphic_Cursor();
	switch(gcursortype) {
		case 1:
			Select_Graphic_Cursor_1(); // PEN
			break;
		case 2:
			Select_Graphic_Cursor_2(); // ARROW
			break;
		case 3:
			Select_Graphic_Cursor_3(); // HOUR GLASS
			break;
		case 4:
			Select_Graphic_Cursor_4(); // ERROR SYMBOL
			break;
		default:
			Select_Graphic_Cursor_2();
	}
	Set_Graphic_Cursor_Color_1(gcursorcolor1);
	Set_Graphic_Cursor_Color_2(gcursorcolor2);
}

// Set graphic cursor position on screen
void RA8876_t3::gcursorxy(uint16_t gcx, uint16_t gcy) {
	Graphic_Cursor_XY(gcx, gcy);
}

//==========================================================================================
//= The following Graphic Button functions are based on Adafruits Graphic button libraries =
//==========================================================================================
// Initialize a graphic button. 
void RA8876_t3::initButton(struct Gbuttons *buttons, uint16_t x, uint16_t y, uint8_t w, uint8_t h,
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
void RA8876_t3::drawButton(struct Gbuttons *buttons, boolean inverted) {
  uint16_t fill, outline;
//  uint16_t text;
  uint16_t fgcolor = _TXTForeColor;	
  uint16_t bgcolor = _TXTBackColor;	

  if(!inverted) {
    fill    = buttons->fillcolor;
    outline = buttons->outlinecolor;
//    text    = buttons->textcolor;
  } else {
    fill    = buttons->textcolor;
    outline = buttons->outlinecolor;
//    text    = buttons->fillcolor;
  }
  drawCircleSquareFill(buttons->x, buttons->y, buttons->w+buttons->x, buttons->h+buttons->y,
					   min(buttons->w,buttons->h)/4, min(buttons->w,buttons->h)/4, fill);

  drawCircleSquare(buttons->x, buttons->y, buttons->w+buttons->x, buttons->h+buttons->y,
				   min(buttons->w,buttons->h)/4, min(buttons->w,buttons->h)/4, outline);

//  _gfx->setCursor(_x - strlen(_label)*3*_textsize, _y-4*_textsize);
//  _gfx->setTextColor(text);
//  _gfx->setTextSize(_textsize);
//  _gfx->print(_label);
  textColor(fgcolor,bgcolor);	// restore colors	
}

// Check the button to see if it is within the range of selection.
boolean RA8876_t3::buttonContains(struct Gbuttons *buttons, uint16_t x, uint16_t y) {
  if ((x < buttons->x) || (x > (buttons->x + buttons->w))) return false;
  if ((y < buttons->y) || (y > (buttons->y + buttons->h))) return false;
  return true;
}

// Signal state of buttons: true = pressed, false = released
void RA8876_t3::buttonPress(struct Gbuttons *buttons, boolean p) {
  buttons->laststate = buttons->currstate;
  buttons->currstate = p;
}

// Check the current state of a button: pressed/released
boolean RA8876_t3::buttonIsPressed(struct Gbuttons *buttons) {
	return buttons->currstate;
}

// Check if button was just pressed
boolean RA8876_t3::buttonJustPressed(struct Gbuttons *buttons) {
	return (buttons->currstate && !buttons->laststate);
 }

// Check if button was just released
boolean RA8876_t3::buttonJustReleased(struct Gbuttons *buttons) {
	return (!buttons->currstate && buttons->laststate);
}
//==========================================================================================

// Check for Touch Screen being touched
boolean RA8876_t3::TStouched(void ) {
//    return touch.isTouching();
return false;
}

// Get Touch Screen x/y coords.
void RA8876_t3::getTSpoint(uint16_t *x, uint16_t *y) {
//	touch.getPosition(x,y);
}

// Put a picture on the screen using raw picture data
// This is a simplified wrapper - more advanced uses (such as putting data onto a page other than current) 
//   should use the underlying BTE functions.
void RA8876_t3::putPicture(ru16 x, ru16 y, ru16 w, ru16 h, const unsigned char *data) {
	//The putPicture_16bppData8 function in the base class is not ideal - it damages the activeWindow setting
	//It also is harder to make it DMA.
	//Ra8876_Lite::putPicture_16bppData8(x, y, w, h, data);

	//Using the BTE function is faster and will use DMA if available
    bteMpuWriteWithROPData8(currentPage, width(), x, y,  //Source 1 is ignored for ROP 12
                              currentPage, width(), x, y, w, h,     //destination address, pagewidth, x/y, width/height
                              RA8876_BTE_ROP_CODE_12,
                              data);
}


// Scroll the screen up one text line
void RA8876_t3::scrollUp(void ) {
	scroll();
}

//=======================================================================
//= RA8876 BTE functions (Block Transfer Engine)                        =
//=======================================================================
// Copy box size part of the current screen page to another screen page
/*uint32_t RA8876_t3::tft_boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0) {
	return boxPut(vPageAddr, x0, y0, x1, y1, dx0, dy0);
}

// Copy a box size part of another screen page to the current screen page
uint32_t RA8876_t3::tft_boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
					uint16_t x1, uint16_t y1, uint16_t dx0, uint16_t dy0) {
	return boxGet(vPageAddr, x0, y0, x1, y1, dx0, dy0);
}
*/

//=======================================================================
//= PIP window function (Display one of two or both PIP windows)        =
//=======================================================================
/*void RA8876_t3::tft_PIP (
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
	PIP (On_Off, Select_PIP, PAddr, XP, YP, ImageWidth, X_Dis, Y_Dis, X_W, Y_H);
}
*/
