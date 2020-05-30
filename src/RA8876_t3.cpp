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
	// Initialize RA8876 driver
	if(!Ra8876_begin())
		return false;

	displayOn(true);	// Turn on TFT display
	UDFont = false;     // Turn off user define fonts
	_scaleX = 1;	// Init Font Scale (x,y)
	_scaleY = 1;
	_FNTwidth = 8;   //8 or 12 or 16; // Default font width
	_FNTheight = 16; // 16 or 24 or 32; // Default font height;
	
	_activeWindowXL = 0;
	_activeWindowXR = SCREEN_WIDTH;
	_activeWindowYT = 0;
	_activeWindowYB = SCREEN_HEIGHT;
	_portrait = true;
	_backTransparent = false;


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

/**************************************************************************/
/*!   
		Give you back the current text cursor position by reading inside RA8875
		Parameters:
		x: horizontal pos in pixels
		y: vertical pos in pixels
		note: works also with rendered fonts
		USE: xxx.getCursor(myX,myY);
*/
/**************************************************************************/
void RA8876_t3::getCursor(int16_t &x, int16_t &y) 
{
		uint8_t t1,t2,t3,t4;
		t1 = lcdRegDataRead(RA8876_F_CURX0);
		t2 = lcdRegDataRead(RA8876_F_CURX1);
		t3 = lcdRegDataRead(RA8876_F_CURY0);
		t4 = lcdRegDataRead(RA8876_F_CURY1);
		x = (t2 << 8) | (t1 & 0xFF);
		y = (t4 << 8) | (t3 & 0xFF);
		//if (_portrait && _use_default) swapvals(x,y);
	
}

int16_t RA8876_t3::getCursorX(void)
{
	//if (_portrait && _use_default) return _cursorY;
	return _cursorX;
}

int16_t RA8876_t3::getCursorY(void)
{
	//if (_portrait && _use_default) return _cursorX;
	return _cursorY;
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
		_setFNTdimensions(0);
		break;
	case 1:
		if((_FNTheight != 16) && (_FNTwidth != 8)) {
			_FNTheight = 16;
			_FNTwidth  = 8;
		}
		_setFNTdimensions(1);
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


void RA8876_t3::_fontWrite(const uint8_t* buffer, uint16_t len)
{
	if(_use_default) {
		//if (_FNTgrandient) _FNTgrandient = false;//cannot use this with write
		//_textWrite((const char *)buffer, len);
		//Serial.printf("Default: %c, %d, %d\n", c, _cursorX, _cursorY);
		return;
	}
	// Lets try to handle some of the special font centering code that was done for default fonts.
	if (_absoluteCenter || _relativeCenter ) {
		int16_t x, y;
	  	uint16_t strngWidth, strngHeight;
	  	getTextBounds(buffer, len, 0, 0, &x, &y, &strngWidth, &strngHeight);
	  	//Serial.printf("_fontwrite bounds: %d %d %u %u\n", x, y, strngWidth, strngHeight);
	  	// Note we may want to play with the x ane y returned if they offset some
		if (_absoluteCenter && strngWidth > 0){//Avoid operations for strngWidth = 0
			_absoluteCenter = false;
			_cursorX = _cursorX - ((x + strngWidth) / 2);
			_cursorY = _cursorY - ((y + strngHeight) / 2);
		} else if (_relativeCenter && strngWidth > 0){//Avoid operations for strngWidth = 0
			_relativeCenter = false;
			if (bitRead(_TXTparameters,5)) {//X = center
				_cursorX = (_width / 2) - ((x + strngWidth) / 2);
				_TXTparameters &= ~(1 << 5);//reset
			}
			if (bitRead(_TXTparameters,6)) {//Y = center
				_cursorY = (_height / 2) - ((y + strngHeight) / 2) ;
				_TXTparameters &= ~(1 << 6);//reset
			}
		}
	}

	while(len) {
		uint8_t c = *buffer++;
		if (font) {
			//Serial.printf("ILI: %c, %d, %d\n", c, _cursorX, _cursorY);
			if (c == '\n') {
				//_cursorY += font->line_space;
				//_cursorX  = 0;
			} else {
			  if (c == 13) {
				_cursorY += font->line_space;
				_cursorX  = 0;
			  } else {
				drawFontChar(c);
			  }
			}
		} else if (gfxFont)  {
			//Serial.printf("GFX: %c, %d, %d\n", c, _cursorX, _cursorY);
			if (c == '\n') {
	            _cursorY += (int16_t)textsize_y * gfxFont->yAdvance;
				_cursorX  = 0;
			} else {
				drawGFXFontChar(c);
			}
		} else {
			if (c == '\n') {
				_cursorY += textsize_y*8;
				_cursorX  = 0;
			} else if (c == '\r') {
				// skip em
			} else {
				drawChar(_cursorX, _cursorY, c, textcolor, textbgcolor, textsize_x, textsize_y);
				_cursorX += textsize_x*6;
				if (wrap && (_cursorX > (_width - textsize_x*6))) {
					_cursorY += textsize_y*6;
					_cursorX = 0;
				}
			}
		}
		len--;
	}
}


//#include "glcdfont.c"
extern "C" const unsigned char glcdfont[];

// Draw a character
void RA8876_t3::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t fgcolor, uint16_t bgcolor, uint8_t size_x, uint8_t size_y)
{
	if((x >= _width)            || // Clip right
	   (y >= _height)           || // Clip bottom
	   ((x + 6 * size_x - 1) < 0) || // Clip left  TODO: is this correct?
	   ((y + 8 * size_y - 1) < 0))   // Clip top   TODO: is this correct?
		return;

	//Serial.printf("drawchar %d %d %c %x %x %d %d\n", x, y, c, fgcolor, bgcolor, size_x, size_y);
	if (_TXTForeColor == _TXTBackColor) {
		// This transparent approach is only about 20% faster
		if ((size_x == 1) && (size_y == 1)) {
			uint8_t mask = 0x01;
			int16_t xoff, yoff;
			for (yoff=0; yoff < 8; yoff++) {
				uint8_t line = 0;
				for (xoff=0; xoff < 5; xoff++) {
					if (glcdfont[c * 5 + xoff] & mask) line |= 1;
					line <<= 1;
				}
				line >>= 1;
				xoff = 0;
				while (line) {
					if (line == 0x1F) {
						drawFastHLine(x + xoff, y + yoff, 5, fgcolor);
						break;
					} else if (line == 0x1E) {
						drawFastHLine(x + xoff, y + yoff, 4, fgcolor);
						break;
					} else if ((line & 0x1C) == 0x1C) {
						drawFastHLine(x + xoff, y + yoff, 3, fgcolor);
						line <<= 4;
						xoff += 4;
					} else if ((line & 0x18) == 0x18) {
						drawFastHLine(x + xoff, y + yoff, 2, fgcolor);
						line <<= 3;
						xoff += 3;
					} else if ((line & 0x10) == 0x10) {
						drawPixel(x + xoff, y + yoff, fgcolor);
						line <<= 2;
						xoff += 2;
					} else {
						line <<= 1;
						xoff += 1;
					}
				}
				mask = mask << 1;
			}
		} else {
			uint8_t mask = 0x01;
			int16_t xoff, yoff;
			for (yoff=0; yoff < 8; yoff++) {
				uint8_t line = 0;
				for (xoff=0; xoff < 5; xoff++) {
					if (glcdfont[c * 5 + xoff] & mask) line |= 1;
					line <<= 1;
				}
				line >>= 1;
				xoff = 0;
				while (line) {
					if (line == 0x1F) {
						fillRect(x + xoff * size_x, y + yoff * size_y,
							5 * size_x, size_y, fgcolor);
						break;
					} else if (line == 0x1E) {
						fillRect(x + xoff * size_x, y + yoff * size_y,
							4 * size_x, size_y, fgcolor);
						break;
					} else if ((line & 0x1C) == 0x1C) {
						fillRect(x + xoff * size_x, y + yoff * size_y,
							3 * size_x, size_y, fgcolor);
						line <<= 4;
						xoff += 4;
					} else if ((line & 0x18) == 0x18) {
						fillRect(x + xoff * size_x, y + yoff * size_y,
							2 * size_x, size_y, fgcolor);
						line <<= 3;
						xoff += 3;
					} else if ((line & 0x10) == 0x10) {
						fillRect(x + xoff * size_x, y + yoff * size_y,
							size_x, size_y, fgcolor);
						line <<= 2;
						xoff += 2;
					} else {
						line <<= 1;
						xoff += 1;
					}
				}
				mask = mask << 1;
			}
		}
	} else {
		// This solid background approach is about 5 time faster
		uint8_t xc, yc;
		uint8_t xr, yr;
		uint8_t mask = 0x01;

		// We need to offset by the origin.
		x+=_originx;
		y+=_originy;
		int16_t x_char_start = x;  // remember our X where we start outputting...

		if((x >= _displayclipx2)            || // Clip right
			 (y >= _displayclipy2)           || // Clip bottom
			 ((x + 6 * size_x - 1) < _displayclipx1) || // Clip left  TODO: this is not correct
			 ((y + 8 * size_y - 1) < _displayclipy1))   // Clip top   TODO: this is not correct
			return;

			// need to build actual pixel rectangle we will output into.
			int16_t y_char_top = y;	// remember the y
			int16_t w =  6 * size_x;
			int16_t h = 8 * size_y;

			if(x < _displayclipx1) {	w -= (_displayclipx1-x); x = _displayclipx1; 	}
			if((x + w - 1) >= _displayclipx2)  w = _displayclipx2  - x;
			if(y < _displayclipy1) {	h -= (_displayclipy1 - y); y = _displayclipy1; 	}
			if((y + h - 1) >= _displayclipy2) h = _displayclipy2 - y;

			//Serial.printf("%d, %d, %d, %d\n", x, y, x + w -1, y + h - 1);
			setActiveWindow(x, y, x + w -1, y + h - 1);
			//_startSend();
			y = y_char_top;	// restore the actual y.
			//writeCommand(RA8875_MRWC);
			for (yc=0; (yc < 8) && (y < _displayclipy2); yc++) {
				for (yr=0; (yr < size_y) && (y < _displayclipy2); yr++) {
					x = x_char_start; 		// get our first x position...
					if (y >= _displayclipy1) {
						for (xc=0; xc < 5; xc++) {
							for (xr=0; xr < size_x; xr++) {
								if ((x >= _displayclipx1) && (x < _displayclipx2)) {
									//write16BitColor(fgcolor);
									drawPixel(xr+x,yc+y,fgcolor);
								}
								x++;
							}
						}
						for (xr=0; xr < size_x; xr++) {
							if ((x >= _displayclipx1) && (x < _displayclipx2)) {
								//write16BitColor(bgcolor);
								drawPixel(xr+x,yc+y,bgcolor);
							}
							x++;
						}
					}
					y++;
				}
				mask = mask << 1;
			}
			//writecommand_last(ILI9488_NOP);
			//_endSend();
	}
}

