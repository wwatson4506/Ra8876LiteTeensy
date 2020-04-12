//**************************************************************//
//**************************************************************//
/*
 * Ra8876LiteTeensy.cpp
 * Modified Version of: File Name : Ra8876_Lite.cpp                                   
 *			Author    : RAiO Application Team                             
 *			Edit Date : 09/13/2017
 *			Version   : v2.0  1.modify bte_DestinationMemoryStartAddr bug 
 *                 			  2.modify ra8876SdramInitial Auto_Refresh
 *                 			  3.modify ra8876PllInitial 
 * 	  	      : For Teensy 3.x and T4
 *                    : By Warren Watson
 *                    : 06/07/2018 - 11/31/2019
 *                    : Copyright (c) 2017-2019 Warren Watson.
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
//**************************************************************//

#include "Ra8876_Lite.h"

//**************************************************************//
// Graphic Cursor Image (Busy)
//**************************************************************//
PROGMEM unsigned char gImage_busy_im[256] = { /* 0X00,0X02,0X20,0X00,0X20,0X00, */
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X55,0X55,0X55,0X55,0X6A,0XAA,
0XAA,0XAA,0X54,0X00,0X00,0X05,0X6A,0XAA,0XAA,0XAA,0X55,0X55,0X55,0X55,0X6A,0XAA,
0XAA,0XAA,0X94,0X00,0X00,0X05,0XAA,0XAA,0XAA,0XAA,0X94,0X00,0X00,0X05,0XAA,0XAA,
0XAA,0XAA,0X94,0X44,0X44,0X45,0XAA,0XAA,0XAA,0XAA,0X94,0X11,0X11,0X05,0XAA,0XAA,
0XAA,0XAA,0X95,0X04,0X44,0X15,0XAA,0XAA,0XAA,0XAA,0XA5,0X41,0X10,0X56,0XAA,0XAA,
0XAA,0XAA,0XA9,0X50,0X41,0X5A,0XAA,0XAA,0XAA,0XAA,0XAA,0X54,0X05,0X6A,0XAA,0XAA,
0XAA,0XAA,0XAA,0X94,0X05,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X94,0X05,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0X94,0X45,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X94,0X05,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0X50,0X01,0X6A,0XAA,0XAA,0XAA,0XAA,0XA9,0X40,0X40,0X5A,0XAA,0XAA,
0XAA,0XAA,0XA5,0X00,0X10,0X16,0XAA,0XAA,0XAA,0XAA,0X94,0X00,0X00,0X05,0XAA,0XAA,
0XAA,0XAA,0X94,0X04,0X44,0X05,0XAA,0XAA,0XAA,0XAA,0X94,0X11,0X11,0X05,0XAA,0XAA,
0XAA,0XAA,0X94,0X44,0X44,0X45,0XAA,0XAA,0XAA,0XAA,0X95,0X11,0X11,0X15,0XAA,0XAA,
0XAA,0XAA,0X55,0X55,0X55,0X55,0X6A,0XAA,0XAA,0XAA,0X54,0X00,0X00,0X05,0X6A,0XAA,
0XAA,0XAA,0X55,0X55,0X55,0X55,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
};

//**************************************************************//
// Graphic Cursor Image (Not)
//**************************************************************//
PROGMEM unsigned char gImage_no_im[256] = { /* 0X00,0X02,0X20,0X00,0X20,0X00, */
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X95,0X55,0XAA,0XAA,0XAA,
0XAA,0XAA,0XA9,0X40,0X00,0X5A,0XAA,0XAA,0XAA,0XAA,0X94,0X00,0X00,0X05,0XAA,0XAA,
0XAA,0XAA,0X40,0X00,0X00,0X00,0X6A,0XAA,0XAA,0XA9,0X00,0X15,0X55,0X00,0X1A,0XAA,
0XAA,0XA4,0X00,0X6A,0XAA,0X50,0X06,0XAA,0XAA,0XA4,0X00,0X6A,0XAA,0XA4,0X06,0XAA,
0XAA,0X90,0X00,0X1A,0XAA,0XA9,0X01,0XAA,0XAA,0X90,0X10,0X06,0XAA,0XA9,0X01,0XAA,
0XAA,0X40,0X64,0X01,0XAA,0XAA,0X40,0X6A,0XAA,0X40,0X69,0X00,0X6A,0XAA,0X40,0X6A,
0XAA,0X40,0X6A,0X40,0X1A,0XAA,0X40,0X6A,0XAA,0X40,0X6A,0X90,0X06,0XAA,0X40,0X6A,
0XAA,0X40,0X6A,0XA4,0X01,0XAA,0X40,0X6A,0XAA,0X40,0X6A,0XA9,0X00,0X6A,0X40,0X6A,
0XAA,0X40,0X6A,0XAA,0X40,0X1A,0X40,0X6A,0XAA,0X90,0X1A,0XAA,0X90,0X05,0X01,0XAA,
0XAA,0X90,0X1A,0XAA,0XA4,0X00,0X01,0XAA,0XAA,0XA4,0X06,0XAA,0XA9,0X00,0X06,0XAA,
0XAA,0XA4,0X01,0X6A,0XAA,0X40,0X06,0XAA,0XAA,0XA9,0X00,0X15,0X55,0X00,0X1A,0XAA,
0XAA,0XAA,0X40,0X00,0X00,0X00,0X6A,0XAA,0XAA,0XAA,0X94,0X00,0X00,0X05,0XAA,0XAA,
0XAA,0XAA,0XA9,0X40,0X00,0X5A,0XAA,0XAA,0XAA,0XAA,0XAA,0X95,0X55,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
};

//**************************************************************//
// Graphic Cursor Image (Arrow)
//**************************************************************//
PROGMEM unsigned char gImage_arrow_il[256] = { /* 0X00,0X02,0X20,0X00,0X20,0X00, */
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0X5A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X46,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0X41,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0X40,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0X40,0X01,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X00,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,
0X40,0X00,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X00,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,
0X40,0X00,0X01,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X00,0X00,0X6A,0XAA,0XAA,0XAA,0XAA,
0X40,0X00,0X00,0X1A,0XAA,0XAA,0XAA,0XAA,0X40,0X00,0X00,0X06,0XAA,0XAA,0XAA,0XAA,
0X40,0X00,0X00,0X01,0XAA,0XAA,0XAA,0XAA,0X40,0X00,0X00,0X00,0X6A,0XAA,0XAA,0XAA,
0X40,0X00,0X15,0X55,0X5A,0XAA,0XAA,0XAA,0X40,0X10,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,
0X40,0X64,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,0X41,0XA4,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,
0X46,0XA9,0X01,0XAA,0XAA,0XAA,0XAA,0XAA,0X5A,0XA9,0X01,0XAA,0XAA,0XAA,0XAA,0XAA,
0X6A,0XAA,0X40,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X6A,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0X90,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X1A,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XA4,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XA4,0X06,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0XA9,0X5A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
};

//**************************************************************//
// Graphic Cursor Image (Pen)
//**************************************************************//
PROGMEM unsigned char gImage_pen_il[256] = { /* 0X00,0X02,0X20,0X00,0X20,0X00, */
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0X96,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X91,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
0XA4,0X15,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XA4,0X00,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,
0XA9,0X01,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XA9,0X00,0X46,0XAA,0XAA,0XAA,0XAA,0XAA,
0XAA,0X40,0X51,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X14,0X6A,0XAA,0XAA,0XAA,0XAA,
0XAA,0XA4,0X05,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XA9,0X01,0X46,0XAA,0XAA,0XAA,0XAA,
0XAA,0XAA,0X40,0X51,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X14,0X6A,0XAA,0XAA,0XAA,
0XAA,0XAA,0XA4,0X05,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XA9,0X01,0X46,0XAA,0XAA,0XAA,
0XAA,0XAA,0XAA,0X40,0X51,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X14,0X69,0XAA,0XAA,
0XAA,0XAA,0XAA,0XA4,0X01,0X14,0X6A,0XAA,0XAA,0XAA,0XAA,0XA9,0X00,0X44,0X1A,0XAA,
0XAA,0XAA,0XAA,0XAA,0X40,0X11,0X06,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X04,0X41,0XAA,
0XAA,0XAA,0XAA,0XAA,0XA4,0X01,0X10,0X6A,0XAA,0XAA,0XAA,0XAA,0XA9,0X00,0X44,0X1A,
0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X11,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0X90,0X04,0X1A,
0XAA,0XAA,0XAA,0XAA,0XAA,0XA4,0X01,0X1A,0XAA,0XAA,0XAA,0XAA,0XAA,0XA9,0X00,0X1A,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X40,0X6A,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0X95,0XAA,
0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,0XAA,
};

//**************************************************************//
// Ra8876_Lite()
//**************************************************************//
Ra8876_Lite::Ra8876_Lite(int CSp, int RSTp, int mosi_pin, int sclk_pin, int miso_pin)
{
	_mosi = mosi_pin;
	_miso = miso_pin;
	_sclk = sclk_pin;
	_cs = CSp;
	_rst = RSTp;
}

//**************************************************************//
// Ra8876_begin()
//**************************************************************//
boolean Ra8876_Lite::Ra8876_begin(void) 
{ 
  //initialize the bus for Teensy 3.6/4.0
	if ((_mosi == 11 || _mosi == 7) && (_miso == 12 || _miso == 8) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
		if (_mosi != 11) SPI.setMOSI(_mosi);
		if (_miso != 12) SPI.setMISO(_miso);
		if (_sclk != 13) SPI.setSCK(_sclk);
	} else {
		_errorCode |= (1 << 1);//set
		return false;
	}
	if (!SPI.pinIsChipSelect(_cs)) {
		_errorCode |= (1 << 2);//set
		return false;
	}
	pinMode(_cs, OUTPUT);
	SPI.begin();
  
	//ra8876 hardware reset
	pinMode(_rst, OUTPUT); 
	digitalWrite(_rst, HIGH);
	delay(1);
	digitalWrite(_rst, LOW);
	delay(1);
	digitalWrite(_rst, HIGH);
	delay(10);
	if(!checkIcReady())
		return false;
	//read ID code must disable pll, 01h bit7 set 0
	lcdRegDataWrite(0x01,0x08);
	delay(1);
	if ((lcdRegDataRead(0xff) != 0x76)&&(lcdRegDataRead(0xff) != 0x77))
		return false;
	// Initialize RA8876 to default settings
	if(!ra8876Initialize())
		return false;
	// return success
	return true;
}

//**************************************************************//
// Initialize RA8876 to default settings.
// Return true on success.
//**************************************************************//
boolean Ra8876_Lite::ra8876Initialize(void) {
	
	// Init PLL
	if(!ra8876PllInitial())
		return false;
	// Init SDRAM
	if(!ra8876SdramInitial())
		return false;

	lcdRegWrite(RA8876_CCR);//01h
//  lcdDataWrite(RA8876_PLL_ENABLE<<7|RA8876_WAIT_MASK<<6|RA8876_KEY_SCAN_DISABLE<<5|RA8876_TFT_OUTPUT24<<3
	lcdDataWrite(RA8876_PLL_ENABLE<<7|RA8876_WAIT_NO_MASK<<6|RA8876_KEY_SCAN_DISABLE<<5|RA8876_TFT_OUTPUT24<<3
	|RA8876_I2C_MASTER_DISABLE<<2|RA8876_SERIAL_IF_ENABLE<<1|RA8876_HOST_DATA_BUS_SERIAL);

	lcdRegWrite(RA8876_MACR);//02h
	lcdDataWrite(RA8876_DIRECT_WRITE<<6|RA8876_READ_MEMORY_LRTB<<4|RA8876_WRITE_MEMORY_LRTB<<1);

	lcdRegWrite(RA8876_ICR);//03h
	lcdDataWrite(RA8877_LVDS_FORMAT<<3|RA8876_GRAPHIC_MODE<<2|RA8876_MEMORY_SELECT_IMAGE);

	lcdRegWrite(RA8876_MPWCTR);//10h
	lcdDataWrite(RA8876_PIP1_WINDOW_DISABLE<<7|RA8876_PIP2_WINDOW_DISABLE<<6|RA8876_SELECT_CONFIG_PIP1<<4
	|RA8876_IMAGE_COLOCR_DEPTH_16BPP<<2|TFT_MODE);

	lcdRegWrite(RA8876_PIPCDEP);//11h
	lcdDataWrite(RA8876_PIP1_COLOR_DEPTH_16BPP<<2|RA8876_PIP2_COLOR_DEPTH_16BPP);
  
	lcdRegWrite(RA8876_AW_COLOR);//5Eh
	lcdDataWrite(RA8876_CANVAS_BLOCK_MODE<<2|RA8876_CANVAS_COLOR_DEPTH_16BPP);
  
	lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_S0_COLOR_DEPTH_16BPP);//92h
  
	/* TFT timing configure (1024x600) */
	lcdRegWrite(RA8876_DPCR);//12h
	lcdDataWrite(XPCLK_INV<<7|RA8876_DISPLAY_OFF<<6|RA8876_OUTPUT_RGB);
 
	lcdRegWrite(RA8876_PCSR);//13h
	lcdDataWrite(XHSYNC_INV<<7|XVSYNC_INV<<6|XDE_INV<<5);
    
	lcdHorizontalWidthVerticalHeight(HDW,VDH);
	lcdHorizontalNonDisplay(HND);
	lcdHsyncStartPosition(HST);
	lcdHsyncPulseWidth(HPW);
	lcdVerticalNonDisplay(VND);
	lcdVsyncStartPosition(VST);
	lcdVsyncPulseWidth(VPW);
	
	// Init Global Variables
	currentPage = PAGE1_START_ADDR; // set default screen page address
	pageOffset = 0;
	_cursorX = 0;
	_cursorY = 0;
	CharPosX = 0;
	CharPosY = 0;
	_FNTwidth = 8; // Default font width
	_FNTheight = 16; // Default font height;
	_scaleX = 1;
	_scaleY = 1;
	_textMode = true;
	_width = 	SCREEN_WIDTH;
	_height = 	SCREEN_HEIGHT;
	prompt_size = 1 * (_FNTwidth * _scaleX); // prompt ">"
	vdata = 0;  // Used in tft_print()
	leftmarg = 0;
	topmarg = 0;
	rightmarg =  (uint8_t)(_width / (_FNTwidth  * _scaleX));
	bottommarg = (uint8_t)(_height / (_FNTheight * _scaleY));
	_scrollXL = 0;
	_scrollXR = _width;
	_scrollYT = 0;
	_scrollYB = _height;
	_cursorX = _scrollXL;
	_cursorY = _scrollYT;
	CharPosX = _scrollXL;
	CharPosY = _scrollYT;
	tab_size = 8;
	_cursorXsize = _FNTwidth;
	_cursorYsize = _FNTheight;
	_TXTForeColor = COLOR65K_WHITE;
	_TXTBackColor = COLOR65K_DARKBLUE;
	RA8876_BUSY = false;
  return true;
}

