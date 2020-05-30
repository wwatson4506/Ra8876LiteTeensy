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
 

// Documentation on the ILI9488_t3 font data format:
// https://forum.pjrc.com/threads/54316-ILI9488_t-font-structure-format
/*
typedef struct {
	const unsigned char *index;
	const unsigned char *unicode;
	const unsigned char *data;
	unsigned char version;
	unsigned char reserved;
	unsigned char index1_first;
	unsigned char index1_last;
	unsigned char index2_first;
	unsigned char index2_last;
	unsigned char bits_index;
	unsigned char bits_width;
	unsigned char bits_height;
	unsigned char bits_xoffset;
	unsigned char bits_yoffset;
	unsigned char bits_delta;
	unsigned char line_space;
	unsigned char cap_height;
} ILI9341_t3_font_t;
*/
#include "ILI9341_fonts.h"

#if !defined(swapvals)
	#if defined(ESP8266)
		#define swapvals(a, b) { int16_t t = a; a = b; b = t; }
		//#define swapvals(a, b) { typeid(a) t = a; a = b; b = t; }
	#else
		#define swapvals(a, b) { typeof(a) t = a; a = b; b = t; }
	#endif
#endif

// Lets see about supporting Adafruit fonts as well?
#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/// Font data stored PER GLYPH
typedef struct {
	uint16_t bitmapOffset;     ///< Pointer into GFXfont->bitmap
	uint8_t  width;            ///< Bitmap dimensions in pixels
    uint8_t  height;           ///< Bitmap dimensions in pixels
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
	int8_t   xOffset;          ///< X dist from cursor pos to UL corner
    int8_t   yOffset;          ///< Y dist from cursor pos to UL corner
} GFXglyph;