void RA8876_t3::setFont(const ILI9341_t3_font_t &f) {
	_use_default = 0;
	if(_portrait && !_use_gfx_font) {
		_cursorY += _cursorX;
		_cursorX -= _cursorY;
	}
	_use_ili_font = 1;
	_use_gfx_font = 0;
	_use_int_font = 1;
	_use_tfont = 0;
	
	_gfx_last_char_x_write = 0;	// Don't use cached data here
	font = &f;
	if (gfxFont) {
        _cursorY -= 6;
		gfxFont = NULL;
	}
	fontbpp = 1;
	// Calculate additional metrics for Anti-Aliased font support (BDF extn v2.3)
	if (font && font->version==23){
		fontbpp = (font->reserved & 0b000011)+1;
		fontbppindex = (fontbpp >> 2)+1;
		fontbppmask = (1 << (fontbppindex+1))-1;
		fontppb = 8/fontbpp;
		fontalphamx = 31/((1<<fontbpp)-1);
		// Ensure text and bg color are different. Note: use setTextColor to set actual bg color
		if (_TXTForeColor == _TXTBackColor) _TXTBackColor = (_TXTForeColor==0x0000)?0xFFFF:0x0000;	}

}


// Maybe support GFX Fonts as well?
void RA8876_t3::setFont(const GFXfont *f) {
	_use_default = 0;
	if(_portrait && !_use_ili_font) {
		_cursorY += _cursorX;
		_cursorX -= _cursorY;
	}
	_use_gfx_font = 1;
	_use_ili_font = 0;
	_use_int_font = 0;
	_use_tfont = 0;
	font = NULL;	// turn off the other font... 
	_gfx_last_char_x_write = 0;	// Don't use cached data here
	if (f == gfxFont) return;	// same font or lack of so can bail.

    if(f) {            // Font struct pointer passed in?
        if(!gfxFont) { // And no current font struct?
            // Switching from classic to new font behavior.
            // Move cursor pos down 6 pixels so it's on baseline.
            _cursorY += 6;
        }

        // Test wondering high and low of Ys here... 
        int8_t miny_offset = 0;
#if 1
        for (uint8_t i=0; i <= (f->last - f->first); i++) {
        	if (f->glyph[i].yOffset < miny_offset) {
        		miny_offset = f->glyph[i].yOffset;
        	}
        }
#else        
        int max_delta = 0;
        uint8_t index_min = 0;
        uint8_t index_max = 0;

        int8_t minx_offset = 127;
        int8_t maxx_overlap = 0;
        uint8_t indexx_min = 0;
        uint8_t indexx_max = 0;
        for (uint8_t i=0; i <= (f->last - f->first); i++) {
        	if (f->glyph[i].yOffset < miny_offset) {
        		miny_offset = f->glyph[i].yOffset;
        		index_min = i;
        	}

        	if (f->glyph[i].xOffset < minx_offset) {
        		minx_offset = f->glyph[i].xOffset;
        		indexx_min = i;
        	}
        	if ( (f->glyph[i].yOffset + f->glyph[i].height) > max_delta) {
        		max_delta = (f->glyph[i].yOffset + f->glyph[i].height);
        		index_max = i;
        	}
        	int8_t x_overlap = f->glyph[i].xOffset + f->glyph[i].width - f->glyph[i].xAdvance;
        	if (x_overlap > maxx_overlap) {
        		maxx_overlap = x_overlap;
        		indexx_max = i;
        	}
        }
        Serial.printf("Set GFX Font(%x): Y: %d %d(%c) %d(%c) X: %d(%c) %d(%c)\n", (uint32_t)f, f->yAdvance, 
        	miny_offset, index_min + f->first, max_delta, index_max + f->first,
        	minx_offset, indexx_min + f->first, maxx_overlap, indexx_max + f->first);
#endif
        _gfxFont_min_yOffset = miny_offset;	// Probably only thing we need... May cache? 

    } else if(gfxFont) { // NULL passed.  Current font struct defined?
        // Switching from new to classic font behavior.
        // Move cursor pos up 6 pixels so it's at top-left of char.
        _cursorY -= 6;
    }
    gfxFont = f;
}

	
void RA8876_t3::drawFontChar(unsigned int c)
{
	uint32_t bitoffset = 0;
	const uint8_t *data;

	//Serial.printf("drawFontChar(%c) %d (%d, %d) %x %x %x\n", c, c, _cursorX, _cursorY, _TXTBackColor, _TXTForeColor, _backTransparent);

	if (c >= font->index1_first && c <= font->index1_last) {
		bitoffset = c - font->index1_first;
		bitoffset *= font->bits_index;
	} else if (c >= font->index2_first && c <= font->index2_last) {
		bitoffset = c - font->index2_first + font->index1_last - font->index1_first + 1;
		bitoffset *= font->bits_index;
	} 

	// TODO: implement sparse unicode

	//Serial.printf("  index =  %d\n", fetchbits_unsigned(font->index, bitoffset, font->bits_index));
	data = font->data + fetchbits_unsigned(font->index, bitoffset, font->bits_index);

	uint32_t encoding = fetchbits_unsigned(data, 0, 3);
	if (encoding != 0) return;
	uint32_t width = fetchbits_unsigned(data, 3, font->bits_width);
	bitoffset = font->bits_width + 3;
	uint32_t height = fetchbits_unsigned(data, bitoffset, font->bits_height);
	bitoffset += font->bits_height;
	//Serial.printf("  size =   %d,%d\n", width, height);
	//Serial.printf("  line space = %d\n", font->line_space);
	//Serial.printf("  cap height = %d\n", font->cap_height);

	int32_t xoffset = fetchbits_signed(data, bitoffset, font->bits_xoffset);
	bitoffset += font->bits_xoffset;
	int32_t yoffset = fetchbits_signed(data, bitoffset, font->bits_yoffset);
	bitoffset += font->bits_yoffset;
	//Serial.printf("  offset = %d,%d\n", xoffset, yoffset);

	uint32_t delta = fetchbits_unsigned(data, bitoffset, font->bits_delta);
	bitoffset += font->bits_delta;
	//Serial.printf("  delta =  %d\n", delta);
	
	//horizontally, we draw every pixel, or none at all
	if (_cursorX < 0) _cursorX = 0;
	int32_t origin_x = _cursorX + xoffset;
	if (origin_x < 0) {
		_cursorX -= xoffset;
		origin_x = 0;
	}
	if (origin_x + (int)width > _width) {
		if (!wrap) return;
		origin_x = 0;
		if (xoffset >= 0) {
			_cursorX = 0;
		} else {
			_cursorX = -xoffset;
		}
		_cursorY += font->line_space;
	}

	if (_cursorY >= _height) return;

	// vertically, the top and/or bottom can be clipped
	int32_t origin_y = _cursorY + font->cap_height - height - yoffset;

	// TODO: compute top skip and number of lines
	int32_t linecount = height;
	//uint32_t loopcount = 0;
	int32_t y = origin_y;
	
	bool opaque = !_backTransparent; //(_TXTBackColor != _TXTForeColor);


	// Going to try a fast Opaque method which works similar to drawChar, which is near the speed of writerect
	if (!opaque) {
		// Anti-alias support
		if (fontbpp>1){
			// This branch should, in most cases, never happen. This is because if an
			// anti-aliased font is being used, textcolor and textbgcolor should always
			// be different. Even though an anti-alised font is being used, pixels in this
			// case will all be solid because pixels are rendered on same colour as themselves!
			// This won't look very good.
			bitoffset = ((bitoffset + 7) & (-8)); // byte-boundary
			uint32_t xp = 0;
			uint8_t halfalpha = 1<<(fontbpp-1);
			while (linecount) {
				uint32_t x = 0;
				while(x<width) {
					// One pixel at a time, either on (if alpha > 0.5) or off
					if (fetchpixel(data, bitoffset, xp)>=halfalpha){
						Pixel(origin_x + x,y, _TXTForeColor);
					}
					bitoffset += fontbpp;
					x++;
					xp++;
				}
				y++;
				linecount--;
			}
		}
		// Soild pixels
		else {
			while (linecount > 0) {
				//Serial.printf("    linecount = %d\n", linecount);
				uint32_t n = 1;
				if (fetchbit(data, bitoffset++) != 0) {
					n = fetchbits_unsigned(data, bitoffset, 3) + 2;
					bitoffset += 3;
				}
				uint32_t x = 0;
				do {
					int32_t xsize = width - x;
					if (xsize > 32) xsize = 32;
					uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
					//Serial.printf("    multi line %d %d %x\n", n, x, bits);
					drawFontBits(opaque, bits, xsize, origin_x + x, y, n);
					bitoffset += xsize;
					x += xsize;
				} while (x < width);


				y += n;
				linecount -= n;
				//if (++loopcount > 100) {
					//Serial.println("     abort draw loop");
					//break;
				//}
			}
		}
	} else {  //Opaque
		if (fontbpp>1){
			// Now opaque mode... 
			// Now write out background color for the number of rows above the above the character
			// figure out bounding rectangle... 
			// In this mode we need to update to use the offset and bounding rectangles as we are doing it it direct.
			// also update the Origin 
			int cursor_x_origin = _cursorX + _originx;
			int cursor_y_origin = _cursorY + _originy;
			_cursorX += _originx;
			_cursorY += _originy;

			int start_x = (origin_x < cursor_x_origin) ? origin_x : cursor_x_origin; 	
			if (start_x < 0) start_x = 0;
			
			int start_y = (origin_y < cursor_y_origin) ? origin_y : cursor_y_origin; 
			if (start_y < 0) start_y = 0;
			int end_x = cursor_x_origin + delta; 
			if ((origin_x + (int)width) > end_x)
				end_x = origin_x + (int)width;
			if (end_x >= _displayclipx2)  end_x = _displayclipx2;	
			int end_y = cursor_y_origin + font->line_space; 
			if ((origin_y + (int)height) > end_y)
				end_y = origin_y + (int)height;
			if (end_y >= _displayclipy2) end_y = _displayclipy2;	
			end_x--;	// setup to last one we draw
			end_y--;
			int start_x_min = (start_x >= _displayclipx1) ? start_x : _displayclipx1;
			int start_y_min = (start_y >= _displayclipy1) ? start_y : _displayclipy1;

			// See if anything is in the display area.
			if((end_x < _displayclipx1) ||(start_x >= _displayclipx2) || (end_y < _displayclipy1) || (start_y >= _displayclipy2)) {
				_cursorX += delta;	// could use goto or another indent level...
				return;
			}
/*
			Serial.printf("drawFontChar(%c) %d\n", c, c);
			Serial.printf("  size =   %d,%d\n", width, height);
			Serial.printf("  line space = %d\n", font->line_space);
			Serial.printf("  offset = %d,%d\n", xoffset, yoffset);
			Serial.printf("  delta =  %d\n", delta);
			Serial.printf("  cursor = %d,%d\n", _cursorX, _cursorY);
			Serial.printf("  origin = %d,%d\n", origin_x, origin_y);

			Serial.printf("  Bounding: (%d, %d)-(%d, %d)\n", start_x, start_y, end_x, end_y);
			Serial.printf("  mins (%d %d),\n", start_x_min, start_y_min);
*/
			//}
			// Anti-aliased font

			//Serial.printf("SetAddr %d %d %d %d\n", start_x_min, start_y_min, end_x, end_y);
			// output rectangle we are updating... We have already clipped end_x/y, but not yet start_x/y
			setActiveWindow( start_x, start_y_min, end_x, end_y);
			int screen_y = start_y_min;
			int screen_x;

			// Clear above character
			while (screen_y < origin_y) {
				for (screen_x = start_x_min; screen_x <= end_x; screen_x++) {
					drawPixel(screen_x, screen_y, _TXTBackColor);
				}
				screen_y++;
			}
				
			screen_y = origin_y;
			bitoffset = ((bitoffset + 7) & (-8)); // byte-boundary
			int glyphend_x = origin_x + width;
			uint32_t xp = 0;
			while (linecount) {
				screen_x = start_x;
				while(screen_x<=end_x) {
					// XXX: I'm sure clipping could be done way more efficiently than just chekcing every single pixel, but let's just get this going
					if ((screen_x >= _displayclipx1) && (screen_x < _displayclipx2) && (screen_y >= _displayclipy1) && (screen_y < _displayclipy2)) {
						// Clear before or after pixel
						if ((screen_x<origin_x) || (screen_x>=glyphend_x)){
							drawPixel(screen_x, screen_y, _TXTBackColor);
						}
						// Draw alpha-blended character
						else{
							uint8_t alpha = fetchpixel(data, bitoffset, xp);
							drawPixel(screen_x, screen_y, alphaBlendRGB565Premultiplied( textcolorPrexpanded, textbgcolorPrexpanded, (uint8_t)(alpha * fontalphamx) ) );
							bitoffset += fontbpp;
							xp++;
						}
					} // clip
					screen_x++;
				}
				screen_y++;
				linecount--;
			}
			
			// clear below character
			setActiveWindow();  //have to do this otherwise it gets clipped
			fillRect(_cursorX, screen_y, delta, _cursorY + font->line_space - screen_y, _TXTBackColor);

		} // anti-aliased

		// 1bpp
		else {
			// Now opaque mode... 
			// Now write out background color for the number of rows above the above the character
			// figure out bounding rectangle... 
			// In this mode we need to update to use the offset and bounding rectangles as we are doing it it direct.
			// also update the Origin 
			fillRect(_cursorX, _cursorY, delta, y - _cursorY, _TXTBackColor);
			while (linecount > 0) {
				//Serial.printf("    linecount = %d\n", linecount);
				uint32_t n = 1;
				if (fetchbit(data, bitoffset++) != 0) {
					n = fetchbits_unsigned(data, bitoffset, 3) + 2;
					bitoffset += 3;
				}
				uint32_t x = 0;
				fillRect(_cursorX, y, origin_x - _cursorX, n, _TXTBackColor);
				do {
					int32_t xsize = width - x;
					if (xsize > 32) xsize = 32;
					uint32_t bits = fetchbits_unsigned(data, bitoffset, xsize);
					//Serial.printf("    multi line %d %d %x\n", n, x, bits);
					drawFontBits(opaque, bits, xsize, origin_x + x, y, n);
					bitoffset += xsize;
					x += xsize;
				} while (x < width);

				if ((width+xoffset) < delta) {
					fillRect(origin_x + x, y, delta - (width+xoffset), n, _TXTBackColor);
				}
				y += n;
				linecount -= n;
				//if (++loopcount > 100) {
					//Serial.println("     abort draw loop");
					//break;
				//}
			}
			fillRect(_cursorX, y, delta, _cursorY + font->line_space - y, _TXTBackColor);
		}
	}
	// Increment to setup for the next character.
	_cursorX += delta;
}

