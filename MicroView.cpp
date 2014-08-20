/*
	MicroView Arduino Library
	Copyright (C) 2014 GeekAmmo

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <avr/pgmspace.h>
//#include <SPI.h>
#include <MicroView.h>

// This fixed ugly GCC warning "only initialized variables can be placed into program memory area"
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))

// Add header of the fonts here.  Remove as many as possible to conserve FLASH memory.
#include <font5x7.h>
#include <font8x16.h>
#include <fontlargenumber.h>
#include <7segment.h>
#include <space01.h>
#include <space02.h>
#include <space03.h>

// Change the total fonts included
#define TOTALFONTS		7
#define recvLEN			100
char serInStr[recvLEN];		// TODO - need to fix a value so that this will not take up too much memory.
uint8_t serCmd[recvLEN];

// Add the font name as declared in the header file.  Remove as many as possible to get conserve FLASH memory.
const unsigned char *MicroView::fontsPointer[]={
	font5x7
	,font8x16
	,sevensegment
	,fontlargenumber
	,space01
	,space02
	,space03
};

// TODO - Need to be able to let user add custom fonts from outside of the library
// TODO - getTotalFonts(), addFont() return font number, removeFont()

/** \brief MicroView screen buffer.

Page buffer 64 x 48 divided by 8 = 384 bytes
Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD controller's memory.
*/
static uint8_t screenmemory [] = { 
	// LCD Memory organised in 64 horizontal pixel and 6 rows of byte
	// B  B .............B  -----
	// y  y .............y       |
	// t  t .............t        |
	// e  e .............e         |
	// 0  1 .............63         |
	//                               |
	// D0 D0.............D0           | ROW 0
	// D1 D1.............D1           |
	// D2 D2.............D2          |
	// D3 D3.............D3         |
	// D4 D4.............D4        |
	// D5 D5.............D5       |
	// D6 D6.............D6      |
	// D7 D7.............D7  ----
	
	//SparkFun Electronics LOGO 
	
	// ROW0, BYTE0 to BYTE63
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x0F, 0x07, 0x07, 0x06, 0x06, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ROW1, BYTE64 to BYTE127
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x81, 0x07, 0x0F, 0x3F, 0x3F, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFC, 0xF8, 0xE0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ROW2, BYTE128 to BYTE191
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xE0, 0xE0, 0xE0, 0xE0, 0xE0, 0xF0, 0xFD, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW3, BYTE192 to BYTE255
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x07, 0x01,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

	// ROW4, BYTE256 to BYTE319
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	
	// ROW5, BYTE320 to BYTE383
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
	0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/** \brief Initialisation of MicroView Library.

	Setup IO pins for SPI port then send initialisation commands to the SSD1306 controller inside the OLED. 
*/
void MicroView::begin() {
	// default 5x7 font
	setFontType(0);
	setColor(WHITE);
	setDrawMode(NORM);
	setCursor(0,0);

	RESETLOW;
	
	// Enable 3.3V power to the display
	pinMode(OLEDPWR, OUTPUT);
	digitalWrite(OLEDPWR, HIGH);

	// Give some time for power to stabilise
	delay(10);

	RESETHIGH;
	delay(5);
	RESETLOW;

	// Setup SPI frequency
	MVSPI.setClockDivider(SPI_CLOCK_DIV2); 
	// Initialise SPI
	MVSPI.begin();

	delay(5);

	// bring out of reset
	RESETHIGH;

	delay(10);

	// Init sequence for 64x48 OLED module
	//	command(DISPLAYOFF);				// 0xAE
	//	command(SETDISPLAYCLOCKDIV, 0x80);	// 0xD5 / the suggested ratio 0x80
	command(SETMULTIPLEX, 0x2F);		// 0xA8
	//	command(SETDISPLAYOFFSET, 0x0);		// 0xD3 / no offset
	//	command(SETSTARTLINE | 0x0);		// 0x40 / line #0
	command(CHARGEPUMP, 0x14);			// 0x8D / enable charge pump
	//	command(NORMALDISPLAY);				// 0xA6
	//	command(DISPLAYALLONRESUME);		// 0xA4
	command(SEGREMAP | 0x1);
	command(COMSCANDEC);
	//	command(SETCOMPINS, 0x12);			// 0xDA
	command(SETCONTRAST, 0x8F);			// 0x81
	command(SETPRECHARGE, 0xF1);		// 0xD9
	command(SETVCOMDESELECT, 0x40);		// 0xDB
	command(DISPLAYON);					//--turn on oled panel

	clear(ALL);							// Erase hardware memory inside the OLED controller to avoid random data in memory.

	//	Serial.begin(115200);				// removed the Serial.begin() so that user can decide their own baud rate.
}

/** \brief Power off the OLED display.

	Reset display control signals and
	prepare the SSD1306 controller for power off,
	then power off the 3.3V regulator.
*/
void MicroView::end() {
	DCLOW;						// Just in case
	command(DISPLAYOFF);
	command(CHARGEPUMP, 0x10);	// Disable the charge pump
	delay(150);					// Wait for charge pump off
	RESETLOW;
	SSLOW;
	MVSPI.end();				// Disable SPI mode
	digitalWrite(OLEDPWR, LOW);	// Power off the 3.3V regulator
}

/** \brief Send 1 command byte.

	Send 1 command byte via SPI to SSD1306 controller.
*/
void MicroView::command(uint8_t c) {
	MVSPI.packetBegin();
	MVSPI.transfer(c);
	MVSPI.packetEnd();
}

/** \brief Send 2 command bytes.

	Send 2 command bytes via SPI to SSD1306 controller.
*/
void MicroView::command(uint8_t c1, uint8_t c2) {
	MVSPI.packetBegin();
	MVSPI.transfer(c1);
	MVSPI.wait();
	MVSPI.transfer(c2);
	MVSPI.packetEnd();
}

/** \brief Send 3 command bytes.

	Send 3 command bytes via SPI to SSD1306 controller.
*/
void MicroView::command(uint8_t c1, uint8_t c2, uint8_t c3) {
	MVSPI.packetBegin();
	MVSPI.transfer(c1);
	MVSPI.wait();
	MVSPI.transfer(c2);
	MVSPI.wait();
	MVSPI.transfer(c3);
	MVSPI.packetEnd();
}

/** \brief SPI data.

	Send 1 data byte via SPI to SSD1306 controller.
*/
void MicroView::data(uint8_t c) {
	MVSPI.packetBegin();
	DCHIGH;
	MVSPI.transfer(c);
	MVSPI.packetEnd();
}

/** \brief Set SSD1306 page address.

	Send page address command and address to the SSD1306 OLED controller.
*/
void MicroView::setPageAddress(uint8_t add) {
	command(SETPAGE|add);
	return;
}

/** \brief Set SSD1306 column address.

	Send column address command and address to the SSD1306 OLED controller.
*/
void MicroView::setColumnAddress(uint8_t add) {
	command((SETHIGHCOLUMN|(add>>4))+0x02, SETLOWCOLUMN|(0x0f&add));
	return;
}

/** \brief Clear screen buffer or SSD1306's memory.

	To clear GDRAM inside the LCD controller, pass in the variable mode = ALL and to clear screen page buffer pass in the variable mode = PAGE.
*/
void MicroView::clear(uint8_t mode) {
	if (mode==ALL) {
		command(SETADDRESSMODE, 0);		// Set horizontal addressing mode
		command(SETCOLUMNBOUNDS, 0, LCDTOTALWIDTH - 1);	// Set width
		command(SETPAGEBOUNDS, 0, LCDTOTALPAGES - 1);	// Set height

		MVSPI.packetBegin();
		DCHIGH;
		MVSPI.transfer(0);
		for (int i = 1; i < LCDTOTALWIDTH * LCDTOTALPAGES; i++) {
			MVSPI.wait();
			MVSPI.transfer(0);
		}
		MVSPI.packetEnd();

		command(SETADDRESSMODE, 2);	// Return display to page addressing mode
	}
	else
	{
		memset(screenmemory, 0, LCDWIDTH * LCDPAGES);
		//display();
	}
}

/** \brief Clear or replace screen buffer or SSD1306's memory with a character.	

	To clear GDRAM inside the LCD controller, pass in the variable mode = ALL with c character and to clear screen page buffer, pass in the variable mode = PAGE with c character.
*/
void MicroView::clear(uint8_t mode, uint8_t c) {
	if (mode==ALL) {
		command(SETADDRESSMODE, 0);		// Set horizontal addressing mode
		command(SETCOLUMNBOUNDS, 0, LCDTOTALWIDTH - 1);	// Set width
		command(SETPAGEBOUNDS, 0, LCDTOTALPAGES - 1);	// Set height

		MVSPI.packetBegin();
		DCHIGH;
		MVSPI.transfer(c);
		for (int i = 1; i < LCDTOTALWIDTH * LCDTOTALPAGES; i++) {
			MVSPI.wait();
			MVSPI.transfer(c);
		}
		MVSPI.packetEnd();

		command(SETADDRESSMODE, 2);	// Return display to page addressing mode
	}
	else
	{
		memset(screenmemory, c, LCDWIDTH * LCDPAGES);
		display();
	}	
}

/** \brief Invert display.

	The WHITE color of the display will turn to BLACK and the BLACK will turn to WHITE.
*/
void MicroView::invert(boolean inv) {
	if (inv)
	command(INVERTDISPLAY);
	else
	command(NORMALDISPLAY);
}

/** \brief Set contrast.

	OLED contrast value from 0 to 255. Note: Contrast level is not very obvious.
*/
void MicroView::contrast(uint8_t contrast) {
	command(SETCONTRAST, contrast);		// 0x81
}

/** \brief Transfer display memory.

	Bulk move the screen buffer to the SSD1306 controller's memory so that images/graphics drawn on the screen buffer will be displayed on the OLED.
*/
void MicroView::display(void) {
	command(SETADDRESSMODE, 0);		// Set horizontal addressing mode
	command(SETCOLUMNBOUNDS,
	LCDCOLUMNOFFSET,
	LCDCOLUMNOFFSET + LCDWIDTH - 1);	// Set width
	command(SETPAGEBOUNDS, 0, LCDPAGES - 1);	// Set height

	MVSPI.packetBegin();
	DCHIGH;
	MVSPI.transfer(screenmemory[0]);
	for (int i = 1; i < LCDWIDTH * LCDPAGES; i++) {
		MVSPI.wait();
		MVSPI.transfer(screenmemory[i]);
	}
	MVSPI.packetEnd();

	command(SETADDRESSMODE, 2);	// Restore to page addressing mode
}

//#if ARDUINO >= 100
/** \brief Override Arduino's Print.

	Arduino's print overridden so that we can use uView.print().
*/
size_t MicroView::write(uint8_t c) {

	//#else
	//	void MicroView::write(uint8_t c) {
	//#endif
	if (c == '\n') {
		cursorY += fontHeight;
		cursorX  = 0;
	} else if (c == '\r') {
		// skip 
	} else {
		drawChar(cursorX, cursorY, c, foreColor, drawMode);
		cursorX += fontWidth+1;
		if ((cursorX > (LCDWIDTH - fontWidth))) {
			cursorY += fontHeight;
			cursorX = 0;
		}
	}
	//#if ARDUINO >= 100
	return 1;
	//#endif
}

/** \brief Set cursor position.

	MicroView's cursor position to x,y.
*/
void MicroView::setCursor(uint8_t x, uint8_t y) {
	cursorX=x;
	cursorY=y;
}

/** \brief Draw pixel.

	Draw pixel using the current fore color and current draw mode in the screen buffer's x,y position.
*/
void MicroView::pixel(uint8_t x, uint8_t y) {
	pixel(x,y,foreColor,drawMode);
}

/** \brief Draw pixel with color and mode.

	Draw color pixel in the screen buffer's x,y position with NORM or XOR draw mode.
*/
void MicroView::pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode) {
	if ((x<0) ||  (x>=LCDWIDTH) || (y<0) || (y>=LCDHEIGHT))
	return;
	
	if (mode==XOR) {
		if (color==WHITE)
		screenmemory[x+ (y/8)*LCDWIDTH] ^= _BV((y%8));
	}
	else {
		if (color==WHITE)
		screenmemory[x+ (y/8)*LCDWIDTH] |= _BV((y%8));
		else
		screenmemory[x+ (y/8)*LCDWIDTH] &= ~_BV((y%8)); 
	}
	
	//display();
}

