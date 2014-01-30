#ifndef MICROVIEW_H
#define MICROVIEW_H

#if ARDUINO >= 100
#include "Arduino.h"
#include "Print.h"
#else
#include "WProgram.h"
#endif

#define swap(a, b) { uint8_t t = a; a = b; b = t; }

#define DC		8
#define SCK		13
#define MOSI	11
#define RESET	12
#define CS 		10

#define BLACK 0
#define WHITE 1

#define LCDWIDTH			64
#define LCDHEIGHT			48
#define FONTHEADERSIZE		6

#define NORM				0
#define XOR					1

#define PAGE				0
#define ALL					1

#define SETCONTRAST 		0x81
#define DISPLAYALLONRESUME 	0xA4
#define DISPLAYALLON 		0xA5
#define NORMALDISPLAY 		0xA6
#define INVERTDISPLAY 		0xA7
#define DISPLAYOFF 			0xAE
#define DISPLAYON 			0xAF
#define SETDISPLAYOFFSET 	0xD3
#define SETCOMPINS 			0xDA
#define SETVCOMDESELECT		0xDB
#define SETDISPLAYCLOCKDIV 	0xD5
#define SETPRECHARGE 		0xD9
#define SETMULTIPLEX 		0xA8
#define SETLOWCOLUMN 		0x00
#define SETHIGHCOLUMN 		0x10
#define SETSTARTLINE 		0x40
#define MEMORYMODE 			0x20
#define COMSCANINC 			0xC0
#define COMSCANDEC 			0xC8
#define SEGREMAP 			0xA0
#define CHARGEPUMP 			0x8D
#define EXTERNALVCC 		0x01
#define SWITCHCAPVCC 		0x02

// Scroll
#define ACTIVATESCROLL 					0x2F
#define DEACTIVATESCROLL 				0x2E
#define SETVERTICALSCROLLAREA 			0xA3
#define RIGHTHORIZONTALSCROLL 			0x26
#define LEFT_HORIZONTALSCROLL 			0x27
#define VERTICALRIGHTHORIZONTALSCROLL	0x29
#define VERTICALLEFTHORIZONTALSCROLL	0x2A

class MICROVIEW : public Print{
public:
	MICROVIEW(void) {};
	void begin(void);

#if ARDUINO >= 100
	virtual size_t write(uint8_t);
#else
	virtual void   write(uint8_t);
#endif

	// RAW LCD functions
	void command(uint8_t c);
	void data(uint8_t c);
	void setColumnAddress(uint8_t add);
	void setPageAddress(uint8_t add);
	
	// LCD Draw functions	
	void clear(uint8_t mode);
	void clear(uint8_t mode, uint8_t c);
	void invert(uint8_t i);
	void display(void);
	void setCursor(uint8_t x, uint8_t y);
	void pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode);
	void line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode);
	void lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode);
	void lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode);
	void rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode);
	void circle(uint8_t x, uint8_t y, uint8_t radius, uint8_t color, uint8_t mode);
	void circleFill(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color, uint8_t mode);
	void drawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode);
	void drawBitmap(void);
	uint8_t getLCDWidth(void);
	uint8_t getLCDHeight(void);

	// Font functions
	uint8_t getFontWidth(void);
	uint8_t getFontHeight(void);
	uint8_t getTotalFonts(void);
	uint8_t getFontType(void);
	uint8_t setFontType(uint8_t type);
	uint8_t getFontStartChar(void);
	uint8_t getFontTotalChar(void);
	
	// applicable for PRINT function
	void setFontColor(uint8_t color);
	void setFontDrawMode(uint8_t mode);
	
	
	// LCD Rotate Scroll functions	
	void scrollRight(uint8_t start, uint8_t stop);
	void scrollLeft(uint8_t start, uint8_t stop);
	void scrollVertRight(uint8_t start, uint8_t stop);
	void scrollVertLeft(uint8_t start, uint8_t stop);
	void stopScroll(void);
	
private:
	//uint8_t cs;
	volatile uint8_t *mosiport, *sckport, *csport, *dcport;	// use volatile because these are fixed location port address
	uint8_t mosipinmask, sckpinmask, cspinmask, dcpinmask;
	uint8_t fontColor,fontMode,fontWidth, fontHeight, fontType, fontStartChar, fontTotalChar, cursorX, cursorY;
	uint16_t fontMapWidth;
	//unsigned char *fontsPointer[TOTALFONTS];
	static const unsigned char *fontsPointer[];
};
#endif