//strPixelLen			- gets pixel length of given ASCII string
int16_t RA8876_t3::strPixelLen(const char * str)
{
//	Serial.printf("strPixelLen %s\n", str);
	if (!str) return(0);
	if (gfxFont) 
	{
		// BUGBUG:: just use the other function for now... May do this for all of them...
	  int16_t x, y;
	  uint16_t w, h;
	  getTextBounds(str, _cursorX, _cursorY, &x, &y, &w, &h);
	  return w;
	}

	uint16_t len=0, maxlen=0;
	while (*str)
	{
		if (*str=='\n')
		{
			if ( len > maxlen )
			{
				maxlen=len;
				len=0;
			}
		}
		else
		{
			if (!font)
			{
				len+=textsize_x*6;
			}
			else
			{

				uint32_t bitoffset;
				const uint8_t *data;
				uint16_t c = *str;

//				Serial.printf("char %c(%d)\n", c,c);

				if (c >= font->index1_first && c <= font->index1_last) {
					bitoffset = c - font->index1_first;
					bitoffset *= font->bits_index;
				} else if (c >= font->index2_first && c <= font->index2_last) {
					bitoffset = c - font->index2_first + font->index1_last - font->index1_first + 1;
					bitoffset *= font->bits_index;
				} else if (font->unicode) {
					continue;
				} else {
					continue;
				}
				//Serial.printf("  index =  %d\n", fetchbits_unsigned(font->index, bitoffset, font->bits_index));
				data = font->data + fetchbits_unsigned(font->index, bitoffset, font->bits_index);

				uint32_t encoding = fetchbits_unsigned(data, 0, 3);
				if (encoding != 0) continue;
//				uint32_t width = fetchbits_unsigned(data, 3, font->bits_width);
//				Serial.printf("  width =  %d\n", width);
				bitoffset = font->bits_width + 3;
				bitoffset += font->bits_height;

//				int32_t xoffset = fetchbits_signed(data, bitoffset, font->bits_xoffset);
//				Serial.printf("  xoffset =  %d\n", xoffset);
				bitoffset += font->bits_xoffset;
				bitoffset += font->bits_yoffset;

				uint32_t delta = fetchbits_unsigned(data, bitoffset, font->bits_delta);
				bitoffset += font->bits_delta;
//				Serial.printf("  delta =  %d\n", delta);

				len += delta;//+width-xoffset;
//				Serial.printf("  len =  %d\n", len);
				if ( len > maxlen )
				{
					maxlen=len;
//					Serial.printf("  maxlen =  %d\n", maxlen);
				}
			
			}
		}
		str++;
	}
//	Serial.printf("Return  maxlen =  %d\n", maxlen);
	return( maxlen );
}