/** \brief Draw line.

	Draw line using current fore color and current draw mode from x0,y0 to x1,y1 of the screen buffer.
*/
void MicroView::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
	line(x0,y0,x1,y1,foreColor,drawMode);
}

/** \brief Draw line with color and mode.

	Draw line using color and mode from x0,y0 to x1,y1 of the screen buffer.
*/
void MicroView::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode) {
	uint8_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	uint8_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;}

	for (; x0<x1; x0++) {
		if (steep) {
			pixel(y0, x0, color, mode);
		} else {
			pixel(x0, y0, color, mode);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}	
}

/** \brief Draw horizontal line.

	Draw horizontal line using current fore color and current draw mode from x,y to x+width,y of the screen buffer.
*/
void MicroView::lineH(uint8_t x, uint8_t y, uint8_t width) {
	line(x,y,x+width,y,foreColor,drawMode);
}

/** \brief Draw horizontal line with color and mode.

	Draw horizontal line using color and mode from x,y to x+width,y of the screen buffer.
*/
void MicroView::lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode) {
	line(x,y,x+width,y,color,mode);
}

/** \brief Draw vertical line.

	Draw vertical line using current fore color and current draw mode from x,y to x,y+height of the screen buffer.
*/
void MicroView::lineV(uint8_t x, uint8_t y, uint8_t height) {
	line(x,y,x,y+height,foreColor,drawMode);
}