/// Data stored for FONT AS A WHOLE
typedef struct { 
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
	uint8_t   first;       ///< ASCII extents (first char)
    uint8_t   last;        ///< ASCII extents (last char)
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_ 
 
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

/*--------------------------------------*/
/* [RENDER TEXT OPTIMIZATIONS] +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
From 0.70b11 the Font Rendering Engine has some optimizations for render font faster but this require much more code.
Not all users need this so you can select if include Render Text Optimizations or not by comment the following line. */
#define _RA8875_TXTRNDOPTIMIZER								// [default uncommented]
//#define RA8875_VISPIXDEBUG 								// [default commented]
#define FORCE_RA8875_TXTREND_FOLLOW_CURS 					// [default uncommented]

#ifndef bitRead
	#define bitRead(a,b) ((a) & (1<<(b)))
#endif
#ifndef bitWrite
	#define __bitSet(value, bit) ((value) |= (1UL << (bit)))
	#define __bitClear(value, bit) ((value) &= ~(1UL << (bit)))
	#define bitWrite(value, bit, bitvalue) (bitvalue ? __bitSet(value, bit) : __bitClear(value, bit))
#endif

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
	void putPicture(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const unsigned char *data);

	void scrollUp(void);
	
//Hack fonts
	// setOrigin sets an offset in display pixels where drawing to (0,0) will appear
	// for example: setOrigin(10,10); drawPixel(5,5); will cause a pixel to be drawn at hardware pixel (15,15)
	void setOrigin(int16_t x = 0, int16_t y = 0) { 
		_originx = x; _originy = y; 
		//if (Serial) Serial.printf("Set Origin %d %d\n", x, y);
		updateDisplayClip();
	}
	void getOrigin(int16_t* x, int16_t* y) { *x = _originx; *y = _originy; }

	// setClipRect() sets a clipping rectangle (relative to any set origin) for drawing to be limited to.
	// Drawing is also restricted to the bounds of the display

	void setClipRect(int16_t x1, int16_t y1, int16_t w, int16_t h) 
		{ _clipx1 = x1; _clipy1 = y1; _clipx2 = x1+w; _clipy2 = y1+h; 
			//if (Serial) Serial.printf("Set clip Rect %d %d %d %d\n", x1, y1, w, h);
			updateDisplayClip();
		}
	void setClipRect() {
			 _clipx1 = 0; _clipy1 = 0; _clipx2 = _width; _clipy2 = _height; 
			//if (Serial) Serial.printf("clear clip Rect\n");
			 updateDisplayClip(); 
		}
		
	bool _invisible = false; 
	bool _standard = true; // no bounding rectangle or origin set. 

	inline void updateDisplayClip() {
		_displayclipx1 = max(0,min(_clipx1+_originx,width()));
		_displayclipx2 = max(0,min(_clipx2+_originx,width()));

		_displayclipy1 = max(0,min(_clipy1+_originy,height()));
		_displayclipy2 = max(0,min(_clipy2+_originy,height()));
		_invisible = (_displayclipx1 == _displayclipx2 || _displayclipy1 == _displayclipy2);
		_standard =  (_displayclipx1 == 0) && (_displayclipx2 == _width) && (_displayclipy1 == 0) && (_displayclipy2 == _height);
		if (Serial) {
			//Serial.printf("UDC (%d %d)-(%d %d) %d %d\n", _displayclipx1, _displayclipy1, _displayclipx2, 
			//	_displayclipy2, _invisible, _standard);

		}
	}
	
	inline void setFontDefault() { 
		_use_default = 1; 
		//if(_portrait && (!_use_gfx_font || !_use_ili_font)) {
		//	_cursorX += _cursorY;
		//_cursorY -= _cursorX;
		//}
		_use_ili_font=0; 
		_use_gfx_font=0;
		_use_int_font=1;
		_use_tfont=0;
		setActiveWindow();
		_textPosition(_cursorX, _cursorY, false);
		};
	void setFontDef();
	void setFont(const ILI9341_t3_font_t &f);
    void setFont(const GFXfont *f = NULL);
	void setFontAdafruit(void) { setFont(); }
	void drawFontChar(unsigned int c);
	void drawGFXFontChar(unsigned int c);
	
    void setTextSize(uint8_t sx, uint8_t sy);
	void inline setTextSize(uint8_t s) { setTextSize(s,s); }
	
	void drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
	void inline drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size) 
	    { drawChar(x, y, c, color, bg, size);}
	void drawFontBits(bool opaque, uint32_t bits, uint32_t numbits, int32_t x, int32_t y, uint32_t repeat);
	void Pixel(int16_t x, int16_t y, uint16_t color);
	
	void charBounds(char c, int16_t *x, int16_t *y,
		int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
    void getTextBounds(const uint8_t *buffer, uint16_t len, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void getTextBounds(const char *string, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
    void getTextBounds(const String &str, int16_t x, int16_t y,
      int16_t *x1, int16_t *y1, uint16_t *w, uint16_t *h);
	int16_t strPixelLen(const char * str);

	void drawFontPixel( uint8_t alpha, uint32_t x, uint32_t y );

	void    	drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void    	drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void setCursor(int16_t x, int16_t y, bool autocenter);

	void getCursor(int16_t &x, int16_t &y); 
	int16_t getCursorX(void);
	int16_t getCursorY(void);
	
	// overwrite print functions:
	virtual size_t write(uint8_t);
	virtual size_t write(const uint8_t *buffer, size_t size);
	
	size_t rawPrint(uint8_t text);
	
using Print::write;
	
private:
 //HACK
	uint8_t  _use_ili_font = 0;
	uint8_t _use_gfx_font = 0;
	uint8_t _use_tfont = 0;
	uint8_t  _use_int_font = 0;
	uint8_t _use_default = 1;
	uint8_t textsize, textsize_x, textsize_y;
	uint16_t textcolor, textbgcolor; 
	//anti-alias font
	uint8_t fontbpp = 1;
	uint8_t fontbppindex = 0;
	uint8_t fontbppmask = 1;
	uint8_t fontppb = 8;
	uint8_t* fontalphalut;
	float fontalphamx = 1;
	
	bool							_backTransparent;

	
	//centering -------------------------------
	bool							_relativeCenter;
	bool							_absoluteCenter;
	bool							_portrait;
	int16_t  _activeWindowXL,_activeWindowXR,_activeWindowYT,_activeWindowYB;

	//text vars-------------------------------------------------		
	uint8_t							_FNTspacing;
	uint8_t							_FNTinterline;
	int							    _spaceCharWidth;
	//const tFont 			  	 *  _currentFont;
	
	uint32_t fetchbit(const uint8_t *p, uint32_t index);
	uint32_t fetchbits_unsigned(const uint8_t *p, uint32_t index, uint32_t required);
	uint32_t fetchbits_signed(const uint8_t *p, uint32_t index, uint32_t required);
	void 	 _fontWrite(const uint8_t* buffer, uint16_t len);
	
	/**
	 * Found in a pull request for the Adafruit framebuffer library. Clever!
	 * https://github.com/tricorderproject/arducordermini/pull/1/files#diff-d22a481ade4dbb4e41acc4d7c77f683d
	 * Converts  0000000000000000rrrrrggggggbbbbb
	 *     into  00000gggggg00000rrrrr000000bbbbb
	 * with mask 00000111111000001111100000011111
	 * This is useful because it makes space for a parallel fixed-point multiply
	 * This implements the linear interpolation formula: result = bg * (1.0 - alpha) + fg * alpha
	 * This can be factorized into: result = bg + (fg - bg) * alpha
	 * alpha is in Q1.5 format, so 0.0 is represented by 0, and 1.0 is represented by 32
	 * @param	fg		Color to draw in RGB565 (16bit)
	 * @param	bg		Color to draw over in RGB565 (16bit)
	 * @param	alpha	Alpha in range 0-255
	 **/
	uint16_t alphaBlendRGB565( uint32_t fg, uint32_t bg, uint8_t alpha )
	 __attribute__((always_inline)) {
	 	alpha = ( alpha + 4 ) >> 3; // from 0-255 to 0-31
		bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
		fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
		uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
		return (uint16_t)((result >> 16) | result); // contract result
	}
	
	/**
	 * Same as above, but fg and bg are premultiplied, and alpah is already in range 0-31
	 */
	uint16_t alphaBlendRGB565Premultiplied( uint32_t fg, uint32_t bg, uint8_t alpha )
	 __attribute__((always_inline)) {
		uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
		return (uint16_t)((result >> 16) | result); // contract result
	}
	
	uint32_t fetchpixel(const uint8_t *p, uint32_t index, uint32_t x);
	

	// Hack to see about combining outputs for speed.
	int16_t _combine_x_start = 0;
	int16_t _combine_y = 0;
	int16_t _combine_count = 0;
	uint16_t _combine_color = 0;

	inline void combineAndDrawPixel(int16_t x, int16_t y, uint16_t color) {
		if (_combine_count && (color == _combine_color)) _combine_count++;
		else {
			if (_combine_count)drawLine(_combine_x_start, _combine_y, _combine_x_start+_combine_count-1, _combine_y, _combine_color);
			_combine_x_start = x;
			_combine_y = y;
			_combine_count = 1;
			_combine_color = color;
		}
	}

	inline void forceCombinedPixelsOut() {
		if (_combine_count)fillRect(_combine_x_start, _combine_y, _combine_count, 1, _combine_color);
		_combine_count = 0;
	}
	
 protected:
 	uint32_t textcolorPrexpanded, textbgcolorPrexpanded;
	boolean wrap; // If set, 'wrap' text at right edge of display
	const ILI9341_t3_font_t *font;
	
	int16_t  _clipx1, _clipy1, _clipx2, _clipy2;
	int16_t  _originx, _originy;
	int16_t  _displayclipx1, _displayclipy1, _displayclipx2, _displayclipy2;
	
 	// GFX Font support
	const GFXfont *gfxFont = nullptr;
	int8_t _gfxFont_min_yOffset = 0;
	
	// Opaque font chracter overlap?
	unsigned int _gfx_c_last;
	int16_t   _gfx_last__cursorX, _gfx_last__cursorY;
	int16_t	 _gfx_last_char_x_write = 0;
	uint16_t _gfx_last_char_textcolor;
	uint16_t _gfx_last_char_textbgcolor;
	bool gfxFontLastCharPosFG(int16_t x, int16_t y);
 
	void setActiveWindow(int16_t XL,int16_t XR ,int16_t YT ,int16_t YB);
	void setActiveWindow(void);
	void _updateActiveWindow(bool full);
	
	void    	_textWrite(const char* buffer, uint16_t len=0);//thanks to Paul Stoffregen for the initial version of this one
	void 		_charWrite(const char c,uint8_t offset);
	void _charWriteR(const char c,uint8_t offset,uint16_t fcolor,uint16_t bcolor);

	int 		_getCharCode(uint8_t ch);
	#if defined(_RA8875_TXTRNDOPTIMIZER)
		void 		_drawChar_unc(int16_t x,int16_t y,int charW,int index,uint16_t fcolor);
	#else
		void 		_drawChar_unc(int16_t x,int16_t y,int16_t w,const uint8_t *data,uint16_t fcolor,uint16_t bcolor);
	#endif
	void _charLineRender(bool lineBuffer[],int charW,int16_t x,int16_t y,int16_t currentYposition,uint16_t fcolor);

	//void 		_drawChar_com(int16_t x,int16_t y,int16_t w,const uint8_t *data);
	void 		_textPosition(int16_t x, int16_t y,bool update);
	void 		_setFNTdimensions(uint8_t index);
	int16_t 	_STRlen_helper(const char* buffer,uint16_t len=0);
	uint8_t		 _FNTbaselineLow, 	  _FNTbaselineTop;

};
#endif