void RA8876_t3::charBounds(char c, int16_t *x, int16_t *y,
  int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy) {

  	uint8_t offset, _x, _y;
	int charW = 0;

	// BUGBUG:: Not handling offset/clip
    if (font) {
        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += font->line_space;
        } else if(c != '\r') { // Not a carriage return; is normal char
			uint32_t bitoffset;
			const uint8_t *data;
			if (c >= font->index1_first && c <= font->index1_last) {
				bitoffset = c - font->index1_first;
				bitoffset *= font->bits_index;
			} else if (c >= font->index2_first && c <= font->index2_last) {
				bitoffset = c - font->index2_first + font->index1_last - font->index1_first + 1;
				bitoffset *= font->bits_index;
			} else if (font->unicode) {
				return; // TODO: implement sparse unicode
			} else {
				return;
			}
			//Serial.printf("  index =  %d\n", fetchbits_unsigned(font->index, bitoffset, font->bits_index));
			data = font->data + fetchbits_unsigned(font->index, bitoffset, font->bits_index);

			uint32_t encoding = fetchbits_unsigned(data, 0, 3);
			if (encoding != 0) return;
			uint32_t width = fetchbits_unsigned(data, 3, font->bits_width);
			bitoffset = font->bits_width + 3;
			uint32_t height = fetchbits_unsigned(data, bitoffset, font->bits_height);
			bitoffset += font->bits_height;
			//Serial.printf("  size =   %d,%d\n", width, height);
			//Serial.printf("  line space = %d\n", font->line_space);

			int32_t xoffset = fetchbits_signed(data, bitoffset, font->bits_xoffset);
			bitoffset += font->bits_xoffset;
			int32_t yoffset = fetchbits_signed(data, bitoffset, font->bits_yoffset);
			bitoffset += font->bits_yoffset;

			uint32_t delta = fetchbits_unsigned(data, bitoffset, font->bits_delta);
			bitoffset += font->bits_delta;

            int16_t
                    x1 = *x + xoffset,
                    y1 = *y + yoffset,
                    x2 = x1 + width,
                    y2 = y1 + height;

		    if(wrap && (x2 > _width)) {
	            *x  = 0; // Reset x to zero, advance y by one line
	            *y += font->line_space;
	            x1 = *x + xoffset,
	            y1 = *y + yoffset,
	            x2 = x1 + width,
	            y2 = y1 + height;
        	}
            if(x1 < *minx) *minx = x1;
            if(y1 < *miny) *miny = y1;
            if(x2 > *maxx) *maxx = x2;
            if(y2 > *maxy) *maxy = y2;
            *x += delta;	// ? guessing here...
        }
    } 

    else if(gfxFont) {

        if(c == '\n') { // Newline?
            *x  = 0;    // Reset x to zero, advance y by one line
            *y += textsize_y * gfxFont->yAdvance;
        } else if(c != '\r') { // Not a carriage return; is normal char
            uint8_t first = gfxFont->first,
                    last  = gfxFont->last;
            if((c >= first) && (c <= last)) { // Char present in this font?
    			GFXglyph *glyph  = gfxFont->glyph + (c - first);
                uint8_t gw = glyph->width,
                        gh = glyph->height,
                        xa = glyph->xAdvance;
                int8_t  xo = glyph->xOffset,
                        yo = glyph->yOffset + gfxFont->yAdvance/2;
                if(wrap && ((*x+(((int16_t)xo+gw)*textsize_x)) > _width)) {
                    *x  = 0; // Reset x to zero, advance y by one line
                    *y += textsize_y * gfxFont->yAdvance;
                }
                int16_t tsx = (int16_t)textsize_x,
                        tsy = (int16_t)textsize_y,
                        x1 = *x + xo * tsx,
                        y1 = *y + yo * tsy,
                        x2 = x1 + gw * tsx - 1,
                        y2 = y1 + gh * tsy - 1;
                if(x1 < *minx) *minx = x1;
                if(y1 < *miny) *miny = y1;
                if(x2 > *maxx) *maxx = x2;
                if(y2 > *maxy) *maxy = y2;
                *x += xa * tsx;
            }
        }
    }
}

