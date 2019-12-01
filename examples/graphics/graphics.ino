// graphics.ino
#include "Arduino.h"
#include "Ra8876_Lite.h"
#include "tft.h"
#include "vt100.h"

// Array of Simple RA8876 Basic Colors
PROGMEM uint16_t myColors[] = {
	0x0000,
	0xffff,
	0xf800,
	0xfc10,
	0x8000,
	0x07e0,
	0x87f0,
	0x0400,
	0x001f,
	0x051f,
	0x841f,
	0x0010,
	0xffe0,
	0xfff0,
	0x8400,
	0x07ff,
	0x87ff,
	0x0410,
	0xf81f,
	0xfc1f,
	0x8010,
	0xA145
};

int interations = 0;

// Draw random unfilled rectangle boxes
void rectangles(uint16_t thickness) {
	uint16_t x0, y0, x1, y1, c;
	uint16_t j;

	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1,512);
		y0 = (uint16_t)random(1,288);
		x1 = (uint16_t)random(512,1023);
		y1 = (uint16_t)random(288,575);
		c = (uint16_t)random(21);
		if(x0 > getGwidth()) x0 = getGwidth();
		if(y0 > getGheight()) y0 = getGheight();
		if(x1 > getGwidth()) x1 = getGwidth();
		if(y1 > getGheight()) y1 = getGheight();
		if(thickness > 0) {
			for(j = 1; j <= thickness; j++) {
				drawRect(x0,y0,x1,y1,myColors[c]);
				if(x0 <= getGwidth())
					x0++;
				if(y0 <= getGheight())
					y0++;
				if(x1 > 0)
					x1--;
				if(y1 > 0)
					y1--;
			}
		} else {
			drawRect(x0,y0,x1,y1,myColors[c]);
		}
	}
	tft_slcls(myColors[11]);
}

// Draw random filled rectangle boxes
void filledRectangles(void) {
	uint16_t x0, y0, x1, y1, c;
	for(int i=0; i< interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		x1 = (uint16_t)random(1023);
		y1 = (uint16_t)random(575);
		c = (uint16_t)random(21);
		fillRect(x0,y0,x1,y1,myColors[c+1]);
	}
	tft_slcls(myColors[11]);
}

// Draw random round rectangle boxes
void rRectangles(uint16_t thickness) {
	uint16_t x0, y0, x1, y1, xr, yr, c;
	uint16_t j;

	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1,512);
		y0 = (uint16_t)random(1,288);
		x1 = (uint16_t)random(512,1023);
		y1 = (uint16_t)random(288,575);
		xr = 20; // Major Radius - Line Thickness must be less than
		yr = 20; // Minor Radius - Major and Minor radiuses by at
				 //				 - least half of xr and yr.
		c = (uint16_t)random(21);
		if(x0 > getGwidth()) x0 = getGwidth();
		if(y0 > getGheight()) y0 = getGheight();
		if(x1 > getGwidth()) x1 = getGwidth();
		if(y1 > getGheight()) y1 = getGheight();
		// Make sure major radius (xr) is less than x1 - x0
		// Must be xr * 2 + 1 less than x1 - x0
		// RA8876.pdf section 12.6 page 62
		if((xr * 2 + 1) >= (x1 - x0))
			xr = (x1 - x0) / 2 - 1;
		// Same for minor radius (yr)
		if((yr * 2 + 1) >= (y1 - y0))
			yr = (y1 - y0) / 2 - 1;
		if(thickness > 0) {
			for(j = 1; j <= thickness; j++) {
				drawRoundRect(x0,y0,x1,y1,xr,yr,myColors[c]);
				if(x0 <= getGwidth())
					x0++;
				if(y0 <= getGheight())
					y0++;
				if(x1 > 0)
					x1--;
				if(y1 > 0 )
					y1--;
				if(xr > 0)
					xr--;
				if(yr > 0)
					yr--;
			}
		} else {
			drawRoundRect(x0,y0,x1,y1,xr,yr,myColors[c]);
		}
	}
	tft_slcls(myColors[11]);
}