/** \brief Draw vertical line with color and mode.

	Draw vertical line using color and mode from x,y to x,y+height of the screen buffer.
*/
void MicroView::lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode) {
	line(x,y,x,y+height,color,mode);
}

/** \brief Draw rectangle.

	Draw rectangle using current fore color and current draw mode from x,y to x+width,y+height of the screen buffer.
*/
void MicroView::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	rect(x,y,width,height,foreColor,drawMode);
}

/** \brief Draw rectangle with color and mode.

	Draw rectangle using color and mode from x,y to x+width,y+height of the screen buffer.
*/
void MicroView::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
	uint8_t tempHeight;
	
	lineH(x,y, width, color, mode);
	lineH(x,y+height-1, width, color, mode);
	
	tempHeight=height-2;
	
	// skip drawing vertical lines to avoid overlapping of pixel that will 
	// affect XOR plot if no pixel in between horizontal lines		
	if (tempHeight<1) return;			

	lineV(x,y+1, tempHeight, color, mode);
	lineV(x+width-1, y+1, tempHeight, color, mode);
}

/** \brief Draw filled rectangle.

	Draw filled rectangle using current fore color and current draw mode from x,y to x+width,y+height of the screen buffer.
*/
void MicroView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
	rectFill(x,y,width,height,foreColor,drawMode);
}

/** \brief Draw filled rectangle with color and mode.

	Draw filled rectangle using color and mode from x,y to x+width,y+height of the screen buffer.
*/	
void MicroView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
	// TODO - need to optimise the memory map draw so that this function will not call pixel one by one
	for (int i=x; i<x+width;i++) {
		lineV(i,y, height, color, mode);
	}
}

/** \brief Draw circle.

	Draw circle with radius using current fore color and current draw mode at x,y of the screen buffer.
*/
void MicroView::circle(uint8_t x0, uint8_t y0, uint8_t radius) {
	circle(x0,y0,radius,foreColor,drawMode);
}

/** \brief Draw circle with color and mode.

	Draw circle with radius using color and mode at x,y of the screen buffer.
*/
void MicroView::circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode) {
	//TODO - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly 
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	pixel(x0, y0+radius, color, mode);
	pixel(x0, y0-radius, color, mode);
	pixel(x0+radius, y0, color, mode);
	pixel(x0-radius, y0, color, mode);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		pixel(x0 + x, y0 + y, color, mode);
		pixel(x0 - x, y0 + y, color, mode);
		pixel(x0 + x, y0 - y, color, mode);
		pixel(x0 - x, y0 - y, color, mode);
		
		pixel(x0 + y, y0 + x, color, mode);
		pixel(x0 - y, y0 + x, color, mode);
		pixel(x0 + y, y0 - x, color, mode);
		pixel(x0 - y, y0 - x, color, mode);
		
	}
}

/** \brief Draw filled circle.

	Draw filled circle with radius using current fore color and current draw mode at x,y of the screen buffer.
*/
void MicroView::circleFill(uint8_t x0, uint8_t y0, uint8_t radius) {
	circleFill(x0,y0,radius,foreColor,drawMode);
}

/** \brief Draw filled circle with color and mode.

	Draw filled circle with radius using color and mode at x,y of the screen buffer.
*/
void MicroView::circleFill(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode) {
	// TODO - - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly 
	int8_t f = 1 - radius;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * radius;
	int8_t x = 0;
	int8_t y = radius;

	// Temporary disable fill circle for XOR mode.
	if (mode==XOR) return;
	
	for (uint8_t i=y0-radius; i<=y0+radius; i++) {
		pixel(x0, i, color, mode);
	}

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		for (uint8_t i=y0-y; i<=y0+y; i++) {
			pixel(x0+x, i, color, mode);
			pixel(x0-x, i, color, mode);
		} 
		for (uint8_t i=y0-x; i<=y0+x; i++) {
			pixel(x0+y, i, color, mode);
			pixel(x0-y, i, color, mode);
		}    
	}
}

/** \brief Get LCD height.

	The height of the LCD return as byte.
*/
uint8_t MicroView::getLCDHeight(void) {
	return LCDHEIGHT;
}

/** \brief Get LCD width.

	The width of the LCD return as byte.
*/	
uint8_t MicroView::getLCDWidth(void) {
	return LCDWIDTH;
}

/** \brief Get font width.

	The cucrrent font's width return as byte.
*/	
uint8_t MicroView::getFontWidth(void) {
	return fontWidth;
}

/** \brief Get font height.

	The current font's height return as byte.
*/
uint8_t MicroView::getFontHeight(void) {
	return fontHeight;
}

/** \brief Get font starting character.

	Return the starting ASCII character of the currnet font, not all fonts start with ASCII character 0. Custom fonts can start from any ASCII character.
*/
uint8_t MicroView::getFontStartChar(void) {
	return fontStartChar;
}