// Add in Adafruit versions of text bounds calculations. 
void RA8876_t3::getTextBounds(const uint8_t *buffer, uint16_t len, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while(len--)
        charBounds(*buffer++, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }

}

void RA8876_t3::getTextBounds(const char *str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    uint8_t c; // Current character

    *x1 = x;
    *y1 = y;
    *w  = *h = 0;

    int16_t minx = _width, miny = _height, maxx = -1, maxy = -1;

    while((c = *str++))
        charBounds(c, &x, &y, &minx, &miny, &maxx, &maxy);

    if(maxx >= minx) {
        *x1 = minx;
        *w  = maxx - minx + 1;
    }
    if(maxy >= miny) {
        *y1 = miny;
        *h  = maxy - miny + 1;
    }
}

void RA8876_t3::getTextBounds(const String &str, int16_t x, int16_t y,
        int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h) {
    if (str.length() != 0) {
        getTextBounds(const_cast<char*>(str.c_str()), x, y, x1, y1, w, h);
    }
}


void RA8876_t3::drawFontBits(bool opaque, uint32_t bits, uint32_t numbits, int32_t x, int32_t y, uint32_t repeat)
{
	//Serial.printf("    drawFontBits: %d %x %x (%d %d) %u\n", opaque, bits, numbits, x, y, repeat);
	if (bits == 0) {
		if (opaque) {
			fillRect(x, y, numbits, repeat, _TXTBackColor);
		}
	} else {
		int32_t x1 = x;
		uint32_t n = numbits;
		int w;
		int bgw;

		w = 0;
		bgw = 0;

		do {
			n--;
			if (bits & (1 << n)) {
				if (bgw>0) {
					if (opaque) {
						fillRect(x1 - bgw, y, bgw, repeat, _TXTBackColor);
						//Serial.printf("        BG fillrect: %d %d %d %d %x\n", x1 - bgw, y, bgw, repeat, _TXTBackColor);
					}
					bgw=0;
				}
				w++;
			} else {
				if (w>0) {
					fillRect(x1 - w, y, w, repeat, _TXTForeColor);
					//Serial.printf("        FG fillrect: %d %d %d %d %x\n", x1 - w, y, w, repeat, _TXTForeColor);
					w = 0;
				}
				bgw++;
			}
			x1++;
		} while (n > 0);

		if (w > 0) {
			fillRect(x1 - w, y, w, repeat, _TXTForeColor);
			//Serial.printf("        FG fillrect: %d %d %d %d %x\n", x1 - w, y, w, repeat, _TXTForeColor);
		}

		if (bgw > 0) {
			if (opaque) {
				fillRect(x1 - bgw, y, bgw, repeat, _TXTBackColor);
				//Serial.printf("        BG fillrect: %d %d %d %d %x\n", x1 - bgw, y, bgw, repeat, _TXTBackColor);
			}
		}
	}
}