//**************************************************************//
// Write to a RA8876 register
//**************************************************************//
void Ra8876_Lite::lcdRegWrite(ru8 reg) 
{
  RA8876_BUSY = true;
  startSend();
  SPI.transfer(RA8876_SPI_CMDWRITE);
  SPI.transfer(reg);
  endSend();
	RA8876_BUSY = false;
}

//**************************************************************//
// Write RA8876 Data
//**************************************************************//
void Ra8876_Lite::lcdDataWrite(ru8 data) 
{
  RA8876_BUSY = true;
  startSend();
  SPI.transfer(RA8876_SPI_DATAWRITE);
  SPI.transfer(data);
  endSend();
	RA8876_BUSY = false;
}

//**************************************************************//
// Read RA8876 Data
//**************************************************************//
ru8 Ra8876_Lite::lcdDataRead(void) 
{
  RA8876_BUSY = true;
  startSend();
  SPI.transfer(RA8876_SPI_DATAREAD);
  ru8 data = SPI.transfer(0xff);
  endSend();
    RA8876_BUSY = false;
  return data;
}

//**************************************************************//
// Read RA8876 status register
//**************************************************************//
ru8 Ra8876_Lite::lcdStatusRead(void) 
{
  RA8876_BUSY = true;
  startSend();
  SPI.transfer(RA8876_SPI_STATUSREAD);
  ru8 data= SPI.transfer(0xff);
  endSend();
	RA8876_BUSY = false;
  return data;
}

//**************************************************************//
// Write Data to a RA8876 register
//**************************************************************//
void Ra8876_Lite::lcdRegDataWrite(ru8 reg,ru8 data)
{
  lcdRegWrite(reg);
  lcdDataWrite(data);
}

//**************************************************************//
// Read a RA8876 register Data
//**************************************************************//
ru8 Ra8876_Lite::lcdRegDataRead(ru8 reg)
{
  lcdRegWrite(reg);
  return lcdDataRead();
}

//**************************************************************//
// support SPI interface to write 16bpp data after Regwrite 04h
//**************************************************************//
void Ra8876_Lite::lcdDataWrite16bbp(ru16 data) 
{
	RA8876_BUSY = true;
	startSend();
	SPI.transfer(RA8876_SPI_DATAWRITE);
	SPI.transfer16(data);
	endSend();
	RA8876_BUSY = false;
}

//**************************************************************//
//RA8876 register 
//**************************************************************//
/*[Status Register] bit7  Host Memory Write FIFO full
0: Memory Write FIFO is not full.
1: Memory Write FIFO is full.
Only when Memory Write FIFO is not full, MPU may write another one pixel.*/ 
//**************************************************************//
void Ra8876_Lite::checkWriteFifoNotFull(void)
{  ru16 i;  
   for(i=0;i<10000;i++) //Please according to your usage to modify i value.
   {
    if( (lcdStatusRead()&0x80)==0 ){break;}
   }
}

//**************************************************************//
/*[Status Register] bit6  Host Memory Write FIFO empty
0: Memory Write FIFO is not empty.
1: Memory Write FIFO is empty.
When Memory Write FIFO is empty, MPU may write 8bpp data 64
pixels, or 16bpp data 32 pixels, 24bpp data 16 pixels directly.*/
//**************************************************************//
void Ra8876_Lite::checkWriteFifoEmpty(void)
{ ru16 i;
   for(i=0;i<10000;i++)   //Please according to your usage to modify i value.
   {
    if( (lcdStatusRead()&0x40)==0x40 ){break;}
   }
}

//**************************************************************//
/*[Status Register] bit5  Host Memory Read FIFO full
0: Memory Read FIFO is not full.
1: Memory Read FIFO is full.
When Memory Read FIFO is full, MPU may read 8bpp data 32
pixels, or 16bpp data 16 pixels, 24bpp data 8 pixels directly.*/
//**************************************************************//
void Ra8876_Lite::checkReadFifoNotFull(void)
{ ru16 i;
  for(i=0;i<10000;i++)  //Please according to your usage to modify i value.
  {if( (lcdStatusRead()&0x20)==0x00 ){break;}}
}

//**************************************************************//
/*[Status Register] bit4   Host Memory Read FIFO empty
0: Memory Read FIFO is not empty.
1: Memory Read FIFO is empty.*/
//**************************************************************//
void Ra8876_Lite::checkReadFifoNotEmpty(void)
{ ru16 i;
  for(i=0;i<10000;i++)// //Please according to your usage to modify i value. 
  {if( (lcdStatusRead()&0x10)==0x00 ){break;}}
}

//**************************************************************//
/*[Status Register] bit3   Core task is busy
Following task is running:
BTE, Geometry engine, Serial flash DMA, Text write or Graphic write
0: task is done or idle.   1: task is busy*/
//**************************************************************//
void Ra8876_Lite::check2dBusy(void)  
{  ru32 i,j; 
   for(i=0;i<5000000;i++)   //Please according to your usage to modify i value.
   { 
//   delayMicroseconds(1);
    if( (j = lcdStatusRead()&0x08)==0x00 )
    {break;}
   }
}  


//**************************************************************//
/*[Status Register] bit2   SDRAM ready for access
0: SDRAM is not ready for access   1: SDRAM is ready for access*/	
//**************************************************************//
boolean Ra8876_Lite::checkSdramReady(void)
{ru32 i;
 for(i=0;i<1000000;i++) //Please according to your usage to modify i value.
 { 
   delayMicroseconds(1);
   if( (lcdStatusRead()&0x04)==0x04 )
    {return true;}
    
 }
 return false;
}

//**************************************************************//
/*[Status Register] bit1  Operation mode status
0: Normal operation state  1: Inhibit operation state
Inhibit operation state means internal reset event keep running or
initial display still running or chip enter power saving state.	*/
//**************************************************************//
boolean Ra8876_Lite::checkIcReady(void)
{ru32 i;
  for(i=0;i<1000000;i++)  //Please according to your usage to modify i value.
   {
     delayMicroseconds(1);
     if( (lcdStatusRead()&0x02)==0x00 )
    {return true;}     
   }
   return false;
}

//**************************************************************//
// Initialize PLL
//**************************************************************//
//[05h] [06h] [07h] [08h] [09h] [0Ah]
//------------------------------------//----------------------------------*/
boolean Ra8876_Lite::ra8876PllInitial(void) 
{
/*(1) 10MHz <= OSC_FREQ <= 15MHz 
  (2) 10MHz <= (OSC_FREQ/PLLDIVM) <= 40MHz
  (3) 250MHz <= [OSC_FREQ/(PLLDIVM+1)]x(PLLDIVN+1) <= 600MHz
PLLDIVM:0
PLLDIVN:1~63
PLLDIVK:CPLL & MPLL = 1/2/4/8.SPLL = 1/2/4/8/16/32/64/128.
ex:
 OSC_FREQ = 10MHz
 Set X_DIVK=2
 Set X_DIVM=0
 => (X_DIVN+1)=(XPLLx4)/10*/
//ru16 x_Divide,PLLC1,PLLC2;
//ru16 pll_m_lo, pll_m_hi;
//ru8 temp;

	// Set tft output pixel clock
		if(SCAN_FREQ>=79)								//&&(SCAN_FREQ<=100))
		{
			lcdRegDataWrite(0x05,0x04);				//PLL Divided by 4
			lcdRegDataWrite(0x06,(SCAN_FREQ*4/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>=63)&&(SCAN_FREQ<=78))
		{
			lcdRegDataWrite(0x05,0x05);				//PLL Divided by 4
			lcdRegDataWrite(0x06,(SCAN_FREQ*8/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>=40)&&(SCAN_FREQ<=62))
		{								  	
			lcdRegDataWrite(0x05,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x06,(SCAN_FREQ*8/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>=32)&&(SCAN_FREQ<=39))
		{								  	
			lcdRegDataWrite(0x05,0x07);				//PLL Divided by 8
			lcdRegDataWrite(0x06,(SCAN_FREQ*16/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>=16)&&(SCAN_FREQ<=31))
		{								  	
			lcdRegDataWrite(0x05,0x16);				//PLL Divided by 16
			lcdRegDataWrite(0x06,(SCAN_FREQ*16/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>=8)&&(SCAN_FREQ<=15))
		{
			lcdRegDataWrite(0x05,0x26);				//PLL Divided by 32
			lcdRegDataWrite(0x06,(SCAN_FREQ*32/OSC_FREQ)-1);
		}
		else if((SCAN_FREQ>0)&&(SCAN_FREQ<=7))
		{
			lcdRegDataWrite(0x05,0x36);				//PLL Divided by 64
//			lcdRegDataWrite(0x06,(SCAN_FREQ*64/OSC_FREQ)-1);
			lcdRegDataWrite(0x06,((SCAN_FREQ*64/OSC_FREQ)-1) & 0x3f);
		}								    
		// Set internal Buffer Ram clock
		if(DRAM_FREQ>=158)							//
		{
			lcdRegDataWrite(0x07,0x02);				//PLL Divided by 4
			lcdRegDataWrite(0x08,(DRAM_FREQ*2/OSC_FREQ)-1);
		}
		else if((DRAM_FREQ>=125)&&(DRAM_FREQ<=157))							
		{
			lcdRegDataWrite(0x07,0x03);				//PLL Divided by 4
			lcdRegDataWrite(0x08,(DRAM_FREQ*4/OSC_FREQ)-1);
		}
		else if((DRAM_FREQ>=79)&&(DRAM_FREQ<=124))					
		{
			lcdRegDataWrite(0x07,0x04);				//PLL Divided by 4
			lcdRegDataWrite(0x08,(DRAM_FREQ*4/OSC_FREQ)-1);
		}
		else if((DRAM_FREQ>=63)&&(DRAM_FREQ<=78))					
		{
			lcdRegDataWrite(0x07,0x05);				//PLL Divided by 4
			lcdRegDataWrite(0x08,(DRAM_FREQ*8/OSC_FREQ)-1);
		}
		else if((DRAM_FREQ>=40)&&(DRAM_FREQ<=62))
		{								  	
			lcdRegDataWrite(0x07,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x08,(DRAM_FREQ*8/OSC_FREQ)-1);
		}
		else if((DRAM_FREQ>=32)&&(DRAM_FREQ<=39))
		{								  	
			lcdRegDataWrite(0x07,0x07);				//PLL Divided by 16
			lcdRegDataWrite(0x08,(DRAM_FREQ*16/OSC_FREQ)-1);
		}
		else if(DRAM_FREQ<=31)
		{
			lcdRegDataWrite(0x07,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x08,(30*8/OSC_FREQ)-1);	//set to 30MHz if out off range
		}
		// Set Core clock
		if(CORE_FREQ>=158)
		{
			lcdRegDataWrite(0x09,0x02);				//PLL Divided by 2
			lcdRegDataWrite(0x0A,(CORE_FREQ*2/OSC_FREQ)-1);
		}
		else if((CORE_FREQ>=125)&&(CORE_FREQ<=157))
		{
			lcdRegDataWrite(0x09,0x03);				//PLL Divided by 4
			lcdRegDataWrite(0x0A,(CORE_FREQ*4/OSC_FREQ)-1);
		}
		else if((CORE_FREQ>=79)&&(CORE_FREQ<=124))					
		{
			lcdRegDataWrite(0x09,0x04);				//PLL Divided by 4
			lcdRegDataWrite(0x0A,(CORE_FREQ*4/OSC_FREQ)-1);
		}
		else if((CORE_FREQ>=63)&&(CORE_FREQ<=78))					
		{
			lcdRegDataWrite(0x09,0x05);				//PLL Divided by 8
			lcdRegDataWrite(0x0A,(CORE_FREQ*8/OSC_FREQ)-1);
		}
		else if((CORE_FREQ>=40)&&(CORE_FREQ<=62))
		{								  	
			lcdRegDataWrite(0x09,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x0A,(CORE_FREQ*8/OSC_FREQ)-1);
		}
		else if((CORE_FREQ>=32)&&(CORE_FREQ<=39))
		{								  	
			lcdRegDataWrite(0x09,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x0A,(CORE_FREQ*8/OSC_FREQ)-1);
		}
		else if(CORE_FREQ<=31)
		{
			lcdRegDataWrite(0x09,0x06);				//PLL Divided by 8
			lcdRegDataWrite(0x0A,(30*8/OSC_FREQ)-1);	//set to 30MHz if out off range
		}

	delay(1);
	lcdRegWrite(0x01);
	lcdDataWrite(0x80);
	delay(2); //wait for pll stable
	if((lcdDataRead()&0x80)==0x80)
		return true;
	else
		return false; 
}

//**************************************************************//
// Initialize SDRAM
//**************************************************************//
boolean Ra8876_Lite::ra8876SdramInitial(void)
{
ru8	CAS_Latency;
ru16	Auto_Refresh;

#ifdef IS42SM16160D
  if(DRAM_FREQ<=133)	
  CAS_Latency=2;
  else 				
  CAS_Latency=3;

  Auto_Refresh=(64*DRAM_FREQ*1000)/(8192);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0xf9);        
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x09);
 #endif

 #ifdef IS42S16320B	
  if(DRAM_FREQ<=133)	
  CAS_Latency=2;
  else 				
  CAS_Latency=3;	
  
  Auto_Refresh=(64*DRAM_FREQ*1000)/(8192);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x32);	
  lcdRegDataWrite(0xe1,CAS_Latency);
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x09);
 #endif

#ifdef IS42S16400F
  if(DRAM_FREQ<143)	
  CAS_Latency=2;
  else 
  CAS_Latency=3;
  
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x28);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif

 #ifdef M12L32162A
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x08);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x09);
 #endif

 #ifdef M12L2561616A
  CAS_Latency=3;	
  Auto_Refresh=(64*DRAM_FREQ*1000)/(8192);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x31);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif

 #ifdef M12L64164A
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x28);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x09);
 #endif

 #ifdef W9825G6JH
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x31);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif

 #ifdef W9812G6JH
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);	
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x29);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif

 #ifdef MT48LC4M16A
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
	Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x28);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif

 #ifdef K4S641632N
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x28);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
#endif

