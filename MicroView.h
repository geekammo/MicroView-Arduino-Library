#ifndef MICROVIEW_H
#define MICROVIEW_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

// CS is defined when constructing MICROVIEW class, thus the library can control multiple slave MicroViews.
#define DC 8
#define SCK 13
#define MOSI 11
#define RESET 12

#define BLACK 0
#define WHITE 1

#define LCDWIDTH			64
#define LCDHEIGHT			48

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

class MICROVIEW {
	public:
		MICROVIEW(uint8_t CS) :cs(CS) {} 
		void begin(void);
		void command(uint8_t c);
		void data(uint8_t c);
		void clear(void);
		void invert(uint8_t i);
		void display(void);
		void drawPixel(uint8_t x, uint8_t y, uint8_t color);
		void drawBitmap(void);
		void setColumnAddress(uint8_t add);
		void setPageAddress(uint8_t add);

		void scrollRight(uint8_t start, uint8_t stop);
		void scrollLeft(uint8_t start, uint8_t stop);
		void scrollVertRight(uint8_t start, uint8_t stop);
		void scrollVertLeft(uint8_t start, uint8_t stop);
		void stopScroll(void);
  
	
	private:
		uint8_t cs;
		volatile uint8_t *mosiport, *sckport, *csport, *dcport;	// use volatile because these are fixed location port address
		uint8_t mosipinmask, sckpinmask, cspinmask, dcpinmask;
};
#endif