void RA8876_t3::drawGFXFontChar(unsigned int c) {
	// Lets do Adafruit GFX character output here as well
    if(c == '\r') 	 return;
    // Some quick and dirty tests to see if we can
    uint8_t first = gfxFont->first;
    if((c < first) || (c > gfxFont->last)) return; 

    GFXglyph *glyph  = gfxFont->glyph + (c - first);
    uint8_t   w     = glyph->width,
              h     = glyph->height;
	//Serial.printf("w = %d, h = %d\n", w, h);
    //if((w == 0) || (h == 0))  return;  // Is there an associated bitmap?

    int16_t xo = glyph->xOffset; // sic
    int16_t yo = glyph->yOffset + gfxFont->yAdvance/2;

    if(wrap && ((_cursorX + textsize_x * (xo + w)) > _width)) {
        _cursorX  = 0;
        _cursorY += (int16_t)textsize_y * gfxFont->yAdvance;
    }

    // Lets do the work to output the font character
    uint8_t  *bitmap = gfxFont->bitmap;

    uint16_t bo = glyph->bitmapOffset;
    uint8_t  xx, yy, bits = 0, bit = 0;
    //Serial.printf("DGFX_char: %c (%d,%d) : %u %u %u %u %d %d %x %x %d\n", c, _cursorX, _cursorY, w, h,  
    //			glyph->xAdvance, gfxFont->yAdvance, xo, yo, _TXTForeColor, _TXTBackColor, 0);  Serial.flush();

    if (_backTransparent) {

	     //Serial.printf("DGFXChar: %c %u, %u, wh:%d %d o:%d %d\n", c, _cursorX, _cursorY, w, h, xo, yo);
	    // Todo: Add character clipping here

    	// NOTE: Adafruit GFX does not support Opaque font output as there
    	// are issues with proportionally spaced characters that may overlap
    	// So the below is not perfect as we may overwrite a small portion 
    	// of a letter with the next one, when we blank out... 
    	// But: I prefer to let each of us decide if the limitations are
    	// worth it or not.  If Not you still have the option to not
    	// Do transparent mode and instead blank out and blink...

	    for(yy=0; yy<h; yy++) {
			uint8_t w_left = w;
	    	xx = 0;
	        while (w_left) {
	            if(!(bit & 7)) {
	                bits = bitmap[bo++];
	            }
				// Could try up to 8 bits at time, but start off trying up to 4
	            uint8_t xCount;
	            if ((w_left >= 8) && ((bits & 0xff) == 0xff)) {
	            	xCount = 8;
	            	//Serial.print("8");
	                fillRect(_cursorX+(xo+xx)*textsize_x, _cursorY+(yo+yy)*textsize_y,
	                      xCount * textsize_x, textsize_y, _TXTForeColor);
	            } else if ((w_left >= 4) && ((bits & 0xf0) == 0xf0)) {
	            	xCount = 4;
	            	//Serial.print("4");
	                fillRect(_cursorX+(xo+xx)*textsize_x, _cursorY+(yo+yy)*textsize_y,
	                      xCount * textsize_x, textsize_y, _TXTForeColor);
	            } else if ((w_left >= 3) && ((bits & 0xe0) == 0xe0)) {
	            	//Serial.print("3");
	            	xCount = 3;
	                fillRect(_cursorX+(xo+xx)*textsize_x, _cursorY+(yo+yy)*textsize_y,
                      xCount * textsize_x, textsize_y, _TXTForeColor);
	            } else if ((w_left >= 2) && ((bits & 0xc0) == 0xc0)) {
	            	//Serial.print("2");
	            	xCount = 2;
	                fillRect(_cursorX+(xo+xx)*textsize_x, _cursorY+(yo+yy)*textsize_y,
	                      xCount * textsize_x, textsize_y, _TXTForeColor);
	            } else {
	            	xCount = 1;
	            	if(bits & 0x80) {
		                if((textsize_x == 1) && (textsize_y == 1)){
		                    drawPixel(_cursorX+xo+xx, _cursorY+yo+yy, _TXTForeColor);
		                } else {
							fillRect(_cursorX+(xo+xx)*textsize_x, _cursorY+(yo+yy)*textsize_y,textsize_x, textsize_y, _TXTForeColor);
		                }
		            }
	            }
	            xx += xCount;
	            w_left -= xCount;
	            bit += xCount;
	            bits <<= xCount;
			}
	    }
    	_gfx_last_char_x_write = 0;
	} else {
		// To Do, properly clipping and offsetting...
		// This solid background approach is about 5 time faster
		// Lets calculate bounding rectangle that we will update
		// We need to offset by the origin.

		// We are going direct so do some offsets and clipping
		int16_t x_offset_cursor = _cursorX + _originx;	// This is where the offseted cursor is.
		int16_t x_start = x_offset_cursor;  // I am assuming no negative x offsets.
		int16_t x_end = x_offset_cursor + (glyph->xAdvance * textsize_x);
		if (glyph->xAdvance < (xo + w)) x_end = x_offset_cursor + ((xo + w)* textsize_x);  // BUGBUG Overlflows into next char position.
		int16_t x_left_fill = x_offset_cursor + xo * textsize_x;
		int16_t x;
		if (xo < 0) { 
			// Unusual character that goes back into previous character
			//Serial.printf("GFX Font char XO < 0: %c %d %d %u %u %u\n", c, xo, yo, w, h, glyph->xAdvance );
			x_start += xo * textsize_x;
			x_left_fill = 0;	// Don't need to fill anything here... 
		}

		int16_t y_start = _cursorY + _originy + (_gfxFont_min_yOffset * textsize_y)+ gfxFont->yAdvance*textsize_y/2;  // UP to most negative value.
		int16_t y_end = y_start +  gfxFont->yAdvance * textsize_y;  // how far we will update
		int16_t y = y_start;
		//int8_t y_top_fill = (yo - _gfxFont_min_yOffset) * textsize_y;	 // both negative like -10 - -16 = 6...
		int8_t y_top_fill = (yo - gfxFont->yAdvance/2 - _gfxFont_min_yOffset) * textsize_y;


		// See if anything is within clip rectangle, if not bail
		if((x_start >= _displayclipx2)   || // Clip right
			 (y_start >= _displayclipy2) || // Clip bottom
			 (x_end < _displayclipx1)    || // Clip left
			 (y_end < _displayclipy1))  	// Clip top 
		{
			// But remember to first update the cursor position
			_cursorX += glyph->xAdvance * (int16_t)textsize_x;
			return;
		}

		// If our y_end > _displayclipy2 set it to _displayclipy2 as to not have to test both  Likewise X
		if (y_end > _displayclipy2) y_end = _displayclipy2;
		if (x_end > _displayclipx2) x_end = _displayclipx2;
		// If we get here and 
		if (_gfx_last__cursorY != (_cursorY + _originy))  _gfx_last_char_x_write = 0;

		// lets try to output text in one output rectangle
		//Serial.printf("    SPI (%d %d) (%d %d)\n", x_start, y_start, x_end, y_end);Serial.flush();
		// compute the actual region we will output given 
		//_startSend();
	
		//setActiveWindow((x_start >= _displayclipx1) ? x_start : _displayclipx1, 
		//		(y_start >= _displayclipy1) ? y_start : _displayclipy1, 
		//		x_end  - 1,  y_end - 1); 
		//writeCommand(RA8875_MRWC);
		//Serial.printf("SetAddr: %u %u %u %u\n", (x_start >= _displayclipx1) ? x_start : _displayclipx1, 
		//		(y_start >= _displayclipy1) ? y_start : _displayclipy1, 
		//		x_end  - 1,  y_end - 1); 
		// First lets fill in the top parts above the actual rectangle...
		//Serial.printf("    y_top_fill %d x_left_fill %d\n", y_top_fill, x_left_fill);
		while (y_top_fill--) {
			if ( (y >= _displayclipy1) && (y < _displayclipy2)) {
				for (int16_t xx = x_start; xx < x_end; xx++) {
					if (xx >= _displayclipx1) {
						combineAndDrawPixel(xx, y, gfxFontLastCharPosFG(xx,y)? _gfx_last_char_textcolor : (xx < x_offset_cursor)? _gfx_last_char_textbgcolor : _TXTBackColor);
					}
				}					
			}
			forceCombinedPixelsOut();
			y++;
		}
		//Serial.println("    After top fill"); Serial.flush();
		// Now lets output all of the pixels for each of the rows.. 
		for(yy=0; (yy<h) && (y < _displayclipy2); yy++) {
			uint16_t bo_save = bo;
			uint8_t bit_save = bit;
			uint8_t bits_save = bits;
			for (uint8_t yts = 0; (yts < textsize_y) && (y < _displayclipy2); yts++) {
				// need to repeat the stuff for each row...
				bo = bo_save;
				bit = bit_save;
				bits = bits_save;
				x = x_start;
				if (y >= _displayclipy1) {
					while (x < x_left_fill) {
						if ( (x >= _displayclipx1) && (x < _displayclipx2)) {
							// Don't need to check if we are in previous char as in this case x_left_fill is set to 0...
							combineAndDrawPixel(x, y, gfxFontLastCharPosFG(x,y)? _gfx_last_char_textcolor :  _TXTBackColor);
						}
						x++;
					}
			        for(xx=0; xx<w; xx++) {
			            if(!(bit++ & 7)) {
			                bits = bitmap[bo++];
			            }
			            for (uint8_t xts = 0; xts < textsize_x; xts++) {
							if ( (x >= _displayclipx1) && (x < _displayclipx2)) {
								if (bits & 0x80) 
									combineAndDrawPixel(x, y, _TXTForeColor); 
								else  
									combineAndDrawPixel(x, y,gfxFontLastCharPosFG(x,y)? _gfx_last_char_textcolor : (x < x_offset_cursor)? _gfx_last_char_textbgcolor : _TXTBackColor);		
							}
			            	x++;	// remember our logical position...
			            }
			            bits <<= 1;
			        }
			        // Fill in any additional bg colors to right of our output
			        while (x < x_end) {
						if (x >= _displayclipx1) {
			        		combineAndDrawPixel(x, y, gfxFontLastCharPosFG(x,y)? _gfx_last_char_textcolor : (x < x_offset_cursor)? _gfx_last_char_textbgcolor : _TXTBackColor);
			        	}
			        	x++;
			        }
		    	}
				forceCombinedPixelsOut();
	        	y++;	// remember which row we just output
		    }
	    }
	    // And output any more rows below us...
	  	//Serial.println("    Bottom fill"); Serial.flush();
		while (y < y_end) {
			if (y >= _displayclipy1) {
				for (int16_t xx = x_start; xx < x_end; xx++) {
					if (xx >= _displayclipx1)  {
						combineAndDrawPixel(xx ,y, gfxFontLastCharPosFG(xx,y)? _gfx_last_char_textcolor : (xx < x_offset_cursor)? _gfx_last_char_textbgcolor : _TXTBackColor);
					}
				}
			}
			forceCombinedPixelsOut();
			y++;
		}
		//writecommand_last(ILI9488_NOP);
		//_endSend();
		//setActiveWindow();

		_gfx_c_last = c; 
		_gfx_last__cursorX = _cursorX + _originx;  
		_gfx_last__cursorY = _cursorY + _originy; 
		_gfx_last_char_x_write = x_end; 
		_gfx_last_char_textcolor = _TXTForeColor; 
		_gfx_last_char_textbgcolor = _TXTBackColor;
		
	}

    _cursorX += glyph->xAdvance * (int16_t)textsize_x;
}