/** \brief Get font total characters.

	Return the total characters of the current font.
*/
uint8_t MicroView::getFontTotalChar(void) {
	return fontTotalChar;
}

/** \brief Get total fonts.

	Return the total number of fonts loaded into the MicroView's flash memory.
*/
uint8_t MicroView::getTotalFonts(void) {
	return TOTALFONTS;
}

/** \brief Get font type.

	Return the font type number of the current font.
*/
uint8_t MicroView::getFontType(void) {
	return fontType;
}

/** \brief Set font type.

	Set the current font type number, ie changing to different fonts base on the type provided.
*/
uint8_t MicroView::setFontType(uint8_t type) {
	if ((type>=TOTALFONTS) || (type<0))
	return false;

	fontType=type;
	fontWidth=pgm_read_byte(fontsPointer[fontType]+0);
	fontHeight=pgm_read_byte(fontsPointer[fontType]+1);
	fontStartChar=pgm_read_byte(fontsPointer[fontType]+2);
	fontTotalChar=pgm_read_byte(fontsPointer[fontType]+3);
	fontMapWidth=(pgm_read_byte(fontsPointer[fontType]+4)*100)+pgm_read_byte(fontsPointer[fontType]+5); // two bytes values into integer 16
	return true;
}

/** \brief Set color.

	Set the current draw's color. Only WHITE and BLACK available.
*/
void MicroView::setColor(uint8_t color) {
	foreColor=color;
}

/** \brief Set draw mode.

	Set current draw mode with NORM or XOR.
*/
void MicroView::setDrawMode(uint8_t mode) {
	drawMode=mode;
}

/** \brief Draw character.

	Draw character c using current color and current draw mode at x,y.
*/
void  MicroView::drawChar(uint8_t x, uint8_t y, uint8_t c) {
	drawChar(x,y,c,foreColor,drawMode);
}

/** \brief Draw character with color and mode.

	Draw character c using color and draw mode at x,y.
*/
void  MicroView::drawChar(uint8_t x, uint8_t y, uint8_t c, uint8_t color, uint8_t mode) {
	// TODO - New routine to take font of any height, at the moment limited to font height in multiple of 8 pixels

	uint8_t rowsToDraw,row, tempC;
	uint8_t i,j,temp;
	uint16_t charPerBitmapRow,charColPositionOnBitmap,charRowPositionOnBitmap,charBitmapStartPosition;
	
	if ((c<fontStartChar) || (c>(fontStartChar+fontTotalChar-1)))		// no bitmap for the required c
	return;
	
	tempC=c-fontStartChar;

	// each row (in datasheet is call page) is 8 bits high, 16 bit high character will have 2 rows to be drawn
	rowsToDraw=fontHeight/8;	// 8 is LCD's page size, see SSD1306 datasheet
	if (rowsToDraw<=1) rowsToDraw=1;

	// the following draw function can draw anywhere on the screen, but SLOW pixel by pixel draw
	if (rowsToDraw==1) {
		for  (i=0;i<fontWidth+1;i++) {
			if (i==fontWidth) // this is done in a weird way because for 5x7 font, there is no margin, this code add a margin after col 5
			temp=0;
			else
			temp=pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(tempC*fontWidth)+i);
			
			for (j=0;j<8;j++) {			// 8 is the LCD's page height (see datasheet for explanation)
				if (temp & 0x1) {
					pixel(x+i, y+j, color,mode);
				}
				else {
					pixel(x+i, y+j, !color,mode);
				}
				
				temp >>=1;
			}
		}
		return;
	}

	// font height over 8 bit
	// take character "0" ASCII 48 as example
	charPerBitmapRow=fontMapWidth/fontWidth;  // 256/8 =32 char per row
	charColPositionOnBitmap=tempC % charPerBitmapRow;  // =16
	charRowPositionOnBitmap=int(tempC/charPerBitmapRow); // =1
	charBitmapStartPosition=(charRowPositionOnBitmap * fontMapWidth * (fontHeight/8)) + (charColPositionOnBitmap * fontWidth) ;

	// each row on LCD is 8 bit height (see datasheet for explanation)
	for(row=0;row<rowsToDraw;row++) {
		for (i=0; i<fontWidth;i++) {
			temp=pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(charBitmapStartPosition+i+(row*fontMapWidth)));
			for (j=0;j<8;j++) {			// 8 is the LCD's page height (see datasheet for explanation)
				if (temp & 0x1) {
					pixel(x+i,y+j+(row*8), color, mode);
				}
				else {
					pixel(x+i,y+j+(row*8), !color, mode);
				}
				temp >>=1;
			}
		}
	}

	/*
	fast direct memory draw but has a limitation to draw in ROWS
	// only 1 row to draw for font with 8 bit height
	if (rowsToDraw==1) {
		for (i=0; i<fontWidth; i++ ) {
			screenmemory[temp + (line*LCDWIDTH) ] = pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(c*fontWidth)+i);
			temp++;
		}
		return;
	}


	// font height over 8 bit
	// take character "0" ASCII 48 as example
	charPerBitmapRow=fontMapWidth/fontWidth;  // 256/8 =32 char per row
	charColPositionOnBitmap=c % charPerBitmapRow;  // =16
	charRowPositionOnBitmap=int(c/charPerBitmapRow); // =1
	charBitmapStartPosition=(fontMapWidth * (fontHeight/8)) + (charColPositionOnBitmap * fontWidth);
	
	temp=x;
	for (row=0; row<rowsToDraw; row++) {
		for (i=0; i<fontWidth; i++ ) {
			screenmemory[temp + (( (line*(fontHeight/8)) +row)*LCDWIDTH) ] = pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(charBitmapStartPosition+i+(row*fontMapWidth)));
			temp++;
		}
		temp=x;
	}
*/

}

/** \brief Stop scrolling.

	Stop the scrolling of graphics on the OLED.
*/
void MicroView::scrollStop(void){
	command(DEACTIVATESCROLL);
}

/** \brief Right scrolling.

	Set row start to row stop on the OLED to scroll right. Refer to http://learn.microview.io/intro/general-overview-of-microview.html for explanation of the rows.
*/
void MicroView::scrollRight(uint8_t start, uint8_t stop){
	if (stop<start)		// stop must be larger or equal to start
		return;
	scrollStop();		// need to disable scrolling before starting to avoid memory corrupt
	command(RIGHTHORIZONTALSCROLL, 0);
	command(start, 0x7, stop);	// scroll speed frames , TODO
	command(0x00, 0xFF, ACTIVATESCROLL);
}