// Draw random filled round rectangle boxes
void filledRRectangles(void) {
	uint16_t x0, y0, x1, y1, xr, yr, c;

	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1,512);
		y0 = (uint16_t)random(1,288);
		x1 = (uint16_t)random(512,1023);
		y1 = (uint16_t)random(288,575);
		xr = 20; // Major Radius
		yr = 20; // Minor Radius
		c = (uint16_t)random(21);

		// Keep x,y within 1024x576 boundries
		if(x0 > getGwidth()) x0 = getGwidth();
		if(y0 > getGheight()) y0 = getGheight();
		if(x1 > getGwidth()) x1 = getGwidth();
		if(y1 > getGheight()) y1 = getGheight();
		
		// Make sure major radius (xr) is less than x1 - x0
		// Must be xr * 2 + 1 less than x1 - x0
		// RA8876.pdf section 12.6 page 62
		if((xr * 2 + 1) >= (x1 - x0))
			xr = (x1 - x0) / 2 - 1;
		// Same for minor radius (yr)
		if((yr * 2 + 1) >= (y1 - y0))
			yr = (y1 - y0) / 2 - 1;
		fillRoundRect(x0, y0, x1, y1, xr, yr, myColors[c]);
	}
	tft_slcls(myColors[11]);
}

// Draw random circles
void drawcircles(uint16_t thickness) {
	uint16_t x0, y0, r, c;
	int j;
	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		r = (uint16_t)random(239);
		c = (uint16_t)random(21);
		if(x0-r <= 0)
			x0 += (uint16_t)r;
		if(y0-r <= 0)
			y0 += (uint16_t)r;
		if(x0+r >=  getGwidth())
			x0 = (uint16_t)(getGwidth() - r);
		if(y0+r >= getGheight())
			y0 = (uint16_t)(getGheight() - r);
		if(thickness > 0) {
			for(j = 1; j <= thickness; j++) {
				drawCircle(x0, y0, r, myColors[c]);
				if(r > 0)
					r--;
			}
		} else {
		drawCircle(x0, y0, r, myColors[c]);
		}
		drawCircle(x0, y0, r, myColors[c]);
	}
	tft_slcls(myColors[11]);
}

// Draw random filled circles
void fillcircles(void) {
	uint16_t x0, y0, r, c;
	for(int i=0; i< interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		r = (uint16_t)random(239);
		c = (uint16_t)random(21);
		if(x0-r <= 0)
			x0 += (uint16_t)r;
		if(y0-r <= 0)
			y0 += (uint16_t)r;
		if(x0+r >=  getGwidth())
			x0 = (uint16_t)(getGwidth() - r);
		if(y0+r >= getGheight())
			y0 = (uint16_t)(getGheight() - r);
		fillCircle(x0, y0, r, myColors[c]);
	}
	tft_slcls(myColors[11]);
}

// Draw random unfilled tritangles
void drawTriangle(void) {
	uint16_t x0, y0, x1, y1, x2, y2, c;
	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		x1 = (uint16_t)random(1023);
		y1 = (uint16_t)random(575);
		x2 = (uint16_t)random(1023);
		y2 = (uint16_t)random(575);
		c = (uint16_t)random(21);
		drawTriangle(x0,y0,x1,y1,x2,y2,myColors[c+1]);
	}
	tft_slcls(myColors[11]);
}

// Draw random filled triangles
void fillTriangle(void) {
	uint16_t x0, y0, x1, y1, x2, y2, c;
	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		x1 = (uint16_t)random(1023);
		y1 = (uint16_t)random(575);
		x2 = (uint16_t)random(1023);
		y2 = (uint16_t)random(575);
		c = (uint16_t)random(21);
		fillTriangle(x0,y0,x1,y1,x2,y2,myColors[c+1]);
	}
	tft_slcls(myColors[11]);
}