// Some fonts overlap characters if we detect that the previous 
// character wrote out more width than they advanced in X direction
// we may want to know if the last character output a FG or BG at a position. 
	// Opaque font chracter overlap?
//	unsigned int _gfx_c_last;
//	int16_t   _gfx_last__cursorX, _gfx_last__cursorY;
//	int16_t	 _gfx_last_x_overlap = 0;
	
bool RA8876_t3::gfxFontLastCharPosFG(int16_t x, int16_t y) {
    GFXglyph *glyph  = gfxFont->glyph + (_gfx_c_last -  gfxFont->first);

    uint8_t   w     = glyph->width,
              h     = glyph->height;


    int16_t xo = glyph->xOffset; // sic
    int16_t yo = glyph->yOffset + gfxFont->yAdvance/2;
    if (x >= _gfx_last_char_x_write) return false; 	// we did not update here...
    if (y < (_gfx_last__cursorY + (yo*textsize_y)))  return false;  // above
    if (y >= (_gfx_last__cursorY + (yo+h)*textsize_y)) return false; // below


    // Lets compute which Row this y is in the bitmap
    int16_t y_bitmap = (y - ((_gfx_last__cursorY + (yo*textsize_y))) + textsize_y - 1) / textsize_y;
    int16_t x_bitmap = (x - ((_gfx_last__cursorX + (xo*textsize_x))) + textsize_x - 1) / textsize_x;
    uint16_t  pixel_bit_offset = y_bitmap * w + x_bitmap;

    return ((gfxFont->bitmap[glyph->bitmapOffset + (pixel_bit_offset >> 3)]) & (0x80 >> (pixel_bit_offset & 0x7)));
}


void RA8876_t3::setTextSize(uint8_t s_x, uint8_t s_y) {
    textsize_x = (s_x > 0) ? s_x : 1;
    textsize_y = (s_y > 0) ? s_y : 1;
	_gfx_last_char_x_write = 0;	// Don't use cached data here
}

void RA8876_t3::drawFontPixel( uint8_t alpha, uint32_t x, uint32_t y ){
	// Adjust alpha based on the number of alpha levels supported by the font (based on bpp)
	// Note: Implemented look-up table for alpha, but made absolutely no difference in speed (T3.6)
	alpha = (uint8_t)(alpha * fontalphamx);
	uint32_t result = ((((textcolorPrexpanded - textbgcolorPrexpanded) * alpha) >> 5) + textbgcolorPrexpanded) & 0b00000111111000001111100000011111;
	Pixel(x,y,(uint16_t)((result >> 16) | result));
}


void RA8876_t3::Pixel(int16_t x, int16_t y, uint16_t color)
 {
	x+=_originx;
	y+=_originy;

	if((x < _displayclipx1) ||(x >= _displayclipx2) || (y < _displayclipy1) || (y >= _displayclipy2)) return;

	setActiveWindow(x, y, x, y);
	//writeCommand(RA8875_MRWC);
	drawPixel(x, y, color);
}


uint32_t RA8876_t3::fetchbit(const uint8_t *p, uint32_t index)
{
	if (p[index >> 3] & (1 << (7 - (index & 7)))) return 1;
	return 0;
}

uint32_t RA8876_t3::fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = 0;
	do {
		uint8_t b = p[index >> 3];
		uint32_t avail = 8 - (index & 7);
		if (avail <= required) {
			val <<= avail;
			val |= b & ((1 << avail) - 1);
			index += avail;
			required -= avail;
		} else {
			b >>= avail - required;
			val <<= required;
			val |= b & ((1 << required) - 1);
			break;
		}
	} while (required);
	return val;
}

uint32_t RA8876_t3::fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required)
{
	uint32_t val = fetchbits_unsigned(p, index, required);
	if (val & (1 << (required - 1))) {
		return (int32_t)val - (1 << required);
	}
	return (int32_t)val;
}

uint32_t RA8876_t3::fetchpixel(const uint8_t *p, uint32_t index, uint32_t x)
{
	// The byte
	uint8_t b = p[index >> 3];
	// Shift to LSB position and mask to get value
	uint8_t s = ((fontppb-(x % fontppb)-1)*fontbpp);
	// Mask and return
	return (b >> s) & fontbppmask;
}


/**************************************************************************/
/*!
      for compatibility with popular Adafruit_GFX
	  draws a single vertical line
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  h: height
	  color: RGB565 color
*/
/**************************************************************************/
void RA8876_t3::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	if (h < 1) h = 1;
	h < 2 ? drawPixel(x,y,color) : drawLine(x, y, x, (y+h)-1, color);
}