/** \brief Left scrolling.

	Set row start to row stop on the OLED to scroll left. Refer to http://learn.microview.io/intro/general-overview-of-microview.html for explanation of the rows.
*/
void MicroView::scrollLeft(uint8_t start, uint8_t stop){
	if (stop<start)		// stop must be larger or equal to start
		return;
	scrollStop();		// need to disable scrolling before starting to avoid memory corrupt
	command(LEFTHORIZONTALSCROLL, 0);
	command(start, 0x7, stop);	// scroll speed frames , TODO
	command(0x00, 0xFF, ACTIVATESCROLL);
}

/** \brief Vertical flip.

	Flip the graphics on the OLED vertically.
*/
void MicroView::flipVertical(boolean flip) {
	if (flip) {
		command(COMSCANINC);
	}
	else {
		command(COMSCANDEC);
	}
}

/** \brief Horizontal flip.

	Flip the graphics on the OLED horizontally.
*/	
void MicroView::flipHorizontal(boolean flip) {
	if (flip) {
		command(SEGREMAP | 0x0);
	}
	else {
		command(SEGREMAP | 0x1);
	}
}

/** \brief Get pointer to screen buffer

	Return a pointer to the start of the RAM screen buffer for direct access.
*/
uint8_t *MicroView::getScreenBuffer(void) {
	return screenmemory;
}