// Draw random unfilled ellipses
void drawEllipse(void)
{
	int16_t  x0, y0, xr, yr;
	uint16_t color;
	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		xr = (uint16_t)random(239);
		yr = (uint16_t)random(239);
		color = (uint16_t)random(21);
		if(x0-xr <= 0)
			x0 += (uint16_t)xr;
		if(y0-yr <= 0)
			y0 += (uint16_t)yr;
		if(x0+xr >=  getGwidth())
			x0 = (uint16_t)(getGwidth() - xr);
		if(y0+yr >= getGheight())
			y0 = (uint16_t)(getGheight() - yr);
		drawEllipse(x0, y0, xr, yr, myColors[color]);
	}
	tft_slcls(myColors[11]);
}

// Draw random filled ellipses
void fillEllipse(void)
{
	int16_t  x0, y0, xr, yr;
	uint16_t color;
	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1023);
		y0 = (uint16_t)random(575);
		xr = (uint16_t)random(239);
		yr = (uint16_t)random(239);
		color = (uint16_t)random(21);
		if(x0-xr <= 0)
			x0 += (uint16_t)xr;
		if(y0-yr <= 0)
			y0 += (uint16_t)yr;
		if(x0+xr >=  getGwidth())
			x0 = (uint16_t)(getGwidth() - xr);
		if(y0+yr >= getGheight())
			y0 = (uint16_t)(getGheight() - yr);
		fillEllipse(x0, y0, xr, yr, myColors[color]);
	}
	tft_slcls(myColors[11]);
	//timeOn(); // Turn on time/date display on status line
}

// Draw random Lines
void drawlines(void) {
	uint16_t x0, y0, x1, y1, c;

	for(int i=0; i < interations; i++) {
		x0 = (uint16_t)random(1,1023);
		y0 = (uint16_t)random(1,575);
		x1 = (uint16_t)random(1,1023);
		y1 = (uint16_t)random(1,575);
		c = (uint16_t)random(21);
		if(x0 > getGwidth()) x0 = getGwidth();
		if(y0 > getGheight()) y0 = getGheight();
		if(x1 > getGwidth()) x1 = getGwidth();
		if(y1 > getGheight()) y1 = getGheight();
		drawLine(x0,y0,x1,y1,myColors[c]);
	}
	tft_slcls(myColors[11]);
}

int i = 0;
void setup() {
	tft_init();
	initVT100();
    setTextAt(0,0);
	tft_cls(myColors[11]);
	setFontSize(1,false);
}

void loop() {
	tft_slprint(0,myColors[1],myColors[11],"Rectangles");
	interations = 30000;
	rectangles(0);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Rectangles 10 pixel line thickness");
	interations = 4000;
	rectangles(10);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Filled Rectangles");
	interations = 4000;
	filledRectangles();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Round Rectangles");
	interations = 30000;
	rRectangles(0);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Round Rectangles 10 pixel line thickness");
	interations = 4000;
	rRectangles(10);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Filled Round Rectangles");
	interations = 4000;
	filledRRectangles();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Circles");
	interations = 30000;
	drawcircles(0);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Circles 10 pixel circle thickness");
	interations = 4000;
	drawcircles(10);
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Filled Circles");
	interations = 4000;
	fillcircles();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Triangles");
	interations = 30000;
	drawTriangle();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Filled Triangles");
	interations = 4000;
	fillTriangle();
	tft_cls(myColors[11]);
	delay(10);
	interations = 30000;
	tft_slprint(0,myColors[1],myColors[11],"Ellipses");
	drawEllipse();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Filled Ellipses");
	interations = 4000;
	fillEllipse();
	tft_cls(myColors[11]);
	delay(10);
	tft_slprint(0,myColors[1],myColors[11],"Lines");
	interations = 100000;
	drawlines();
	tft_cls(myColors[11]);
	delay(10);
}
	