/**************************************************************************/
/*!
      for compatibility with popular Adafruit_GFX
	  draws a single orizontal line
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  w: width
	  color: RGB565 color
*/
/**************************************************************************/
void RA8876_t3::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	if (w < 1) w = 1;
	w < 2 ? drawPixel(x,y,color) : drawLine(x, y, (w+x)-1, y, color);
}

/**************************************************************************/
/*!		
		Set the Active Window
	    Parameters:
		XL: Horizontal Left
		XR: Horizontal Right
		YT: Vertical TOP
		YB: Vertical Bottom
*/
/**************************************************************************/
void RA8876_t3::setActiveWindow(int16_t XL,int16_t XR ,int16_t YT ,int16_t YB)
{
	if (_portrait){ swapvals(XL,YT); swapvals(XR,YB);}
	
	if (XR >= SCREEN_WIDTH) XR = SCREEN_WIDTH;
	if (YB >= SCREEN_HEIGHT) YB = SCREEN_HEIGHT;
	
	_activeWindowXL = XL; _activeWindowXR = XR;
	_activeWindowYT = YT; _activeWindowYB = YB;
	_updateActiveWindow(false);
}

/**************************************************************************/
/*!		
		Set the Active Window as FULL SCREEN
*/
/**************************************************************************/
void RA8876_t3::setActiveWindow(void)
{
	_activeWindowXL = 0; _activeWindowXR = SCREEN_WIDTH;
	_activeWindowYT = 0; _activeWindowYB = SCREEN_HEIGHT;
	//if (_portrait){swapvals(_activeWindowXL,_activeWindowYT); swapvals(_activeWindowXR,_activeWindowYB);}
	_updateActiveWindow(true);
}

/**************************************************************************/
/*!
		this update the RA8875 Active Window registers
		[private]
*/
/**************************************************************************/
void RA8876_t3::_updateActiveWindow(bool full)
{
	if (full){
		// X
		activeWindowWH(_width, _height);;
	} else {
		activeWindowXY(_activeWindowXL, _activeWindowYT);
		activeWindowWH(_activeWindowXR-_activeWindowXL, _activeWindowYB-_activeWindowYT);		
	}
}

/**************************************************************************/
/*!   
		Set the x,y position for text only
		Parameters:
		x: horizontal pos in pixels
		y: vertical pos in pixels
		update: true track the actual text position internally
		note: not active with rendered fonts, just set x,y internal tracked param
		[private]
*/
/**************************************************************************/
void RA8876_t3::_textPosition(int16_t x, int16_t y, bool update)
{
	lcdRegDataWrite(RA8876_F_CURX0,x, false); //63h
	lcdRegDataWrite(RA8876_F_CURX1,x>>8, false);//64h
	lcdRegDataWrite(RA8876_F_CURY0,y, false);//65h
	lcdRegDataWrite(RA8876_F_CURY1,y>>8, true);//66h
	if (update){ _cursorX = x; _cursorY = y;}
}

void RA8876_t3::_setFNTdimensions(uint8_t index) 
{
	_FNTwidth 		= 	fontDimPar[index][0];
	_FNTheight 		= 	fontDimPar[index][1];
	_FNTbaselineLow  = 	fontDimPar[index][2];
	_FNTbaselineTop  = 	fontDimPar[index][3];
}


/**************************************************************************/
/*!  
		choose from internal/external (if exist) Font Rom
		Parameters:
		s: Font source (INTFONT,EXTFONT)
*/
/**************************************************************************/
void RA8876_t3::setFontDef() 
{
	_use_int_font = 1;
	_use_tfont = 0;
	_use_ili_font = 0;
	_use_gfx_font = 0;
	
	// Select internal fonts or user defined fonts (0 for internal or 1 for user defined)
	//setFontSource(0);

}


//**************************************************************//
/* Print a character to current active screen                   */
/* This function processes most ASCII control codes and will    */
/* scroll the screen up when text position is past bottom line  */
//**************************************************************//
// overwrite functions from class Print:
size_t RA8876_t3::write(uint8_t c) {
	return write(&c, 1);
}

size_t RA8876_t3::write(const uint8_t *buffer, size_t size) {
  if(_use_default){
	size_t cb = size;
	while (cb) {
		uint8_t text = *buffer++;
		cb--;
		vdata = text;
		if (text == 13){//'\r'
			//Ignore carriage-return
		} else if(text == '\n') {
			_cursorY += (_FNTheight * _scaleY);
			prompt_line = _cursorY + _scrollYT;
			_cursorX = _scrollXL;
			update_xy();
		} else if(text == 127) { // Destructive Backspace
				if((_cursorX > (prompt_size +_scrollXL)) || (_cursorY > (prompt_line + _scrollYT)))
				{
					_cursorX-=(_FNTwidth * _scaleX);
					update_xy();
					vdata = 0x20;
					update_tft(vdata);
					update_xy();
				}
				if((_cursorY > (prompt_line + _scrollYT)) && ((_cursorX + _scrollXL) < 0)) {
					_cursorY -= (_FNTheight * _scaleY);
					_cursorX = _width-(_FNTwidth * _scaleX);
					if(_FNTwidth == 12)
						_cursorX -= 4; // 1024 / 12 = 85.3 so adjust
					update_xy();

				}
		} else if(text == 0x09) { // TAB Character
				_cursorX += (tab_size*(_FNTwidth * _scaleX));
				update_xy();
		} else if(text == 0x07) { // BELL Character
				tone(35,1000,500); // Need Pin variable for tone output (now pin #35)
		} else if(text == 0x0c) { // form feed
			drawSquareFill(_scrollXL, _scrollYT, _scrollXR, _scrollYB, _TXTBackColor);
			textColor(_TXTForeColor,_TXTBackColor);
			setTextCursor(_scrollXL,_scrollYT);
		} else {
			update_tft(vdata);
			_cursorX += (_FNTwidth * _scaleX);
			switch(_FNTwidth) {
				case 12: // Font width is 12, 1024 / 12 =  85.3, have to
						 // subtract 12 to keep within screen width.
					if(_cursorX >= _scrollXR-(_FNTwidth * _scaleX)) {
					_cursorY += (_FNTheight * _scaleY);
					_cursorX = _scrollXL;
				}
				break;
				default:
					if(_cursorX >= _scrollXR) {
					_cursorY += (_FNTheight * _scaleY);
					_cursorX = _scrollXL;
				}
			}
			update_xy();
		}
	}
	return 1;
  } else {
	  _fontWrite(buffer, size);
	  return 1;
  }
}

//************************************************//
/* Print a character to current active screen.    */
/* This function does not process ASCII control   */
/* codes and will scroll the screen up when text  */
/* position is past bottom line.                  */
//************************************************//
size_t RA8876_t3::rawPrint(uint8_t text) {
		update_tft(text);
		_cursorX += (_FNTwidth * _scaleX);
		switch(_FNTwidth) {
			case 12: // Font width is 12, 1024 / 12 =  85.3, have to
					 // subtract 12 to keep within screen width.
				if(_cursorX >= _scrollXR-(_FNTwidth * _scaleX)) {
				_cursorY += (_FNTheight * _scaleY);
				_cursorX = _scrollXL;
			}
			break;
			default:
				if(_cursorX >= _scrollXR) {
				_cursorY += (_FNTheight * _scaleY);
				_cursorX = _scrollXL;
			}
		}
		update_xy();
	return 1;
}