/** \brief Parse command.

	Command stored in serCmd array will be parsed to performed draw functions.
*/
void MicroView::doCmd(uint8_t cmdCount) {
	// decode command
	switch (serCmd[0]) {
	case CMD_CLEAR: {
			if (cmdCount==1) {
				Serial.print("clear ");
				Serial.println(serCmd[1]);
				clear(serCmd[1]);
			} else if (cmdCount==2) {
				Serial.print("clear ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.println(serCmd[2]);
				clear(serCmd[1], serCmd[2]);
			}
			break;
		}
		
	case CMD_INVERT: {
			
			if (cmdCount==1) {
				Serial.print("invert ");
				Serial.println(serCmd[1]);
				invert(serCmd[1]);
			}
			break;
		}
		
	case CMD_CONTRAST: {
			
			if (cmdCount==1) {
				Serial.print("contrast ");
				Serial.println(serCmd[1]);
				contrast(serCmd[1]);
			}
			break;
		}
		
	case CMD_DISPLAY: {
			
			if (cmdCount==0) {
				Serial.println("display");
				display();
			}
			break;
		}
		
	case CMD_SETCURSOR: {
			
			if (cmdCount==2) {
				Serial.print("setCursor ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.println(serCmd[2]);
				setCursor(serCmd[1], serCmd[2]);
			}
			break;
		}
		
	case CMD_PIXEL: {
			
			if (cmdCount==2) {
				Serial.print("pixel ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.println(serCmd[2]);
				pixel(serCmd[1],serCmd[2]);
				display();
			} else if (cmdCount==4) {
				Serial.print("pixel ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.println(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.println(serCmd[4]);
				
				pixel(serCmd[1],serCmd[2],serCmd[3],serCmd[4]);
				display();
			}
			break;
		}
		
	case CMD_LINE: {
			if (cmdCount==4) {
				Serial.print("line ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.println(serCmd[4]);
				
				line(serCmd[1],serCmd[2],serCmd[3],serCmd[4]);
				display();
			} else if (cmdCount==6) {
				Serial.print("line ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.print(serCmd[5]);
				Serial.print(" ");
				Serial.println(serCmd[6]);
				
				line(serCmd[1],serCmd[2],serCmd[3],serCmd[4],serCmd[5],serCmd[6]);
				display();
			}
			break;
		}

	case CMD_LINEH: {
			if (cmdCount==3) {
				Serial.print("lineH ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.println(serCmd[3]);

				lineH(serCmd[1], serCmd[2], serCmd[3]);
				display();
			} else if (cmdCount==5) {
				Serial.print("lineH ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.println(serCmd[5]);

				lineH(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5]);
				display();
			}
			break;
		}
		
	case CMD_LINEV: {
			if (cmdCount==3) {
				Serial.print("lineH ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.println(serCmd[3]);

				lineV(serCmd[1], serCmd[2], serCmd[3]);
				display();
			} else if (cmdCount==5) {
				Serial.print("lineH ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.println(serCmd[5]);
				lineV(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5]);
				display();
			}
			break;
		}
		
	case CMD_RECT: {
			if (cmdCount==4) {
				Serial.print("rect ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.println(serCmd[4]);
				rect(serCmd[1], serCmd[2], serCmd[3], serCmd[4]);
				display();
			} else if (cmdCount==6) {
				Serial.print("rect ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.print(serCmd[5]);
				Serial.print(" ");
				Serial.println(serCmd[6]);
				rect(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5], serCmd[6]);
				display();
			}
			break;
		}

	case CMD_RECTFILL: {
			if (cmdCount==4) {
				Serial.print("rectFill ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.println(serCmd[4]);
				rectFill(serCmd[1], serCmd[2], serCmd[3], serCmd[4]);
				display();
			} else if (cmdCount==6) {
				Serial.print("rectFill ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.print(serCmd[5]);
				Serial.print(" ");
				Serial.println(serCmd[6]);
				rectFill(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5], serCmd[6]);
				display();
			}
			break;
			
		}
		
	case CMD_CIRCLE: {
			if (cmdCount==3) {
				Serial.print("circle ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.println(serCmd[3]);
				circle(serCmd[1], serCmd[2], serCmd[3]);
				display();
			} else if (cmdCount==5) {
				Serial.print("circle ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.println(serCmd[5]);
				circle(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5]);
				display();
			}
			break;
		}

	case CMD_CIRCLEFILL: {

			if (cmdCount==3) {
				Serial.print("circle ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.println(serCmd[3]);
				circleFill(serCmd[1], serCmd[2], serCmd[3]);
				display();
			} else if (cmdCount==5) {
				Serial.print("circle ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.println(serCmd[5]);
				circleFill(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5]);
				display();
			}
			break;
		}

	case CMD_DRAWCHAR: {
			if (cmdCount==3) {
				Serial.print("drawChar ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.println(serCmd[3]);
				drawChar(serCmd[1], serCmd[2], serCmd[3]);
				display();
			} else if (cmdCount==5) {
				Serial.print("drawChar ");
				Serial.print(serCmd[1]);
				Serial.print(" ");
				Serial.print(serCmd[2]);
				Serial.print(" ");
				Serial.print(serCmd[3]);
				Serial.print(" ");
				Serial.print(serCmd[4]);
				Serial.print(" ");
				Serial.println(serCmd[5]);
				drawChar(serCmd[1], serCmd[2], serCmd[3], serCmd[4], serCmd[5]);
				display();
			}
			break;
		}

	case CMD_DRAWBITMAP: {
			// TODO
			
			break;
		}
		
	case CMD_GETLCDWIDTH: {
			
			if (cmdCount==0) {
				Serial.print("LCDWidth=");
				Serial.println(getLCDWidth());
			}
			break;
		}
		
	case CMD_GETLCDHEIGHT: {
			if (cmdCount==0) {
				Serial.print("LCDHeight=");
				Serial.println(getLCDHeight());
			}
			break;
		}
		
	case CMD_SETCOLOR: {
			if (cmdCount==1) {
				Serial.print("setColor ");
				Serial.println(serCmd[1]);
				setColor(serCmd[1]);
			}
			break;
		}
		
	case CMD_SETDRAWMODE: {
			if (cmdCount==1) {
				Serial.print("drawMode ");
				Serial.println(serCmd[1]);
				setDrawMode(serCmd[1]);
			}
			break;
			
		}
	default:
		break;
	}
}

/** \brief Listen for serial command.

	Instruct the MicroView to check for serial command from the UART.
*/
void MicroView::checkComm(void) {
	int count = readSerial();
	char *result;
	uint8_t index=0;
	int temp;
	
	if (count>0) {
		// process Serial data
		result=strtok(serInStr,",");
		if (result !=NULL) {
			temp=atoi(result);
			serCmd[index]=(uint8_t)temp & 0xff;		// we only need 8 bit number
			index++;
			for (uint8_t i=0;i<recvLEN;i++) {
				result=strtok(NULL,",");
				if (result != NULL) {
					
					temp=atoi(result);
					serCmd[index]=(uint8_t)temp & 0xff;		// we only need 8 bit number
					index++;
				}
				else {
					break;
				}
			}
			/*
				// debug output
				Serial.print("command received=");
				Serial.println(index);
				for (uint8_t i=0;i<index;i++) {
					Serial.println(serCmd[i]);
				}
				*/
		}
		doCmd(index-1);	// index-1 is the total parameters count of a command
	}
}


/** \brief Read serial port.

	Check for data from the serial port (UART) and store to serInStr array.
*/
int MicroView::readSerial(void)
{
	int i=0;
	if(!Serial.available())
	return -1;

	while (Serial.available()>0)
	{
		if( i < recvLEN)
		{
			serInStr[i++] = Serial.read();
			delay(2);
		}
		else
		break;	
	}
	serInStr[i]='\0';
	return i;
}

// -------------------------------------------------------------------------------------
// MicroViewWidget Class - start
// -------------------------------------------------------------------------------------

/** \brief MicroView widget parent class.

	The MicroViewWidget class is the parent class for child widget like MicroViewSlider and MicroViewGauge.
*/
MicroViewWidget::MicroViewWidget(uint8_t newx, uint8_t newy, int16_t min, int16_t max) {
	setX(newx);
	setY(newy);
	value=0;
	setMinValue(min);
	setMaxValue(max);
}

/** \brief Get widget x position. */
uint8_t MicroViewWidget::getX() { return x; }

/** \brief Get widget y position. */
uint8_t MicroViewWidget::getY() { return y; }

/** \brief Set widget x position. */
void MicroViewWidget::setX(uint8_t newx) { x = newx; }

/** \brief Set widget y position. */
void MicroViewWidget::setY(uint8_t newy) { y = newy; }

/** \brief Get minimum value.

	Return the minimum value of the widget.
*/
int16_t MicroViewWidget::getMinValue() { return minValue; }

/** \brief Get maximum value.

	Return the maximum value of the widget.
*/
int16_t MicroViewWidget::getMaxValue() { return maxValue; }

/** \brief Get current value.

	Return the current value of the widget.
*/
int16_t MicroViewWidget::getValue() { return value; }

/** \brief Set minimum value.

	The minimum value of the widget is set to the variable passed in.
*/
void MicroViewWidget::setMinValue(int16_t min) { minValue=min; }

/** \brief Set maximum value.

	The maximum value of the widget is set to the variable passed in.
*/
void MicroViewWidget::setMaxValue(int16_t max) { maxValue=max; }

/** \brief Get the maximum possible print lenth of the value

	Return the maximum number of characters that would be printed using printf("%d", value) for the current value range.
*/
uint8_t MicroViewWidget::getMaxValLen() {
	uint8_t minLen, maxLen;

	maxLen = getInt16PrintLen(maxValue);
	minLen = getInt16PrintLen(minValue);
	return maxLen >= minLen ? maxLen : minLen;
}

/** \brief Set current value.

	The current value of the widget is set to the variable passed in.
*/
void MicroViewWidget::setValue(int16_t val) { 
	if ((val<=maxValue) && (val>=minValue)){ 
		value=val; 
		this->draw();
	}
}

/** \brief MicroView Widget reDraw routine.
	
	Redraws the widget.
*/
void MicroViewWidget::reDraw() {
	needFirstDraw=true;
	this->drawFace();
	this->draw();
}
	
// -------------------------------------------------------------------------------------
// MicroViewWidget Class - end
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Slider Widget - start
// -------------------------------------------------------------------------------------

/** \brief MicroViewSlider class initialisation. 

	Initialise the MicroViewSlider widget with default style.
*/
MicroViewSlider::MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max):MicroViewWidget(newx, newy, min, max) {
	style=0;
	totalTicks=30;
	prevValue=getMinValue();
	needFirstDraw=true;
	drawFace();
	draw();
}

/** \brief MicroViewSlider class initialisation with style. 

	Initialise the MicroViewSlider widget with style WIDGETSTYLE0 or WIDGETSTYLE1 or WIDGETSTYLE2 (like 0, but vertical) or WIDGETSTYLE3 (like 1, but vertical). If this list gets any longer, it might be better as a switch/case statement.
*/
MicroViewSlider::MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty):MicroViewWidget(newx, newy, min, max) {
	if (sty==WIDGETSTYLE1) {
		style=1;
		totalTicks=60;
	}
	else if (sty==WIDGETSTYLE2) {
		style=2;
		totalTicks=20;
	}
	else if (sty==WIDGETSTYLE3) {
		style=3;
		totalTicks=40;
	}
	else {		// Use style 0 if specified or invalid
		style=0;
		totalTicks=30;
	}

	prevValue=getMinValue();
	needFirstDraw=true;
	drawFace();
	draw();
}

/** \brief Draw widget face.

	Draw image/diagram representing the widget's face.
*/
void MicroViewSlider::drawFace() {
	uint8_t offsetX, offsetY, endOffset;
	offsetX=getX();
	offsetY=getY();

	//Horizontal styles, style 0 or 1
	if (style==0 || style==1) {
		endOffset = offsetX + totalTicks + 2;
		// Draw minor ticks
		for (uint8_t i=offsetX+1; i<endOffset; i+=2) {
			uView.lineV(i, offsetY+5, 3);
		}
		// Draw extensions for major ticks
		for (uint8_t i=offsetX+1; i<endOffset; i+=10) {
			uView.lineV(i, offsetY+3, 2);
		}
	}
	//Vertical styles, style 2 or 3
	else {
		endOffset = offsetY + totalTicks + 2;
		// Draw minor ticks
		for (uint8_t i=offsetY+1; i<=endOffset; i+=2) {
			uView.lineH(offsetX, i, 3);
		}
		// Draw extensions for major ticks
		for (uint8_t i=offsetY+1; i<=endOffset; i+=10) {
			uView.lineH(offsetX+3, i, 2);
		}
	}
}

/** \brief Draw widget value.

	Convert the current value of the widget and draw the ticker representing the value.
*/
void MicroViewSlider::draw() {
	uint8_t offsetX, offsetY;
	uint8_t tickPosition=0;
	char strBuffer[7];
	char formatStr[] = "%1d";

	formatStr[1] = '0' + getMaxValLen();	// Set the field width for the value range

	offsetX=getX();
	offsetY=getY();

	if (needFirstDraw) {
		if (style==0 || style==1){		//Horizontal
			tickPosition = ((float)(uint16_t)(prevValue-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
		}
		else {					//Vertical
			tickPosition = ((float)(uint16_t)(getMaxValue()-prevValue)/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineV(offsetX+7, offsetY+tickPosition, 3, WHITE, XOR);
			uView.pixel(offsetX+6, offsetY+1+tickPosition, WHITE, XOR);
		}

		sprintf(strBuffer, formatStr, prevValue);	// print with fixed width so that blank space will cover larger value
		needFirstDraw=false;
	}
	else {
		// Draw previous pointer in XOR mode to erase it
		if (style==0 || style==1){		//Horizontal
			tickPosition = ((float)(uint16_t)(prevValue-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
		}
		else {					//Vertical
			tickPosition = ((float)(uint16_t)(getMaxValue()-prevValue)/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineV(offsetX+7, offsetY+tickPosition, 3, WHITE, XOR);
			uView.pixel(offsetX+6, offsetY+1+tickPosition, WHITE, XOR);
		}

		// Draw current pointer
		if (style==0 || style==1){		//Horizontal
			tickPosition = ((float)(uint16_t)(getValue()-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
		}
		else {					//Vertical
			tickPosition = ((float)(uint16_t)(getMaxValue()-getValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*totalTicks;
			uView.lineV(offsetX+7, offsetY+tickPosition, 3, WHITE, XOR);
			uView.pixel(offsetX+6, offsetY+1+tickPosition, WHITE, XOR);
		}

		sprintf(strBuffer, formatStr, getValue());	// print with fixed width so that blank space will cover larger value
		prevValue=getValue();
	}

	// Draw value
	if (style==0)
		uView.setCursor(offsetX+totalTicks+4, offsetY+1);
	else if (style==1)
		uView.setCursor(offsetX, offsetY+10);
	else if (style==2)
		uView.setCursor(offsetX+1, offsetY+totalTicks+4);
	else //style==3
		uView.setCursor(offsetX+9, offsetY);

	uView.print(strBuffer);
}

// -------------------------------------------------------------------------------------
// Slider Widget - end
// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Gauge Widget - start
// -------------------------------------------------------------------------------------

/** \brief MicroViewGauge class initilisation.

	Initialise the MicroViewGauge widget with default style.
*/
MicroViewGauge::MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max):MicroViewWidget(newx, newy, min, max) {
	style=0;
	radius=15;
	prevValue=getMinValue();
	needFirstDraw=true;
	drawFace();
	draw();
}

/** \brief MicroViewGauge class initialisation with style.

	Initialise the MicroViewGauge widget with style WIDGETSTYLE0 or WIDGETSTYLE1.
*/
MicroViewGauge::MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty):MicroViewWidget(newx, newy, min, max) {
	if (sty==WIDGETSTYLE0) {
		style=0;
		radius=15;
	}
	else {
		style=1;
		radius=23;
	}
	prevValue=getMinValue();
	needFirstDraw=true;
	drawFace();
	draw();
}

/** \brief Draw widget face.

	Draw image/diagram representing the widget's face.
*/
void MicroViewGauge::drawFace() {
	uint8_t offsetX, offsetY;
	float degreeSec, fromSecX, fromSecY, toSecX, toSecY;
	offsetX=getX();
	offsetY=getY();

	uView.circle(offsetX,offsetY,radius);
	
	for (int i=150;i<=390;i+=30) {	// Major tick from 150 degree to 390 degree
		degreeSec=i*(PI/180);
		fromSecX = cos(degreeSec) * (radius / 1.5);
		fromSecY = sin(degreeSec) * (radius / 1.5);
		toSecX = cos(degreeSec) * (radius / 1);
		toSecY = sin(degreeSec) * (radius / 1);
		uView.line(1+offsetX+fromSecX,1+offsetY+fromSecY,1+offsetX+toSecX,1+offsetY+toSecY);
	}
	
	if(radius>15) {
		for (int i=150;i<=390;i+=15) {	// Minor tick from 150 degree to 390 degree
			degreeSec=i*(PI/180);
			fromSecX = cos(degreeSec) * (radius / 1.3);
			fromSecY = sin(degreeSec) * (radius / 1.3);
			toSecX = cos(degreeSec) * (radius / 1);
			toSecY = sin(degreeSec) * (radius / 1);
			uView.line(1+offsetX+fromSecX,1+offsetY+fromSecY,1+offsetX+toSecX,1+offsetY+toSecY);
		}
	}
}

/** \brief Draw widget value.

	Convert the current value of the widget and draw the ticker representing the value.
*/
void MicroViewGauge::draw() {
	uint8_t offsetX, offsetY;
	uint8_t maxValLen, valOffset;
	float degreeSec, toSecX, toSecY;
	char strBuffer[7];
	char formatStr[] = "%1d"; // The 1 will be replaced later by the proper length

	maxValLen = getMaxValLen();
	formatStr[1] = '0' + maxValLen;	// Set the field width for the value range
	valOffset = maxValLen * 3 - 1;	// Offset left of centre to print the value

	offsetX=getX();
	offsetY=getY();

	if (needFirstDraw) {
		degreeSec = ((float)(uint16_t)(prevValue-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*240;	// total 240 degree in the widget
		degreeSec = (degreeSec+150) * (PI/180);		// 150 degree starting point
		toSecX = cos(degreeSec) * (radius / 1.2);
		toSecY = sin(degreeSec) * (radius / 1.2);
		uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);
		sprintf(strBuffer, formatStr, prevValue);	// print with fixed width so that blank space will cover larger value
		needFirstDraw=false;
	}
	else {
		// Draw previous pointer in XOR mode to erase it
		degreeSec = ((float)(uint16_t)(prevValue-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*240;	// total 240 degree in the widget
		degreeSec = (degreeSec+150) * (PI/180);
		toSecX = cos(degreeSec) * (radius / 1.2);
		toSecY = sin(degreeSec) * (radius / 1.2);
		uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);
		
		// draw current pointer
		degreeSec = ((float)(uint16_t)(getValue()-getMinValue())/(float)(uint16_t)(getMaxValue()-getMinValue()))*240;	// total 240 degree in the widget
		degreeSec = (degreeSec+150) * (PI/180);		// 150 degree starting point
		toSecX = cos(degreeSec) * (radius / 1.2);
		toSecY = sin(degreeSec) * (radius / 1.2);
		uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);

		sprintf(strBuffer, formatStr, getValue());	// print with fixed width so that blank space will cover larger value
		prevValue=getValue();
	}

	// Draw value
	if (style>0) 
	uView.setCursor(offsetX-valOffset, offsetY+10);
	else
	uView.setCursor(offsetX-valOffset, offsetY+11);
	
	uView.print(strBuffer);
}

// -------------------------------------------------------------------------------------
// Gauge Widget - end
// -------------------------------------------------------------------------------------

/** \brief SPI Initialisation.

	Setup SCK, MOSI, SS and DC pins for SPI transmission.
*/
void MVSPIClass::begin() {
	// Set SS to high so the display will be "deselected" by default
	SSHIGH;
	// Warning: if the SS pin ever becomes a LOW INPUT then SPI
	// automatically switches to Slave.
	// This should not occur with the MicroView as nothing can drive the pin.

	// Set DC low for command mode, where it should always default to unless
	// data is being transmitted.
	DCLOW;

	// Set SCK and MOSI to be outputs and low when SPI is disabled.
	digitalWrite(SCK, LOW);
	digitalWrite(MOSI, LOW);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);

	// Set SPI master mode. Don't enable SPI at this time.
	SPCR |= _BV(MSTR);
}

/** \brief End SPI. */
void MVSPIClass::end() {
	SPCR &= ~_BV(SPE);
	// SCK and MOSI should already be low but set them again, to be sure.
	digitalWrite(SCK, LOW);
	digitalWrite(MOSI, LOW);
	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);
}

/** \brief Set up SPI for transmitting

	Pepare the SPI interface for transmitting on or more bytes of
	commands and/or data.
*/
void MVSPIClass::packetBegin() {
	// Enable SPI mode
	SPCR |= _BV(SPE);

	// Set SS low
	SSLOW;
}

/** \brief End a SPI packet transmission

	End a SPI packet transmission:
	- Wait for the last byte to finish being transmitted.
	- Set DC to command mode (even if already set that way, just in case).
	- Set SS high.
	- Disable SPI mode (causing SCK and MOSI to go low).
*/
void MVSPIClass::packetEnd() {
	while (!(SPSR & _BV(SPIF)))
	;
	DCLOW;
	SSHIGH;
	SPCR &= ~_BV(SPE);
}

/** \brief Set SPI bit order.

	Set SPI port bit order with LSBFIRST or MSBFIRST.
*/
void MVSPIClass::setBitOrder(uint8_t bitOrder)
{
	if(bitOrder == LSBFIRST) {
		SPCR |= _BV(DORD);
	} else {
		SPCR &= ~(_BV(DORD));
	}
}

/** \brief Set SPI data mode.

	Set the SPI data mode: clock polarity and phase.  mode - SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3.
*/
void MVSPIClass::setDataMode(uint8_t mode)
{
	SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
}

/** \brief Set clock divider.

	Set the clocl divider of the SPI, default is 4Mhz.

	rate:   SPI_CLOCK_DIV2
			SPI_CLOCK_DIV4
			SPI_CLOCK_DIV8
			SPI_CLOCK_DIV16
			SPI_CLOCK_DIV32
			SPI_CLOCK_DIV64
			SPI_CLOCK_DIV128 
*/
void MVSPIClass::setClockDivider(uint8_t rate)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

MVSPIClass MVSPI;
MicroView uView;

/** \brief Get the number of characters for a 16 bit signed value.

	Return the number of characters that would be printed using printf("%d", x) for x being a signed 16 bit integer.
*/
uint8_t getInt16PrintLen(int16_t val) {
	char sbuf[7];

	return sprintf(sbuf, "%d", val);
}