#ifdef K4S281632K
  CAS_Latency=3;
  Auto_Refresh=(64*DRAM_FREQ*1000)/(4096);	
  Auto_Refresh=Auto_Refresh-2; 
  lcdRegDataWrite(0xe0,0x29);      
  lcdRegDataWrite(0xe1,CAS_Latency);      //CAS:2=0x02，CAS:3=0x03
  lcdRegDataWrite(0xe2,Auto_Refresh);
  lcdRegDataWrite(0xe3,Auto_Refresh>>8);
  lcdRegDataWrite(0xe4,0x01);
 #endif
  return checkSdramReady();
}

//**************************************************************//
// Turn Display ON/Off (true = ON)
//**************************************************************//
 void Ra8876_Lite::displayOn(boolean on)
 {
  if(on)
   lcdRegDataWrite(RA8876_DPCR, XPCLK_INV<<7|RA8876_DISPLAY_ON<<6|RA8876_OUTPUT_RGB);
  else
   lcdRegDataWrite(RA8876_DPCR, XPCLK_INV<<7|RA8876_DISPLAY_OFF<<6|RA8876_OUTPUT_RGB);
   
  delay(20);
 }

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdHorizontalWidthVerticalHeight(ru16 width,ru16 height)
{
	unsigned char temp;
	temp=(width/8)-1;
	lcdRegDataWrite(RA8876_HDWR,temp);
	temp=width%8;
	lcdRegDataWrite(RA8876_HDWFTR,temp);
	temp=height-1;
	lcdRegDataWrite(RA8876_VDHR0,temp);
	temp=(height-1)>>8;
	lcdRegDataWrite(RA8876_VDHR1,temp);
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdHorizontalNonDisplay(ru16 numbers)
{
	ru8 temp;
	if(numbers<8)
	{
		lcdRegDataWrite(RA8876_HNDR,0x00);
		lcdRegDataWrite(RA8876_HNDFTR,numbers);
	} else {
		temp=(numbers/8)-1;
		lcdRegDataWrite(RA8876_HNDR,temp);
		temp=numbers%8;
		lcdRegDataWrite(RA8876_HNDFTR,temp);
	}	
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdHsyncStartPosition(ru16 numbers)
{
	ru8 temp;
	if(numbers<8)
	{
		lcdRegDataWrite(RA8876_HSTR,0x00);
	} else {
		temp=(numbers/8)-1;
		lcdRegDataWrite(RA8876_HSTR,temp);
	}
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdHsyncPulseWidth(ru16 numbers)
{
	ru8 temp;
	if(numbers<8)
	{
		lcdRegDataWrite(RA8876_HPWR,0x00);
	} else {
		temp=(numbers/8)-1;
		lcdRegDataWrite(RA8876_HPWR,temp);
	}
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdVerticalNonDisplay(ru16 numbers)
{
	ru8 temp;
	temp=numbers-1;
	lcdRegDataWrite(RA8876_VNDR0,temp);
	lcdRegDataWrite(RA8876_VNDR1,temp>>8);
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdVsyncStartPosition(ru16 numbers)
{
	ru8 temp;
	temp=numbers-1;
	lcdRegDataWrite(RA8876_VSTR,temp);
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::lcdVsyncPulseWidth(ru16 numbers)
{
	ru8 temp;
	temp=numbers-1;
	lcdRegDataWrite(RA8876_VPWR,temp);
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::displayImageStartAddress(ru32 addr)	
{
	lcdRegDataWrite(RA8876_MISA0,addr);//20h
	lcdRegDataWrite(RA8876_MISA1,addr>>8);//21h 
	lcdRegDataWrite(RA8876_MISA2,addr>>16);//22h  
	lcdRegDataWrite(RA8876_MISA3,addr>>24);//23h 
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::displayImageWidth(ru16 width)	
{
	lcdRegDataWrite(RA8876_MIW0,width); //24h
	lcdRegDataWrite(RA8876_MIW1,width>>8); //25h 
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::displayWindowStartXY(ru16 x0,ru16 y0)	
{
	lcdRegDataWrite(RA8876_MWULX0,x0);//26h
	lcdRegDataWrite(RA8876_MWULX1,x0>>8);//27h
	lcdRegDataWrite(RA8876_MWULY0,y0);//28h
	lcdRegDataWrite(RA8876_MWULY1,y0>>8);//29h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::canvasImageStartAddress(ru32 addr)	
{
	lcdRegDataWrite(RA8876_CVSSA0,addr);//50h
	lcdRegDataWrite(RA8876_CVSSA1,addr>>8);//51h
	lcdRegDataWrite(RA8876_CVSSA2,addr>>16);//52h
	lcdRegDataWrite(RA8876_CVSSA3,addr>>24);//53h  
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::canvasImageWidth(ru16 width)	
{
	lcdRegDataWrite(RA8876_CVS_IMWTH0,width);//54h
	lcdRegDataWrite(RA8876_CVS_IMWTH1,width>>8); //55h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::activeWindowXY(ru16 x0,ru16 y0)	
{
	lcdRegDataWrite(RA8876_AWUL_X0,x0);//56h
	lcdRegDataWrite(RA8876_AWUL_X1,x0>>8);//57h 
	lcdRegDataWrite(RA8876_AWUL_Y0,y0);//58h
	lcdRegDataWrite(RA8876_AWUL_Y1,y0>>8);//59h 
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::activeWindowWH(ru16 width,ru16 height)	
{
	lcdRegDataWrite(RA8876_AW_WTH0,width);//5ah
	lcdRegDataWrite(RA8876_AW_WTH1,width>>8);//5bh
	lcdRegDataWrite(RA8876_AW_HT0,height);//5ch
	lcdRegDataWrite(RA8876_AW_HT1,height>>8);//5dh  
}

//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::setPixelCursor(ru16 x,ru16 y)
{
	lcdRegDataWrite(RA8876_CURH0,x); //5fh
	lcdRegDataWrite(RA8876_CURH1,x>>8);//60h
	lcdRegDataWrite(RA8876_CURV0,y);//61h
	lcdRegDataWrite(RA8876_CURV1,y>>8);//62h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::linearAddressSet(ru32 addr)	
{
	lcdRegDataWrite(RA8876_CURH0,addr); //5fh
	lcdRegDataWrite(RA8876_CURH1,addr>>8); //60h
	lcdRegDataWrite(RA8876_CURV0,addr>>16); //61h
	lcdRegDataWrite(RA8876_CURV1,addr>>24); //62h
}

//**************************************************************//
//**************************************************************//
ru8 Ra8876_Lite::vmemReadData(ru32 addr)	
{
	ru8 vmemData = 0;
  
	graphicMode(true);
	Memory_Linear_Mode();
	linearAddressSet(addr);
	ramAccessPrepare();
	vmemData = lcdDataRead(); // dummyread to alert SPI
	vmemData = lcdDataRead(); // read byte
	check2dBusy();
	Memory_XY_Mode();
	graphicMode(false);
	return vmemData;
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::vmemWriteData(ru32 addr, ru8 vmemData)	
{
	graphicMode(true);
	Memory_Linear_Mode();
	linearAddressSet(addr);
	ramAccessPrepare();
//  checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
	lcdDataWrite(vmemData);
//  checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
	graphicMode(false);
	Memory_XY_Mode();
	graphicMode(false);
}

//**************************************************************//
//**************************************************************//
ru16 Ra8876_Lite::vmemReadData16(ru32 addr)	
{
	ru16 vmemData = 0;
  
	vmemData = (vmemReadData(addr) & 0xff); // lo byte
	vmemData |= vmemReadData(addr+1) << 8; // hi byte
	return vmemData;
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::vmemWriteData16(ru32 addr, ru16 vmemData)	
{
	vmemWriteData(addr,vmemData); // lo byte
	vmemWriteData(addr+1,vmemData>>8); // hi byte
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source0_MemoryStartAddr(ru32 addr)	
{
	lcdRegDataWrite(RA8876_S0_STR0,addr);//93h
	lcdRegDataWrite(RA8876_S0_STR1,addr>>8);//94h
	lcdRegDataWrite(RA8876_S0_STR2,addr>>16);//95h
	lcdRegDataWrite(RA8876_S0_STR3,addr>>24);////96h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source0_ImageWidth(ru16 width)	
{
	lcdRegDataWrite(RA8876_S0_WTH0,width);//97h
	lcdRegDataWrite(RA8876_S0_WTH1,width>>8);//98h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source0_WindowStartXY(ru16 x0,ru16 y0)	
{
	lcdRegDataWrite(RA8876_S0_X0,x0);//99h
	lcdRegDataWrite(RA8876_S0_X1,x0>>8);//9ah
	lcdRegDataWrite(RA8876_S0_Y0,y0);//9bh
	lcdRegDataWrite(RA8876_S0_Y1,y0>>8);//9ch
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source1_MemoryStartAddr(ru32 addr)	
{
	lcdRegDataWrite(RA8876_S1_STR0,addr);//9dh
	lcdRegDataWrite(RA8876_S1_STR1,addr>>8);//9eh
	lcdRegDataWrite(RA8876_S1_STR2,addr>>16);//9fh
	lcdRegDataWrite(RA8876_S1_STR3,addr>>24);//a0h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source1_ImageWidth(ru16 width)	
{
	lcdRegDataWrite(RA8876_S1_WTH0,width);//a1h
	lcdRegDataWrite(RA8876_S1_WTH1,width>>8);//a2h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bte_Source1_WindowStartXY(ru16 x0,ru16 y0)	
{
	lcdRegDataWrite(RA8876_S1_X0,x0);//a3h
	lcdRegDataWrite(RA8876_S1_X1,x0>>8);//a4h
	lcdRegDataWrite(RA8876_S1_Y0,y0);//a5h
	lcdRegDataWrite(RA8876_S1_Y1,y0>>8);//a6h
}

//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::bte_DestinationMemoryStartAddr(ru32 addr)	
{
	lcdRegDataWrite(RA8876_DT_STR0,addr);//a7h
	lcdRegDataWrite(RA8876_DT_STR1,addr>>8);//a8h
	lcdRegDataWrite(RA8876_DT_STR2,addr>>16);//a9h
	lcdRegDataWrite(RA8876_DT_STR3,addr>>24);//aah
}

//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::bte_DestinationImageWidth(ru16 width)	
{
	lcdRegDataWrite(RA8876_DT_WTH0,width);//abh
	lcdRegDataWrite(RA8876_DT_WTH1,width>>8);//ach
}

//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::bte_DestinationWindowStartXY(ru16 x0,ru16 y0)	
{
	lcdRegDataWrite(RA8876_DT_X0,x0);//adh
	lcdRegDataWrite(RA8876_DT_X1,x0>>8);//aeh
	lcdRegDataWrite(RA8876_DT_Y0,y0);//afh
	lcdRegDataWrite(RA8876_DT_Y1,y0>>8);//b0h
}

//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::bte_WindowSize(ru16 width, ru16 height)
{
	lcdRegDataWrite(RA8876_BTE_WTH0,width);//b1h
	lcdRegDataWrite(RA8876_BTE_WTH1,width>>8);//b2h
	lcdRegDataWrite(RA8876_BTE_HIG0,height);//b3h
	lcdRegDataWrite(RA8876_BTE_HIG1,height>>8);//b4h
}

//**************************************************************//
// These 8 bits determine prescaler value for Timer 0 and 1.*/
// Time base is “Core_Freq / (Prescaler + 1)”*/
//**************************************************************//
void Ra8876_Lite::pwm_Prescaler(ru8 prescaler)
{
	lcdRegDataWrite(RA8876_PSCLR,prescaler);//84h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm_ClockMuxReg(ru8 pwm1_clk_div, ru8 pwm0_clk_div, ru8 xpwm1_ctrl, ru8 xpwm0_ctrl)
{
	lcdRegDataWrite(RA8876_PMUXR,pwm1_clk_div<<6|pwm0_clk_div<<4|xpwm1_ctrl<<2|xpwm0_ctrl);//85h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm_Configuration(ru8 pwm1_inverter,ru8 pwm1_auto_reload,ru8 pwm1_start,ru8 
                       pwm0_dead_zone, ru8 pwm0_inverter, ru8 pwm0_auto_reload,ru8 pwm0_start) {
	lcdRegDataWrite(RA8876_PCFGR,pwm1_inverter<<6|pwm1_auto_reload<<5|pwm1_start<<4|pwm0_dead_zone<<3|
	pwm0_inverter<<2|pwm0_auto_reload<<1|pwm0_start);//86h                
}   

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm0_Duty(ru16 duty)
{
	lcdRegDataWrite(RA8876_TCMPB0L,duty);//88h 
	lcdRegDataWrite(RA8876_TCMPB0H,duty>>8);//89h 
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm0_ClocksPerPeriod(ru16 clocks_per_period)
{
	lcdRegDataWrite(RA8876_TCNTB0L,clocks_per_period);//8ah
	lcdRegDataWrite(RA8876_TCNTB0H,clocks_per_period>>8);//8bh
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm1_Duty(ru16 duty)
{
	lcdRegDataWrite(RA8876_TCMPB1L,duty);//8ch 
	lcdRegDataWrite(RA8876_TCMPB1H,duty>>8);//8dh
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::pwm1_ClocksPerPeriod(ru16 clocks_per_period)
{
	lcdRegDataWrite(RA8876_TCNTB1L,clocks_per_period);//8eh
	lcdRegDataWrite(RA8876_TCNTB1F,clocks_per_period>>8);//8fh
}

//**************************************************************//
// Call this before reading or writing to SDRAM
//**************************************************************//
void  Ra8876_Lite::ramAccessPrepare(void)
{
	lcdRegWrite(RA8876_MRWDP); //04h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::foreGroundColor16bpp(ru16 color)
{
	lcdRegDataWrite(RA8876_FGCR,color>>8);//d2h
	lcdRegDataWrite(RA8876_FGCG,color>>3);//d3h
	lcdRegDataWrite(RA8876_FGCB,color<<3);//d4h
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::backGroundColor16bpp(ru16 color)
{
	lcdRegDataWrite(RA8876_BGCR,color>>8);//d5h
	lcdRegDataWrite(RA8876_BGCG,color>>3);//d6h
	lcdRegDataWrite(RA8876_BGCB,color<<3);//d7h
}

//***************************************************//
/*                 GRAPHIC FUNCTIONS                 */
//***************************************************//

//**************************************************************//
/* Turn RA8876 graphic mode ON/OFF (True = ON)                  */
//**************************************************************//
void Ra8876_Lite::graphicMode(boolean on)
 {
	if(on)
		lcdRegDataWrite(RA8876_ICR,RA8877_LVDS_FORMAT<<3|RA8876_GRAPHIC_MODE<<2|RA8876_MEMORY_SELECT_IMAGE);//03h  //switch to graphic mode
	else
		lcdRegDataWrite(RA8876_ICR,RA8877_LVDS_FORMAT<<3|RA8876_TEXT_MODE<<2|RA8876_MEMORY_SELECT_IMAGE);//03h  //switch back to text mode
 }

//**************************************************************//
/* Read a 16bpp pixel                                           */
//**************************************************************//
ru16 Ra8876_Lite::getPixel_16bpp(ru16 x,ru16 y)
{
	ru16 rdata = 0;
	graphicMode(true);
	setPixelCursor(x, y);		// set memory address
	ramAccessPrepare();			// Setup SDRAM Access
	rdata = lcdDataRead();		// dummyread to alert SPI
	rdata = lcdDataRead();		// read low byte
	rdata |= lcdDataRead()<<8;	// add high byte 
	check2dBusy();				// Wait for completion
	graphicMode(false);			// Switch to Text Mode
	return rdata;
}

//**************************************************************//
/* Write a 16bpp pixel                                          */
//**************************************************************//
void  Ra8876_Lite::putPixel_16bpp(ru16 x,ru16 y,ru16 color)
{
	graphicMode(true);
	setPixelCursor(x,y);
	ramAccessPrepare();
	checkWriteFifoNotFull();	//if high speed mcu and without Xnwait check
	lcdDataWrite(color);
	lcdDataWrite(color>>8);
// lcdDataWrite16bbp(color);
	checkWriteFifoEmpty();		//if high speed mcu and without Xnwait check
	graphicMode(false);
}

//**************************************************************//
/* Write 16bpp(RGB565) picture data for user operation */
//**************************************************************//
void  Ra8876_Lite::putPicture_16bpp(ru16 x,ru16 y,ru16 width, ru16 height)
{
	graphicMode(true);
	activeWindowXY(x,y);
	activeWindowWH(width,height);
	setPixelCursor(x,y);
	ramAccessPrepare();
// SOMETHING IS MISSING HERE !!!
	graphicMode(false);
}

//*******************************************************************//
/* write 16bpp(RGB565) picture data in byte format from data pointer */
//*******************************************************************//
void  Ra8876_Lite::putPicture_16bppData8(ru16 x,ru16 y,ru16 width, ru16 height, const unsigned char *data)
{
	ru16 i,j;

	graphicMode(true);
	activeWindowXY(x,y);
	activeWindowWH(width,height);
	setPixelCursor(x,y);
	ramAccessPrepare();
	for(j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			//checkWriteFifoNotFull();	//if high speed mcu and without Xnwait check
			lcdDataWrite(*data);
			data++;
			//checkWriteFifoNotFull();	//if high speed mcu and without Xnwait check
			lcdDataWrite(*data);
			data++;
		}
	} 
	checkWriteFifoEmpty();				//if high speed mcu and without Xnwait check
	activeWindowXY(0,0);
// activeWindowWH(SCREEN_WIDTH,SCREEN_HEIGHT);
	activeWindowWH(SCREEN_WIDTH,600);
	graphicMode(false);
}

//****************************************************************//
/* Write 16bpp(RGB565) picture data word format from data pointer */
//****************************************************************//
void  Ra8876_Lite::putPicture_16bppData16(ru16 x,ru16 y,ru16 width, ru16 height, const unsigned short *data)
{
	ru16 i,j;
	graphicMode(true);
	activeWindowXY(x,y);	
	activeWindowWH(width,height);
	setPixelCursor(x,y);
	ramAccessPrepare();
	for(j=0;j<height;j++) {
		for(i=0;i<width;i++) {
			//checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
			lcdDataWrite16bbp(*data);
			data++;
			//checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
		}
	} 
	checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
	activeWindowXY(0,0);
	// activeWindowWH(SCREEN_WIDTH,SCREEN_HEIGHT);
	activeWindowWH(SCREEN_WIDTH,600);
	graphicMode(false);
}

//***************************************************//
/*                 TEXT FUNCTIONS                    */
//***************************************************//

//**************************************************************//
/* Turn on RA8876 Text Mode, (Same Graphics Mode in reverse)    */
//**************************************************************//
void Ra8876_Lite::textMode(boolean on)
{
	if(on)
		lcdRegDataWrite(RA8876_ICR,RA8877_LVDS_FORMAT<<3|RA8876_TEXT_MODE<<2|RA8876_MEMORY_SELECT_IMAGE);//03h  //switch to text mode
	else
		lcdRegDataWrite(RA8876_ICR,RA8877_LVDS_FORMAT<<3|RA8876_GRAPHIC_MODE<<2|RA8876_MEMORY_SELECT_IMAGE);//03h  //switch back to graphic mode

}

//**************************************************************//
/* Set text Foreground and Bckground colors (16 bit)            */
//**************************************************************//
void Ra8876_Lite::textColor(ru16 foreground_color,ru16 background_color)
{
	foreGroundColor16bpp(foreground_color);
	backGroundColor16bpp(background_color);
	_TXTForeColor = foreground_color;
	_TXTBackColor = background_color;
}

//**************************************************************//
/* Position Text Cursor                                         */
//**************************************************************//
void  Ra8876_Lite::setTextCursor(ru16 x,ru16 y)
{
	lcdRegDataWrite(RA8876_F_CURX0,x); //63h
	lcdRegDataWrite(RA8876_F_CURX1,x>>8);//64h
	lcdRegDataWrite(RA8876_F_CURY0,y);//65h
	lcdRegDataWrite(RA8876_F_CURY1,y>>8);//66h
	_cursorX = x; _cursorY = y; // Update global cursor position variables
}

//***************************************************************//
/* Used by ClearActiveScreen(), (Window OP's)                    */
//***************************************************************//
void Ra8876_Lite::textxy(ru16 x, ru16 y)
{
	x += (_scrollXL / (_FNTwidth  * _scaleX));
	y += (_scrollYT / (_FNTheight * _scaleY));
	if(x > (_scrollXR / (_FNTwidth  * _scaleX)))
		x = _scrollXR / (_FNTwidth  * _scaleX)-1;
	if(y > (_scrollYB / (_FNTheight  * _scaleY)))
		y = _scrollYB / (_FNTheight  * _scaleY)-1;
	setTextCursor(x * (_FNTwidth  * _scaleX), y * (_FNTheight * _scaleY));
}

//**************************************************************//
/* source_select = 0 : internal CGROM,  source_select = 1: external CGROM, source_select = 2: user-define*/
/* size_select = 0 : 8*16/16*16, size_select = 1 : 12*24/24*24, size_select = 2 : 16*32/32*32  */
/* iso_select = 0 : iso8859-1, iso_select = 1 : iso8859-2, iso_select = 2 : iso8859-4, iso_select = 3 : iso8859-5*/
//**************************************************************//
void Ra8876_Lite::setTextParameter1(ru8 source_select,ru8 size_select,ru8 iso_select)
{
	lcdRegDataWrite(RA8876_CCR0,source_select<<6|size_select<<4|iso_select); //cch
}

//***********************************************************************************************//
/* align = 0 : full alignment disable, align = 1 : full alignment enable                         */
/* chroma_key = 0 : text with chroma key disable, chroma_key = 1 : text with chroma key enable   */
/* width_enlarge and height_enlarge can be set 0~3, (00b: X1) (01b : X2)  (10b : X3)  (11b : X4) */
//***********************************************************************************************//
void Ra8876_Lite::setTextParameter2(ru8 align, ru8 chroma_key, ru8 width_enlarge, ru8 height_enlarge)
{
	lcdRegDataWrite(RA8876_CCR1,align<<7|chroma_key<<6|width_enlarge<<2|height_enlarge);//cdh
}

//**************************************************************//
/* Setup SPI to read character rom selected font set            */
//**************************************************************//
void Ra8876_Lite::genitopCharacterRomParameter(ru8 scs_select, ru8 clk_div, ru8 rom_select, ru8 character_select, ru8 gt_width)
{ 
  if(scs_select==0)
  lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT0<<7|RA8876_SERIAL_FLASH_FONT_MODE<<6|RA8876_SERIAL_FLASH_ADDR_24BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
  if(scs_select==1)
  lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT1<<7|RA8876_SERIAL_FLASH_FONT_MODE<<6|RA8876_SERIAL_FLASH_ADDR_24BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
  
  lcdRegDataWrite(RA8876_SPI_DIVSOR,clk_div);//bbh 
  
  lcdRegDataWrite(RA8876_GTFNT_SEL,rom_select<<5);//ceh
  lcdRegDataWrite(RA8876_GTFNT_CR,character_select<<3|gt_width);//cfh
}

//**************************************************************//
/* Print a string using internal font or external font code     */
//**************************************************************//
void  Ra8876_Lite::putString(ru16 x0,ru16 y0, const char *str)
{
  textMode(true);
  while(*str != '\0')
  {
  tftPrint(*str);
  ++str; 
  } 
  textMode(false);
}

//**************************************************************//
/* Clear the status line                                        */
//**************************************************************//
void Ra8876_Lite::clearStatusLine(uint16_t color) {
	drawSquareFill(0,SCREEN_HEIGHT,SCREEN_WIDTH-1,600,color);
}

//**************************************************************//
/* Write a string to status line using a constant 8x16 font     */
/* x0 is starting position of string                            */
/* fgcolr is foreground color                                   */
/* bgcolor is background color                                  */
/* str is a pointer to a text string                            */
//**************************************************************//
void  Ra8876_Lite::writeStatusLine(ru16 x0, uint16_t fgcolor, uint16_t bgcolor, const char *str)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	uint16_t tempBGColor = _TXTBackColor;
	uint16_t tempFGColor = _TXTForeColor;
	uint8_t tempScaleX = _scaleX;
	uint8_t tempScaleY = _scaleY;
	uint8_t tempFontWidth = _FNTwidth;
	uint8_t tempFontHeight = _FNTheight;

	// Set fontsize to a canstant value
	if(UDFont) {
		_scaleX = _scaleY = 1;
		_FNTwidth = 8;
		_FNTheight = 16;
	}// else {
//		_FNTwidth = 12;
//		_FNTheight = 24;
//	}

	buildTextScreen();
	textMode(true);
	setTextCursor(x0,576);
	textColor(fgcolor,bgcolor);
	if(!UDFont) {
		ramAccessPrepare();
		while(*str != '\0')
		{
			checkWriteFifoNotFull();  
			lcdDataWrite(*str);
			++str; 
		} 
		check2dBusy();
	}
	if(UDFont) {
		_scaleX = _scaleY = 1;
		while(*str != '\0')
		{
			CGRAM_Start_address(PATTERN1_RAM_START_ADDR);
			ramAccessPrepare();
			checkWriteFifoNotFull();  
			lcdDataWrite(*str>>8);
			checkWriteFifoNotFull();  
			lcdDataWrite(*str);
			++str; 
			x0+=_FNTwidth*_scaleX;
		} 
		check2dBusy();
	}
	_scaleX = tempScaleX;
	_scaleY = tempScaleY;
	_FNTwidth = tempFontWidth;
	_FNTheight = tempFontHeight;
	buildTextScreen();
	textMode(false);
	setTextCursor(tempX,tempY);
	textColor(tempFGColor,tempBGColor);
}

// Support function for VT100: Clear to End Of Line
void Ra8876_Lite::clreol(void)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	drawSquareFill(tempX, _cursorY, _width-1, _cursorY+(_FNTheight*_scaleY), _TXTBackColor);
	setTextCursor(tempX,tempY);
	textColor(_TXTForeColor,_TXTBackColor);
}

// Support function for VT100: Clear to End Of Screen
void Ra8876_Lite::clreos(void)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	clreol();
	drawSquareFill(0, _cursorY+(_FNTheight*_scaleY), _width-1, _height-1, _TXTBackColor);
	setTextCursor(tempX,tempY);
	textColor(_TXTForeColor,_TXTBackColor);
}

// Support function for VT100: Clear to End Of Line
void Ra8876_Lite::clrbol(void)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	drawSquareFill(0, _cursorY, tempX, _cursorY+(_FNTheight*_scaleY), _TXTBackColor);
	setTextCursor(tempX,tempY);
	textColor(_TXTForeColor,_TXTBackColor);
}

// Support function for VT100: Clear to begining of Screen
void Ra8876_Lite::clrbos(void)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	clrbol();
	drawSquareFill(0, 0, _width-1, _cursorY, _TXTBackColor);
	setTextCursor(tempX,tempY);
	textColor(_TXTForeColor,_TXTBackColor);
}

// Support function for VT100: Clear Line
void Ra8876_Lite::clrlin(void)
{
	uint16_t tempX = _cursorX;
	uint16_t tempY = _cursorY;
	setTextCursor(0,tempY);
	drawSquareFill(0, _cursorY, _width-1, _cursorY+(_FNTheight*_scaleY), _TXTBackColor);
	setTextCursor(tempX,tempY);
	textColor(_TXTForeColor,_TXTBackColor);
}

// Clear current Active Screen. Home Cursor. Set default forground and background colors.
void Ra8876_Lite::clearActiveScreen(void)
{
	drawSquareFill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, _TXTBackColor);
	textColor(_TXTForeColor,_TXTBackColor);
	textxy(0,0);
}

//**************************************************************//
/* For control code 7 (BELL Character in tftPrint())            */
//**************************************************************//
extern void tone(uint8_t pin, uint16_t frequency, uint32_t duration);

//**************************************************************//
/* Print a character to current active screen                   */
/* This function processes most ASCII control codes and will    */
/* scroll the screen up when text position is past bottom line  */
//**************************************************************//
size_t Ra8876_Lite::tftPrint(uint8_t text) {

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
	return 1;
}

//************************************************//
/* Print a character to current active screen.    */
/* This function does not process ASCII control   */
/* codes and will scroll the screen up when text  */
/* position is past bottom line.                  */
//************************************************//
size_t Ra8876_Lite::tftRawPrint(uint8_t text) {
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

//**************************************************************//
/* update display coordinates and scroll screen if needed        */
//**************************************************************//
void Ra8876_Lite::update_xy(void)
{
   if(_cursorY >= _scrollYB) 
   {
      scroll();
      _cursorY -= (_FNTheight * _scaleY);
	  _cursorX = _scrollXL;
   }
   setTextCursor(_cursorX,_cursorY);
}

//**************************************************************//
/* Write character to active text screen                        */
//**************************************************************//
void Ra8876_Lite::update_tft(uint8_t data)
{
	CGRAM_Start_address(PATTERN1_RAM_START_ADDR);
	textMode(true);
	setTextCursor(_cursorX,_cursorY);
	ramAccessPrepare();
	checkWriteFifoNotFull();  
	if(!UDFont) {
		lcdDataWrite(data);
	}
	if(UDFont) {
		lcdDataWrite(data>>8);
		checkWriteFifoNotFull();  
		lcdDataWrite(data);
	}
	check2dBusy();
}

//**************************************************************//
/* Select RA8876 fonts or user defined fonts                    */
//**************************************************************//
void Ra8876_Lite::setFontSource(uint8_t source) {
	switch(source) {
		case 0:
			UDFont = false;
			break;
		case 1:
			UDFont = true;
			break;
		default:
			UDFont = false;
	}
}

//********************************************************************//
/* Set up the current active screen based on global setup parameters  */
// for this active screen page                                        */
//********************************************************************//
void Ra8876_Lite::buildTextScreen(void)
{
	switch(_FNTheight) {
		case 16:
			_fontheight = 0;
			break;
		case 24:
			_fontheight = 1;
			break;
		case 32:
			_fontheight = 2;
			break;
		default:
			break;
	}

	leftmarg =  (uint8_t)(_scrollXL / (_FNTwidth  * _scaleX));
	topmarg =  (uint8_t)(_scrollYT / (_FNTheight  * _scaleY));
	rightmarg =  (uint8_t)(_scrollXR / (_FNTwidth  * _scaleX));
	bottommarg = (uint8_t)(_scrollYB / (_FNTheight * _scaleY));
	if(!UDFont) {
		setTextParameter1(RA8876_SELECT_INTERNAL_CGROM,_fontheight,RA8876_SELECT_8859_1);//cch
		setTextParameter2(RA8876_TEXT_FULL_ALIGN_ENABLE, RA8876_TEXT_CHROMA_KEY_DISABLE,
						  RA8876_TEXT_WIDTH_ENLARGEMENT_X1,RA8876_TEXT_HEIGHT_ENLARGEMENT_X1);//cdh
	} else {
		setTextParameter1(RA8876_SELECT_USER_DEFINED,RA8876_CHAR_HEIGHT_16,RA8876_SELECT_8859_1);//cch
		setTextParameter2(RA8876_TEXT_FULL_ALIGN_DISABLE, RA8876_TEXT_CHROMA_KEY_DISABLE,
						  _scaleX-1, _scaleY-1);//cdh
	}
	genitopCharacterRomParameter(RA8876_SERIAL_FLASH_SELECT0,RA8876_SPI_DIV4,RA8876_GT30L32S4W,
					  RA8876_ASCII, RA8876_GT_FIXED_WIDTH);
//	Text_Cursor_H_V(_cursorXsize,_cursorYsize); 		// Block cusror
	textColor(_TXTForeColor,_TXTBackColor);
}

//********************************************************************//
/* Set Character Generator Starting RAM address (32 bit aligned)      */
/* RA8876 registers[DBh]~[DEh]                                        */
//********************************************************************//
void Ra8876_Lite::CGRAM_Start_address(uint32_t Addr)
{

	//CGRAM START ADDRESS [31:0] 
	lcdRegDataWrite(RA8876_CGRAM_STR0, Addr);
	lcdRegDataWrite(RA8876_CGRAM_STR1, Addr >> 8);
	lcdRegDataWrite(RA8876_CGRAM_STR2, Addr >> 16);
	lcdRegDataWrite(RA8876_CGRAM_STR3, Addr >> 24);
}

//********************************************************************//
/* Initialize CGRAM with user defined font                            */
//********************************************************************//
void Ra8876_Lite::CGRAM_initial(uint32_t charAddr, const uint8_t *data, uint16_t count)
{
  uint16_t i;

  graphicMode(true);//switch to graphic mode
  
  lcdRegWrite(RA8876_AW_COLOR);// 5Eh 
  lcdDataWrite(RA8876_CANVAS_LINEAR_MODE<<2|RA8876_CANVAS_COLOR_DEPTH_8BPP); // set memory to 8bpp and linear mode
  linearAddressSet(charAddr); // Set linear address (32 bit)
  //Set the start address for User Define Font, and write data.
  ramAccessPrepare();
   for(i = 0; i < count; i++)
   {
    checkWriteFifoNotFull();
    lcdDataWrite(*data);
    data++;
   }
   checkWriteFifoEmpty(); // If high speed mcu and without Xnwait check
   
  lcdRegWrite(RA8876_AW_COLOR);//5Eh 
  lcdDataWrite(RA8876_CANVAS_BLOCK_MODE<<2|RA8876_CANVAS_COLOR_DEPTH_16BPP);
}


//***************************************************//
/*                 GRAPHIC FUNCTIONS                    */
//***************************************************//

//***************************************************************************//
/* Initialize graphic cursor RAM with 4 available graphic cursor shapes      */
/* These are 16x16 byte arrays, 256 bytes per image * 4 images (2048 bytes)  */  
/* Need to change this code to use User Defined cursor shapes                */
//***************************************************************************//
void Ra8876_Lite::Graphic_cursor_initial(void)
{
	unsigned int i ;

	graphicMode(true);
    Memory_Select_Graphic_Cursor_RAM(); 
	// Pen cursor
    Select_Graphic_Cursor_1();
	ramAccessPrepare();
    for(i=0;i<256;i++)
    {					 
     lcdDataWrite(gImage_pen_il[i]);
    }
	// Standard arrow cursor
    Select_Graphic_Cursor_2();
	ramAccessPrepare();
    for(i=0;i<256;i++)
    {					 
     lcdDataWrite(gImage_arrow_il[i]);
    }
	// Busy cursor
     Select_Graphic_Cursor_3();
	 ramAccessPrepare();
     for(i=0;i<256;i++)
     {					 
      lcdDataWrite(gImage_busy_im[i]);
     }
	// Stop cursor
     Select_Graphic_Cursor_4();
     ramAccessPrepare();
     for(i=0;i<256;i++)
     {					 
      lcdDataWrite(gImage_no_im[i]);
     }
	Memory_Select_SDRAM();// Reselect image SDRAM
    // Set default cursor foreground and outline colors
    Set_Graphic_Cursor_Color_1(0xff); // White foreground color
    Set_Graphic_Cursor_Color_2(0x00); // Background outline color
    Graphic_Cursor_XY(0,0);
	graphicMode(false);
}

//***************************************************//
/* Select Character Generator RAM                    */
//***************************************************//
void Ra8876_Lite::Memory_Select_CGRAM(void) // this may not be right
{
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_ICR); //0x03
//    temp &= cClrb1; // manual says this bit is clear
    temp |= cSetb0;
    temp &= cClrb0; // manual says this bit is set ?? Figure 14-10
	lcdRegDataWrite(RA8876_ICR, temp);
}

//***************************************************//
/* Select display buffer RAM                         */
//***************************************************//
void Ra8876_Lite::Memory_Select_SDRAM(void)
{
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_ICR); //0x03
    temp &= cClrb1;
    temp &= cClrb0;	// B
	lcdRegDataWrite(RA8876_ICR, temp);
}

//***************************************************//
/* Select graphic cursor RAM                         */
//***************************************************//
void Ra8876_Lite::Memory_Select_Graphic_Cursor_RAM(void)
{
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_ICR); //0x03
    temp |= cSetb1;
    temp &= cClrb0;
	lcdRegDataWrite(RA8876_ICR, temp);
}

//***************************************************//
/* Select RAM X-Y addressing mode                    */
//***************************************************//
void Ra8876_Lite::Memory_XY_Mode(void)	
{
/*
Canvas addressing mode
	0: Block mode (X-Y coordination addressing)
	1: linear mode
*/
	unsigned char temp;

	temp = lcdRegDataRead(RA8876_AW_COLOR);
	temp &= cClrb2;
	lcdRegDataWrite(RA8876_AW_COLOR, temp);
}

//***************************************************//
/* Select RAM linear addressing mode                 */
//***************************************************//
void Ra8876_Lite::Memory_Linear_Mode(void)	
{
/*
Canvas addressing mode
	0: Block mode (X-Y coordination addressing)
	1: linear mode
*/
	unsigned char temp;

	temp = lcdRegDataRead(RA8876_AW_COLOR);
	temp |= cSetb2;
	lcdRegDataWrite(RA8876_AW_COLOR, temp);
}

//***************************************************//
/* Enable Graphic Cursor                             */
//***************************************************//
void Ra8876_Lite::Enable_Graphic_Cursor(void)	
{
/*
Graphic Cursor Enable
	0 : Graphic Cursor disable.
	1 : Graphic Cursor enable.
*/
	unsigned char temp;
	
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp |= cSetb4;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//***************************************************//
/* Disable Graphic Cursor                            */
//***************************************************//
void Ra8876_Lite::Disable_Graphic_Cursor(void)	
{
/*
Graphic Cursor Enable
	0 : Graphic Cursor disable.
	1 : Graphic Cursor enable.
*/
	unsigned char temp;
	
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp &= cClrb4;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Select one of four available graphic cursors  */
//************************************************/
void Ra8876_Lite::Select_Graphic_Cursor_1(void)	
{
/*
Graphic Cursor Selection Bit
Select one from four graphic cursor types. (00b to 11b)
	00b : Graphic Cursor Set 1.
	01b : Graphic Cursor Set 2.
	10b : Graphic Cursor Set 3.
	11b : Graphic Cursor Set 4.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp &= cClrb3;
	temp &= cClrb2;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Select one of four available graphic cursors  */
//************************************************/
void Ra8876_Lite::Select_Graphic_Cursor_2(void)	
{
/*
Graphic Cursor Selection Bit
Select one from four graphic cursor types. (00b to 11b)
	00b : Graphic Cursor Set 1.
	01b : Graphic Cursor Set 2.
	10b : Graphic Cursor Set 3.
	11b : Graphic Cursor Set 4.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp &= cClrb3;
	temp |= cSetb2;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Select one of four available graphic cursors  */
//************************************************/
void Ra8876_Lite::Select_Graphic_Cursor_3(void)	
{
/*
Graphic Cursor Selection Bit
Select one from four graphic cursor types. (00b to 11b)
	00b : Graphic Cursor Set 1.
	01b : Graphic Cursor Set 2.
	10b : Graphic Cursor Set 3.
	11b : Graphic Cursor Set 4.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp |= cSetb3;
	temp &= cClrb2;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Select one of four available graphic cursors  */
//************************************************/
void Ra8876_Lite::Select_Graphic_Cursor_4(void)	
{
/*
Graphic Cursor Selection Bit
Select one from four graphic cursor types. (00b to 11b)
	00b : Graphic Cursor Set 1.
	01b : Graphic Cursor Set 2.
	10b : Graphic Cursor Set 3.
	11b : Graphic Cursor Set 4.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); //0x3c
	temp |= cSetb3;
	temp |= cSetb2;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Position graphic cursor                       */
//************************************************/
void Ra8876_Lite::Graphic_Cursor_XY(unsigned short WX,unsigned short HY)
{
/*
REG[40h] Graphic Cursor Horizontal Location[7:0]
REG[41h] Graphic Cursor Horizontal Location[12:8]
REG[42h] Graphic Cursor Vertical Location[7:0]
REG[43h] Graphic Cursor Vertical Location[12:8]
Reference main Window coordinates.
*/	
	lcdRegDataWrite(RA8876_GCHP0,WX);
	lcdRegDataWrite(RA8876_GCHP1,WX>>8);

	lcdRegDataWrite(RA8876_GCVP0,HY);
	lcdRegDataWrite(RA8876_GCVP1,HY>>8);
}

//************************************************/
/* Set graphic cursor foreground color           */
//************************************************/
void Ra8876_Lite::Set_Graphic_Cursor_Color_1(unsigned char temp)
{
/*
REG[44h] Graphic Cursor Color 0 with 256 Colors
RGB Format [7:0] = RRRGGGBB.
*/	
	lcdRegDataWrite(RA8876_GCC0,temp);
}

//************************************************/
/* Set graphic cursor outline color              */
//************************************************/
void Ra8876_Lite::Set_Graphic_Cursor_Color_2(unsigned char temp)
{
/*
REG[45h] Graphic Cursor Color 1 with 256 Colors
RGB Format [7:0] = RRRGGGBB.
*/	
	lcdRegDataWrite(RA8876_GCC1,temp);
}

//************************************************/
/* turn on text cursor                           */
//************************************************/
void Ra8876_Lite::Enable_Text_Cursor(void)	
{
/*
Text Cursor Enable
	0 : Disable.
	1 : Enable.
Text cursor & Graphic cursor cannot enable simultaneously.
Graphic cursor has higher priority then Text cursor if enabled simultaneously.
*/
	unsigned char temp;
	
	temp = lcdRegDataRead(RA8876_GTCCR); // 0x3c
	temp |= cSetb1;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* turn off text cursor                          */
//************************************************/
void Ra8876_Lite::Disable_Text_Cursor(void)	
{
/*
Text Cursor Enable
	0 : Disable.
	1 : Enable.
Text cursor & Graphic cursor cannot enable simultaneously.
Graphic cursor has higher priority then Text cursor if enabled simultaneously.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); // 0x3c
	temp &= cClrb1;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Turn on blinking text cursor                  */
//************************************************/
void Ra8876_Lite::Enable_Text_Cursor_Blinking(void)	
{
/*
Text Cursor Blinking Enable
	0 : Disable.
	1 : Enable.
*/
	unsigned char temp;
	
	temp = lcdRegDataRead(RA8876_GTCCR); // 0x3c
	temp |= cSetb0;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Turn off blinking text cursor                  */
//************************************************/
void Ra8876_Lite::Disable_Text_Cursor_Blinking(void)	
{
/*
Text Cursor Blinking Enable
	0 : Disable.
	1 : Enable.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_GTCCR); // 0x3c
	temp &= cClrb0;
	lcdRegDataWrite(RA8876_GTCCR,temp);
}

//************************************************/
/* Set text cursor blink rate                    */
//************************************************/
void Ra8876_Lite::Blinking_Time_Frames(unsigned char temp)	
{
/*
Text Cursor Blink Time Setting (Unit: Frame)
	00h : 1 frame time.
	01h : 2 frames time.
	02h : 3 frames time.
	:
	FFh : 256 frames time.
*/
	lcdRegDataWrite(RA8876_BTCR,temp); // 0x3d
}

//************************************************/
/* Set text cursor horizontal and vertical size  */
//************************************************/
void Ra8876_Lite::Text_Cursor_H_V(unsigned short WX,unsigned short HY)
{
/*
REG[3Eh]
Text Cursor Horizontal Size Setting[4:0]
Unit : Pixel
Zero-based number. Value 0 means 1 pixel.
Note : When font is enlarged, the cursor setting will multiply the
same times as the font enlargement.
REG[3Fh]
Text Cursor Vertical Size Setting[4:0]
Unit : Pixel
Zero-based number. Value 0 means 1 pixel.
Note : When font is enlarged, the cursor setting will multiply the
same times as the font enlargement.
*/
	lcdRegDataWrite(RA8876_CURHS,WX); // 0x3e
	lcdRegDataWrite(RA8876_CURVS,HY); // 0x3f
}

//************************************************/
/* Turn on color bar test screen                 */
//************************************************/
void Ra8876_Lite::Color_Bar_ON(void)
{
/*	
Display Test Color Bar
	0b: Disable.
	1b: Enable.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_DPCR); // 0x12
	temp |= cSetb5;
	lcdRegDataWrite(RA8876_DPCR,temp); // 0x12
}

//************************************************/
/* Turn off color bar test screen                */
//************************************************/
void Ra8876_Lite::Color_Bar_OFF(void)
{
/*	
Display Test Color Bar
	0b: Disable.
	1b: Enable.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_DPCR); // 0x12
	temp &= cClrb5;
	lcdRegDataWrite(RA8876_DPCR,temp); // 0x12
}

//************************************************/
/* Drawing Functions                             */
//************************************************/

//**************************************************************//
/* Draw a line                                                  */
/* x0,y0: Line start coords                                     */
/* x1,y1: Line end coords                                       */
//**************************************************************//
void Ra8876_Lite::drawLine(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh        
  lcdRegDataWrite(RA8876_DCR0,RA8876_DRAW_LINE);//67h,0x80
  check2dBusy();
  graphicMode(false);
}

/**************************************************************************
      For compatibility with popular Adafruit_GFX
	  draws a single vertical line
	  Parameters:
	  x: horizontal start
	  y: vertical start
	  h: height
	  color: RGB565 color

**************************************************************************/
void Ra8876_Lite::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	if (h < 1) h = 1;
	h < 2 ? putPixel_16bpp(x,y,color) : drawLine(x, y, x, (y+h)-1, color);
}

//**************************************************************//
// Draw a rectangle:
// x0,y0 is upper left start
// x1,y1 is lower right end corner
//**************************************************************//
void Ra8876_Lite::drawSquare(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh        
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_SQUARE);//76h,0xa0
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a filled rectangle:
// x0,y0 is upper left start
// x1,y1 is lower right end corner
//**************************************************************//
void Ra8876_Lite::drawSquareFill(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh        
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_SQUARE_FILL);//76h,0xE0
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a round corner rectangle:
// x0,y0 is upper left start
// x1,y1 is lower right end corner
// xr is the major radius of corner (horizontal)
// yr is the minor radius of corner (vertical)
//**************************************************************//
void Ra8876_Lite::drawCircleSquare(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 xr, ru16 yr, ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh    
  lcdRegDataWrite(RA8876_ELL_A0,xr);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,xr>>8);//79h 
  lcdRegDataWrite(RA8876_ELL_B0,yr);//7ah    
  lcdRegDataWrite(RA8876_ELL_B1,yr>>8);//7bh
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_CIRCLE_SQUARE);//76h,0xb0
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a filled round corner rectangle:
// x0,y0 is upper left start
// x1,y1 is lower right end corner
// xr is the major radius of corner (horizontal)
// yr is the minor radius of corner (vertical)
//**************************************************************//
void Ra8876_Lite::drawCircleSquareFill(ru16 x0, ru16 y0, ru16 x1, ru16 y1, ru16 xr, ru16 yr, ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh    
  lcdRegDataWrite(RA8876_ELL_A0,xr);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,xr>>8);//78h 
  lcdRegDataWrite(RA8876_ELL_B0,yr);//79h    
  lcdRegDataWrite(RA8876_ELL_B1,yr>>8);//7ah
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_CIRCLE_SQUARE_FILL);//76h,0xf0
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a triangle
// x0,y0 is triangle start
// x1,y1 is triangle second point
// x2,y2 is triangle end point
//**************************************************************//
void Ra8876_Lite::drawTriangle(ru16 x0,ru16 y0,ru16 x1,ru16 y1,ru16 x2,ru16 y2,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h point 0
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h point 0
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah point 0
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh point 0
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch point 1
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh point 1
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh point 1
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh point 1
  lcdRegDataWrite(RA8876_DTPH0,x2);//70h point 2
  lcdRegDataWrite(RA8876_DTPH1,x2>>8);//71h point 2
  lcdRegDataWrite(RA8876_DTPV0,y2);//72h point 2
  lcdRegDataWrite(RA8876_DTPV1,y2>>8);//73h  point 2
  lcdRegDataWrite(RA8876_DCR0,RA8876_DRAW_TRIANGLE);//67h,0x82
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a filled triangle
// x0,y0 is triangle start
// x1,y1 is triangle second point
// x2,y2 is triangle end point
//**************************************************************//
void Ra8876_Lite::drawTriangleFill(ru16 x0,ru16 y0,ru16 x1,ru16 y1,ru16 x2,ru16 y2,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DLHSR0,x0);//68h
  lcdRegDataWrite(RA8876_DLHSR1,x0>>8);//69h
  lcdRegDataWrite(RA8876_DLVSR0,y0);//6ah
  lcdRegDataWrite(RA8876_DLVSR1,y0>>8);//6bh
  lcdRegDataWrite(RA8876_DLHER0,x1);//6ch
  lcdRegDataWrite(RA8876_DLHER1,x1>>8);//6dh
  lcdRegDataWrite(RA8876_DLVER0,y1);//6eh
  lcdRegDataWrite(RA8876_DLVER1,y1>>8);//6fh  
  lcdRegDataWrite(RA8876_DTPH0,x2);//70h
  lcdRegDataWrite(RA8876_DTPH1,x2>>8);//71h
  lcdRegDataWrite(RA8876_DTPV0,y2);//72h
  lcdRegDataWrite(RA8876_DTPV1,y2>>8);//73h  
  lcdRegDataWrite(RA8876_DCR0,RA8876_DRAW_TRIANGLE_FILL);//67h,0xa2
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a circle
// x,y is center point of circle
// r is radius of circle
// See page 59 of RA8876.pdf for information on drawing arc's. (1 of 4 quadrants at a time only) 
//**************************************************************//
void Ra8876_Lite::drawCircle(ru16 x0,ru16 y0,ru16 r,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DEHR0,x0);//7bh
  lcdRegDataWrite(RA8876_DEHR1,x0>>8);//7ch
  lcdRegDataWrite(RA8876_DEVR0,y0);//7dh
  lcdRegDataWrite(RA8876_DEVR1,y0>>8);//7eh
  lcdRegDataWrite(RA8876_ELL_A0,r);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,r>>8);//78h 
  lcdRegDataWrite(RA8876_ELL_B0,r);//79h    
  lcdRegDataWrite(RA8876_ELL_B1,r>>8);//7ah
//  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_BOTTOM_LEFT_CURVE);//76h,0x90 (arc test)
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_CIRCLE);//76h,0x80
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw a filled circle
// x,y is center point of circle
// r is radius of circle
// See page 59 of RA8876.pdf for information on drawing arc's. (1 of 4 quadrants at a time only)
//**************************************************************//
void Ra8876_Lite::drawCircleFill(ru16 x0,ru16 y0,ru16 r,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DEHR0,x0);//7bh
  lcdRegDataWrite(RA8876_DEHR1,x0>>8);//7ch
  lcdRegDataWrite(RA8876_DEVR0,y0);//7dh
  lcdRegDataWrite(RA8876_DEVR1,y0>>8);//7eh
  lcdRegDataWrite(RA8876_ELL_A0,r);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,r>>8);//78h 
  lcdRegDataWrite(RA8876_ELL_B0,r);//79h    
  lcdRegDataWrite(RA8876_ELL_B1,r>>8);//7ah
//  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_BOTTOM_LEFT_CURVE_FILL);//76h,0xd0 (arc test)
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_CIRCLE_FILL);//76h,0xc0
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw an ellipse
// x0,y0 is ellipse start
// x1,y1 is ellipse x radius
// x2,y2 is ellipse y radius
//**************************************************************//
void Ra8876_Lite::drawEllipse(ru16 x0,ru16 y0,ru16 xr,ru16 yr,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DEHR0,x0);//7bh
  lcdRegDataWrite(RA8876_DEHR1,x0>>8);//7ch
  lcdRegDataWrite(RA8876_DEVR0,y0);//7dh
  lcdRegDataWrite(RA8876_DEVR1,y0>>8);//7eh
  lcdRegDataWrite(RA8876_ELL_A0,xr);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,xr>>8);//78h 
  lcdRegDataWrite(RA8876_ELL_B0,yr);//79h    
  lcdRegDataWrite(RA8876_ELL_B1,yr>>8);//7ah
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_ELLIPSE);//76h,0x80
  check2dBusy();
  graphicMode(false);
}

//**************************************************************//
// Draw an filled ellipse
// x0,y0 is ellipse start
// x1,y1 is ellipse x radius
// x2,y2 is ellipse y radius
//**************************************************************//
void Ra8876_Lite::drawEllipseFill(ru16 x0,ru16 y0,ru16 xr,ru16 yr,ru16 color)
{
  graphicMode(true);
  check2dBusy();
  foreGroundColor16bpp(color);
  lcdRegDataWrite(RA8876_DEHR0,x0);//7bh
  lcdRegDataWrite(RA8876_DEHR1,x0>>8);//7ch
  lcdRegDataWrite(RA8876_DEVR0,y0);//7dh
  lcdRegDataWrite(RA8876_DEVR1,y0>>8);//7eh
  lcdRegDataWrite(RA8876_ELL_A0,xr);//77h    
  lcdRegDataWrite(RA8876_ELL_A1,xr>>8);//78h 
  lcdRegDataWrite(RA8876_ELL_B0,yr);//79h    
  lcdRegDataWrite(RA8876_ELL_B1,yr>>8);//7ah
  lcdRegDataWrite(RA8876_DCR1,RA8876_DRAW_ELLIPSE_FILL);//76h,0xc0
  check2dBusy();
  graphicMode(false);
}

//*************************************************************//
// Scroll Screen up
//*************************************************************//
void Ra8876_Lite::scroll(void) { // No arguments for now
	bteMemoryCopy(currentPage,SCREEN_WIDTH, _scrollXL, _scrollYT+(_FNTheight*_scaleY),	//Source
				  currentPage,SCREEN_WIDTH, _scrollXL, pageOffset+_scrollYT,	//Desination
				  _scrollXR-_scrollXL, _scrollYB-_scrollYT-(_FNTheight*_scaleY)); //Copy Width, Height
	// Clear bottom text line
	drawSquareFill(_scrollXL, _scrollYB-(_FNTheight*_scaleY), _scrollXR-1, _scrollYB-1, _TXTBackColor);
	textColor(_TXTForeColor,_TXTBackColor);
}

//*************************************************************//
// Scroll Screen down
//*************************************************************//
void Ra8876_Lite::scroll_down(void) { // No arguments for now
	bteMemoryCopy(currentPage,SCREEN_WIDTH, _scrollXL, _scrollYT,	//Source
				  PAGE10_START_ADDR,SCREEN_WIDTH, _scrollXL, _scrollYT,	//Desination
				  _scrollXR-_scrollXL, (_scrollYB-_scrollYT)-(_FNTheight*_scaleY)); //Copy Width, Height
	bteMemoryCopy(PAGE10_START_ADDR,SCREEN_WIDTH, _scrollXL, _scrollYT,	//Source
				  currentPage,SCREEN_WIDTH, _scrollXL, _scrollYT+(_FNTheight*_scaleY),	//Desination
				  _scrollXR-_scrollXL, (_scrollYB-_scrollYT)-_FNTheight); //Copy Width, Height
	// Clear top text line
	drawSquareFill(_scrollXL, _scrollYT, _scrollXR-1,_scrollYT+(_FNTheight*_scaleY), _TXTBackColor);
	textColor(_TXTForeColor,_TXTBackColor);

}

//*************************************************************//
// Put a section of current screen page to another screen page.
// x0,y0 is upper left start coordinates of section
// x1,y1 is lower right coordinates of section
// dx0,dy0 is start coordinates of destination screen page
//*************************************************************//
uint32_t Ra8876_Lite::boxPut(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
							 uint16_t x1, uint16_t y1,
							 uint16_t dx0,uint16_t dy0) {

	bteMemoryCopy(currentPage, SCREEN_WIDTH, x0, y0,
    vPageAddr, SCREEN_WIDTH, dx0, dy0,
    x1-x0, y1-y0);
	return vPageAddr;
}

//*************************************************************//
// get a section of another screen page to current screen page.
// x0,y0 is upper left start coordinates of section
// x1,y1 is lower right coordinates of section
// dx0,dy0 is start coordinates of current screen page
//*************************************************************//
uint32_t Ra8876_Lite::boxGet(uint32_t vPageAddr, uint16_t x0, uint16_t y0,
							 uint16_t x1, uint16_t y1,
							 uint16_t dx0,uint16_t dy0) {

	bteMemoryCopy(vPageAddr, SCREEN_WIDTH, x0, y0,
	              currentPage, SCREEN_WIDTH, dx0, dy0,
	              x1-dx0, y1-dy0);
	return vPageAddr;
}

/*BTE function*/
//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMemoryCopy(ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
								ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,
								ru16 copy_width,ru16 copy_height)
{
  bte_Source0_MemoryStartAddr(s0_addr);
  bte_Source0_ImageWidth(s0_image_width);
  bte_Source0_WindowStartXY(s0_x,s0_y);

//  bte_Source1_MemoryStartAddr(PAGE3_START_ADDR);
//  bte_Source1_ImageWidth(des_image_width);
//  bte_Source1_WindowStartXY(des_x,des_y);

  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(copy_width,copy_height); 

  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_CODE_12<<4|RA8876_BTE_MEMORY_COPY_WITH_ROP);//91h
//  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_CODE_12<<4|3);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,(RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP) & 0x7f);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  check2dBusy();
} 

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMemoryCopyWithROP(
	ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
	ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,
    ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,
    ru16 copy_width,ru16 copy_height,ru8 rop_code)
{
  bte_Source0_MemoryStartAddr(s0_addr);
  bte_Source0_ImageWidth(s0_image_width);
  bte_Source0_WindowStartXY(s0_x,s0_y);
  bte_Source1_MemoryStartAddr(s1_addr);
  bte_Source1_ImageWidth(s1_image_width);
  bte_Source1_WindowStartXY(s1_x,s1_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(copy_width,copy_height);
  lcdRegDataWrite(RA8876_BTE_CTRL1,rop_code<<4|RA8876_BTE_MEMORY_COPY_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  check2dBusy();
} 
//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMemoryCopyWithChromaKey(ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 copy_width,ru16 copy_height, ru16 chromakey_color)
{
  bte_Source0_MemoryStartAddr(s0_addr);
  bte_Source0_ImageWidth(s0_image_width);
  bte_Source0_WindowStartXY(s0_x,s0_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(copy_width,copy_height);
  backGroundColor16bpp(chromakey_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_MEMORY_COPY_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  check2dBusy();
}
//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithROPData8(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,
ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code,const unsigned char *data)
{
   ru16 i,j;
  bte_Source1_MemoryStartAddr(s1_addr);
  bte_Source1_ImageWidth(s1_image_width);
  bte_Source1_WindowStartXY(s1_x,s1_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  lcdRegDataWrite(RA8876_BTE_CTRL1,rop_code<<4|RA8876_BTE_MPU_WRITE_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
  for(i=0;i< height;i++)
  {	
   for(j=0;j< (width*2);j++)
   {
    checkWriteFifoNotFull();
    lcdDataWrite(*data);
    data++;
    }
   }
  checkWriteFifoEmpty();
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithROPData16(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,
ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code,const unsigned short *data)
{
   ru16 i,j;
  bte_Source1_MemoryStartAddr(s1_addr);
  bte_Source1_ImageWidth(s1_image_width);
  bte_Source1_WindowStartXY(s1_x,s1_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  lcdRegDataWrite(RA8876_BTE_CTRL1,rop_code<<4|RA8876_BTE_MPU_WRITE_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
 for(j=0;j<height;j++)
 {
  for(i=0;i<width;i++)
  {
   checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
   lcdDataWrite16bbp(*data);
   data++;
   //checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
  }
 } 
  checkWriteFifoEmpty();
}
//**************************************************************//
//write data after setting
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithROP(ru32 s1_addr,ru16 s1_image_width,ru16 s1_x,ru16 s1_y,ru32 des_addr,ru16 des_image_width,ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru8 rop_code)
{
//   ru16 i,j;
  bte_Source1_MemoryStartAddr(s1_addr);
  bte_Source1_ImageWidth(s1_image_width);
  bte_Source1_WindowStartXY(s1_x,s1_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  lcdRegDataWrite(RA8876_BTE_CTRL1,rop_code<<4|RA8876_BTE_MPU_WRITE_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
}
//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithChromaKeyData8(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color,const unsigned char *data)
{
  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  backGroundColor16bpp(chromakey_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_MPU_WRITE_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
  for(i=0;i< height;i++)
  {	
   for(j=0;j< (width*2);j++)
   {
    checkWriteFifoNotFull();
    lcdDataWrite(*data);
    data++;
    }
   }
  checkWriteFifoEmpty();
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithChromaKeyData16(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height, ru16 chromakey_color,const unsigned short *data)
{
  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  backGroundColor16bpp(chromakey_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_MPU_WRITE_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
 for(j=0;j<height;j++)
 {
  for(i=0;i<width;i++)
  {
   checkWriteFifoNotFull();//if high speed mcu and without Xnwait check
   lcdDataWrite16bbp(*data);
   data++;
   //checkWriteFifoEmpty();//if high speed mcu and without Xnwait check
  }
 } 
  checkWriteFifoEmpty();
}

//**************************************************************//
//write data after setting
//**************************************************************//
void Ra8876_Lite::bteMpuWriteWithChromaKey(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color)
{
//  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  backGroundColor16bpp(chromakey_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_MPU_WRITE_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
}

//**************************************************************//
//**************************************************************//
void Ra8876_Lite::bteMpuWriteColorExpansionData(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color,const unsigned char *data)
{
  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  foreGroundColor16bpp(foreground_color);
  backGroundColor16bpp(background_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_BUS_WIDTH8<<4|RA8876_BTE_MPU_WRITE_COLOR_EXPANSION);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
  for(i=0;i< height;i++)
  {	
   for(j=0;j< (width/8);j++)
   {
    checkWriteFifoNotFull();
    lcdDataWrite(*data);
    data++;
    }
   }
  checkWriteFifoEmpty();
  check2dBusy();
}
//**************************************************************//
//write data after setting
//**************************************************************//
void Ra8876_Lite::bteMpuWriteColorExpansion(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color)
{
//  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height); 
  foreGroundColor16bpp(foreground_color);
  backGroundColor16bpp(background_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_BUS_WIDTH8<<4|RA8876_BTE_MPU_WRITE_COLOR_EXPANSION);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
}
//**************************************************************//
/*background_color do not set the same as foreground_color*/
//**************************************************************//
void Ra8876_Lite::bteMpuWriteColorExpansionWithChromaKeyData(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color, const unsigned char *data)
{
  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  foreGroundColor16bpp(foreground_color);
  backGroundColor16bpp(background_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_BUS_WIDTH8<<4|RA8876_BTE_MPU_WRITE_COLOR_EXPANSION_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
  
  for(i=0;i< height;i++)
  {	
   for(j=0;j< (width/8);j++)
   {
    checkWriteFifoNotFull();
    lcdDataWrite(*data);
    data++;
    }
   }
  checkWriteFifoEmpty();
  check2dBusy();
}
//**************************************************************//
/*background_color do not set the same as foreground_color*/
//write data after setting
//**************************************************************//
void Ra8876_Lite::bteMpuWriteColorExpansionWithChromaKey(ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 foreground_color,ru16 background_color)
{
//  ru16 i,j;
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  foreGroundColor16bpp(foreground_color);
  backGroundColor16bpp(background_color);
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_BUS_WIDTH8<<4|RA8876_BTE_MPU_WRITE_COLOR_EXPANSION_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4);//90h
  ramAccessPrepare();
}
//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::btePatternFill(ru8 p8x8or16x16, ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,
                                 ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height)
{ 
  bte_Source0_MemoryStartAddr(s0_addr);
  bte_Source0_ImageWidth(s0_image_width);
  bte_Source0_WindowStartXY(s0_x,s0_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height); 
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_CODE_12<<4|RA8876_BTE_PATTERN_FILL_WITH_ROP);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  
  if(p8x8or16x16 == 0)
   lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4|RA8876_PATTERN_FORMAT8X8);//90h
  else
   lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4|RA8876_PATTERN_FORMAT16X16);//90h
   
  check2dBusy();
}
//**************************************************************//
//**************************************************************//
void  Ra8876_Lite::btePatternFillWithChromaKey(ru8 p8x8or16x16, ru32 s0_addr,ru16 s0_image_width,ru16 s0_x,ru16 s0_y,ru32 des_addr,ru16 des_image_width, ru16 des_x,ru16 des_y,ru16 width,ru16 height,ru16 chromakey_color)
{
  bte_Source0_MemoryStartAddr(s0_addr);
  bte_Source0_ImageWidth(s0_image_width);
  bte_Source0_WindowStartXY(s0_x,s0_y);
  bte_DestinationMemoryStartAddr(des_addr);
  bte_DestinationImageWidth(des_image_width);
  bte_DestinationWindowStartXY(des_x,des_y);
  bte_WindowSize(width,height);
  backGroundColor16bpp(chromakey_color); 
  lcdRegDataWrite(RA8876_BTE_CTRL1,RA8876_BTE_ROP_CODE_12<<4|RA8876_BTE_PATTERN_FILL_WITH_CHROMA);//91h
  lcdRegDataWrite(RA8876_BTE_COLR,RA8876_S0_COLOR_DEPTH_16BPP<<5|RA8876_S1_COLOR_DEPTH_16BPP<<2|RA8876_DESTINATION_COLOR_DEPTH_16BPP);//92h
  if(p8x8or16x16 == 0)
   lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4|RA8876_PATTERN_FORMAT8X8);//90h
  else
   lcdRegDataWrite(RA8876_BTE_CTRL0,RA8876_BTE_ENABLE<<4|RA8876_PATTERN_FORMAT16X16);//90h
  check2dBusy();
}

 /*DMA Function*/
 //**************************************************************//
 /*If used 32bit address serial flash through ra8876, must be set command to serial flash to enter 4bytes mode first.
 only needs set one times after power on */
 //**************************************************************//
 void  Ra8876_Lite::setSerialFlash4BytesMode(ru8 scs_select)
 {
  if(scs_select==0)
  {
  lcdRegDataWrite( RA8876_SPIMCR2, RA8876_SPIM_NSS_SELECT_0<<5|RA8876_SPIM_MODE0);//b9h
  lcdRegDataWrite( RA8876_SPIMCR2, RA8876_SPIM_NSS_SELECT_0<<5|RA8876_SPIM_NSS_ACTIVE<<4|RA8876_SPIM_MODE0);//b9h 
  lcdRegWrite( RA8876_SPIDR);//b8h
  delay(1);
  lcdDataWrite(0xB7);//
  delay(1);
  lcdRegDataWrite( RA8876_SPIMCR2, RA8876_SPIM_NSS_SELECT_0<<5|RA8876_SPIM_NSS_INACTIVE<<4|RA8876_SPIM_MODE0);//b9h 
  }
  if(scs_select==1)
  {
  lcdRegDataWrite( RA8876_SPIMCR2 ,RA8876_SPIM_NSS_SELECT_1<<5|RA8876_SPIM_MODE0);//b9h
  lcdRegDataWrite( RA8876_SPIMCR2, RA8876_SPIM_NSS_SELECT_1<<5|RA8876_SPIM_NSS_ACTIVE<<4|RA8876_SPIM_MODE0);//b9h
  lcdRegWrite( RA8876_SPIDR);//b8h
  delay(1);
  lcdDataWrite(0xB7);//
  delay(1);
  lcdRegDataWrite( RA8876_SPIMCR2, RA8876_SPIM_NSS_SELECT_1<<5|RA8876_SPIM_NSS_INACTIVE<<4|RA8876_SPIM_MODE0);//b9h 
  } 
 }
//**************************************************************//
/* scs = 0 : select scs0, scs = 1 : select scs1, */
//**************************************************************//
 void  Ra8876_Lite::dma_24bitAddressBlockMode(ru8 scs_select,ru8 clk_div,ru16 x0,ru16 y0,ru16 width,ru16 height,ru16 picture_width,ru32 addr)
 {
   if(scs_select==0)
    lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT0<<7|RA8876_SERIAL_FLASH_DMA_MODE<<6|RA8876_SERIAL_FLASH_ADDR_24BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
   if(scs_select==1)
    lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT1<<7|RA8876_SERIAL_FLASH_DMA_MODE<<6|RA8876_SERIAL_FLASH_ADDR_24BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
  
  lcdRegDataWrite(RA8876_SPI_DIVSOR,clk_div);//bbh  
  lcdRegDataWrite(RA8876_DMA_DX0,x0);//c0h
  lcdRegDataWrite(RA8876_DMA_DX1,x0>>8);//c1h
  lcdRegDataWrite(RA8876_DMA_DY0,y0);//c2h
  lcdRegDataWrite(RA8876_DMA_DY1,y0>>8);//c3h 
  lcdRegDataWrite(RA8876_DMAW_WTH0,width);//c6h
  lcdRegDataWrite(RA8876_DMAW_WTH1,width>>8);//c7h
  lcdRegDataWrite(RA8876_DMAW_HIGH0,height);//c8h
  lcdRegDataWrite(RA8876_DMAW_HIGH1,height>>8);//c9h 
  lcdRegDataWrite(RA8876_DMA_SWTH0,picture_width);//cah
  lcdRegDataWrite(RA8876_DMA_SWTH1,picture_width>>8);//cbh 
  lcdRegDataWrite(RA8876_DMA_SSTR0,addr);//bch
  lcdRegDataWrite(RA8876_DMA_SSTR1,addr>>8);//bdh
  lcdRegDataWrite(RA8876_DMA_SSTR2,addr>>16);//beh
  lcdRegDataWrite(RA8876_DMA_SSTR3,addr>>24);//bfh 
  
  lcdRegDataWrite(RA8876_DMA_CTRL,RA8876_DMA_START);//b6h 
  check2dBusy(); 
 }
 //**************************************************************//
/* scs = 0 : select scs0, scs = 1 : select scs1, */
//**************************************************************//
 void  Ra8876_Lite::dma_32bitAddressBlockMode(ru8 scs_select,ru8 clk_div,ru16 x0,ru16 y0,ru16 width,ru16 height,ru16 picture_width,ru32 addr)
 {
   if(scs_select==0)
    lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT0<<7|RA8876_SERIAL_FLASH_DMA_MODE<<6|RA8876_SERIAL_FLASH_ADDR_32BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
   if(scs_select==1)
    lcdRegDataWrite(RA8876_SFL_CTRL,RA8876_SERIAL_FLASH_SELECT1<<7|RA8876_SERIAL_FLASH_DMA_MODE<<6|RA8876_SERIAL_FLASH_ADDR_32BIT<<5|RA8876_FOLLOW_RA8876_MODE<<4|RA8876_SPI_FAST_READ_8DUMMY);//b7h
  
  lcdRegDataWrite(RA8876_SPI_DIVSOR,clk_div);//bbh 
  
  lcdRegDataWrite(RA8876_DMA_DX0,x0);//c0h
  lcdRegDataWrite(RA8876_DMA_DX1,x0>>8);//c1h
  lcdRegDataWrite(RA8876_DMA_DY0,y0);//c2h
  lcdRegDataWrite(RA8876_DMA_DY1,y0>>8);//c3h 
  lcdRegDataWrite(RA8876_DMAW_WTH0,width);//c6h
  lcdRegDataWrite(RA8876_DMAW_WTH1,width>>8);//c7h
  lcdRegDataWrite(RA8876_DMAW_HIGH0,height);//c8h
  lcdRegDataWrite(RA8876_DMAW_HIGH1,height>>8);//c9h 
  lcdRegDataWrite(RA8876_DMA_SWTH0,picture_width);//cah
  lcdRegDataWrite(RA8876_DMA_SWTH1,picture_width>>8);//cbh 
  lcdRegDataWrite(RA8876_DMA_SSTR0,addr);//bch
  lcdRegDataWrite(RA8876_DMA_SSTR1,addr>>8);//bdh
  lcdRegDataWrite(RA8876_DMA_SSTR2,addr>>16);//beh
  lcdRegDataWrite(RA8876_DMA_SSTR3,addr>>24);//bfh  
  
  lcdRegDataWrite(RA8876_DMA_CTRL,RA8876_DMA_START);//b6h 
  check2dBusy(); 
 }
 
//**************************************************************//
// Setup PIP Windows ( 2 PIP Windows Avaiable)
//**************************************************************//
void Ra8876_Lite::PIP (
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
)
{
	if(Select_PIP == 1 )  
	{
	Select_PIP1_Parameter();
	}
	if(Select_PIP == 2 )  
	{
	Select_PIP2_Parameter();
	}
	PIP_Display_Start_XY(X_Dis,Y_Dis);
	PIP_Image_Start_Address(PAddr);
	PIP_Image_Width(ImageWidth);
	PIP_Window_Image_Start_XY(XP,YP);
	PIP_Window_Width_Height(X_W,Y_H);

	if(On_Off == 0)
    {
  		if(Select_PIP == 1 )  
		{ 
		Disable_PIP1();	
		}
		if(Select_PIP == 2 )  
		{
		Disable_PIP2();
		}
    }

    if(On_Off == 1)
    {
  		if(Select_PIP == 1 )  
		{ 
		Enable_PIP1();	
		}
		if(Select_PIP == 2 )  
		{
		Enable_PIP2();
		}
    }
}

//[2Ah][2Bh][2Ch][2Dh]=========================================================================
void Ra8876_Lite::PIP_Display_Start_XY(unsigned short WX,unsigned short HY)	
{
/*
Reference Main Window coordination.
Unit: Pixel
It must be divisible by 4. PWDULX Bit [1:0] tie to ¡§0¡š internally.
X-axis coordination should less than horizontal display width.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters. 
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWDULX0, WX);    // [2Ah] PIP Window Display Upper-Left corner X-coordination [7:0]
	lcdRegDataWrite(RA8876_PWDULX1, WX>>8); // [2Bh] PIP Window Display Upper-Left corner X-coordination [12:8]


/*
Reference Main Window coordination.
Unit: Pixel
Y-axis coordination should less than vertical display height.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters.
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWDULY0, HY);    // [2Ch] PIP Window Display Upper-Left corner Y-coordination [7:0]
	lcdRegDataWrite(RA8876_PWDULY1, HY>>8); // [2Dh] PIP Window Display Upper-Left corner Y-coordination [12:8]
}

//[2Eh][2Fh][30h][31h]=========================================================================
void Ra8876_Lite::PIP_Image_Start_Address(unsigned long Addr)	
{
	lcdRegDataWrite(RA8876_PISA0, Addr); // [2Eh] PIP Image Start Address[7:2]
	lcdRegDataWrite(RA8876_PISA1, Addr>>8); // [2Fh] PIP Image Start Address[15:8]
	lcdRegDataWrite(RA8876_PISA2, Addr>>16); // [30h] PIP Image Start Address [23:16]
	lcdRegDataWrite(RA8876_PISA3, Addr>>24); // [31h] PIP Image Start Address [31:24]
}

//[32h][33h]=========================================================================
void Ra8876_Lite::PIP_Image_Width(unsigned short WX)	
{
/*
Unit: Pixel.
It must be divisible by 4. PIW Bit [1:0] tie to ¡§0¡š internally.
The value is physical pixel number.
This width should less than horizontal display width.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters.
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PIW0, WX);     // [32h] PIP Image Width [7:0]
	lcdRegDataWrite(RA8876_PIW1, WX>>8);  // [33h] PIP Image Width [12:8]
}

//[34h][35h][36h][37h]=========================================================================
void Ra8876_Lite::PIP_Window_Image_Start_XY(unsigned short WX,unsigned short HY)	
{
/*
Reference PIP Image coordination.
Unit: Pixel
It must be divisible by 4. PWIULX Bit [1:0] tie to ¡§0¡š internally.
X-axis coordination plus PIP image width cannot large than 8188.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters. 
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWIULX0, WX);    // [34h] PIP 1 or 2 Window Image Upper-Left corner X-coordination [7:0]
	lcdRegDataWrite(RA8876_PWIULX1, WX>>8); // [35h] PIP Window Image Upper-Left corner X-coordination [12:8]

/*
Reference PIP Image coordination.
Unit: Pixel
Y-axis coordination plus PIP window height should less than 8191.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters. 
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWIULY0, HY);    // [36h] PIP Windows Display Upper-Left corner Y-coordination [7:0]
	lcdRegDataWrite(RA8876_PWIULY1, HY>>8); // [37h] PIP Windows Image Upper-Left corner Y-coordination [12:8]

}

//[38h][39h][3Ah][3Bh]=========================================================================
void Ra8876_Lite::PIP_Window_Width_Height(unsigned short WX,unsigned short HY)	
{
/*

Unit: Pixel.
It must be divisible by 4. The value is physical pixel number.
Maximum value is 8188 pixels.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters. 
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWW0, WX);    // [38h] PIP Window Width [7:0]
	lcdRegDataWrite(RA8876_PWW1, WX>>8); // [39h] PIP Window Width [10:8]

/*
Unit: Pixel
The value is physical pixel number. Maximum value is 8191 pixels.
According to bit of Select Configure PIP 1 or 2 Window¡Šs parameters. 
Function bit will be configured for relative PIP window.
*/
	lcdRegDataWrite(RA8876_PWH0, HY);    // [3Ah] PIP Window Height [7:0]
	lcdRegDataWrite(RA8876_PWH1, HY>>8); // [3Bh] PIP Window Height [10:8]

}

//[10h]=========================================================================
// Turn on PIP window 1
void Ra8876_Lite::Enable_PIP1(void)
{
/*
PIP 1 window Enable/Disable
0 : PIP 1 window disable.
1 : PIP 1 window enable
PIP 1 window always on top of PIP 2 window.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
	temp |= cSetb7;
	lcdRegDataWrite(RA8876_MPWCTR,temp);

}

// Turn off PIP window 1
void Ra8876_Lite::Disable_PIP1(void)
{
/*
PIP 1 window Enable/Disable
0 : PIP 1 window disable.
1 : PIP 1 window enable
PIP 1 window always on top of PIP 2 window.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb7;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

// Turn on PIP window 2
void Ra8876_Lite::Enable_PIP2(void)
{
/*
PIP 2 window Enable/Disable
0 : PIP 2 window disable.
1 : PIP 2 window enable
PIP 1 window always on top of PIP 2 window.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
	temp |= cSetb6;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

// Turn off PIP window 1
void Ra8876_Lite::Disable_PIP2(void)
{
/*
PIP 2 window Enable/Disable
0 : PIP 2 window disable.
1 : PIP 2 window enable
PIP 1 window always on top of PIP 2 window.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb6;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

void Ra8876_Lite::Select_PIP1_Parameter(void)
{
/*
0: To configure PIP 1¡Šs parameters.
1: To configure PIP 2¡Šs parameters..
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb4;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

void Ra8876_Lite::Select_PIP2_Parameter(void)
{
/*
0: To configure PIP 1¡Šs parameters.
1: To configure PIP 2¡Šs parameters..
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
	temp |= cSetb4;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}
void Ra8876_Lite::Select_Main_Window_8bpp(void)
{
/*
Main Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb3;
    temp &= cClrb2;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}
void Ra8876_Lite::Select_Main_Window_16bpp(void)
{
/*
Main Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb3;
    temp |= cSetb2;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}
void Ra8876_Lite::Select_Main_Window_24bpp(void)
{
/*
Main Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
	temp |= cSetb3;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

void Ra8876_Lite::Select_LCD_Sync_Mode(void)
{
/*
To Control panel's synchronous signals
0: Sync Mode: Enable XVSYNC, XHSYNC, XDE
1: DE Mode: Only XDE enable, XVSYNC & XHSYNC in idle state
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
    temp &= cClrb0;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}
void Ra8876_Lite::Select_LCD_DE_Mode(void)
{
/*
To Control panel's synchronous signals
0: Sync Mode: Enable XVSYNC, XHSYNC, XDE
1: DE Mode: Only XDE enable, XVSYNC & XHSYNC in idle state
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_MPWCTR); // 0x10
	temp |= cSetb0;
	lcdRegDataWrite(RA8876_MPWCTR,temp);
}

//[11h]=========================================================================
void Ra8876_Lite::Select_PIP1_Window_8bpp(void)
{
/*
PIP 1 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp &= cClrb3;
    temp &= cClrb2;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}

void Ra8876_Lite::Select_PIP1_Window_16bpp(void)
{
/*
PIP 1 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp &= cClrb3;
    temp |= cSetb2;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}

void Ra8876_Lite::Select_PIP1_Window_24bpp(void)
{
/*
PIP 1 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp |= cSetb3;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}

void Ra8876_Lite::Select_PIP2_Window_8bpp(void)
{
/*
PIP 2 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp &= cClrb1;
    temp &= cClrb0;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}

void Ra8876_Lite::Select_PIP2_Window_16bpp(void)
{
/*
PIP 2 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp &= cClrb1;
    temp |= cSetb0;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}

void Ra8876_Lite::Select_PIP2_Window_24bpp(void)
{
/*
PIP 2 Window Color Depth Setting
00b: 8-bpp generic TFT, i.e. 256 colors.
01b: 16-bpp generic TFT, i.e. 65K colors.
1xb: 24-bpp generic TFT, i.e. 1.67M colors.
*/
	unsigned char temp;
	temp = lcdRegDataRead(RA8876_PIPCDEP); // 0x11
    temp |= cSetb1;
//    temp &= cClrb0;
	lcdRegDataWrite(RA8876_PIPCDEP,temp);
}
