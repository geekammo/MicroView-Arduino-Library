#include <avr/pgmspace.h>
#include <SPI.h>
#include <MicroView.h>

// This fixed ugly GCC warning "only initialized variables can be placed into program memory area"
#undef PROGMEM
#define PROGMEM __attribute__((section(".progmem.data")))

// Add header of the fonts here.  Remove as many as possible to get conserve FLASH memory.
#include <font5x7.h>
#include <font8x16.h>
#include <fontlargenumber.h>
#include <7segment.h>
#include <space01.h>
#include <space02.h>
#include <space03.h>

// Change the total fonts included
#define TOTALFONTS		7

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

/*
Page buffer 64 x 48 divided by 8 = 384 bytes
Page buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This page buffer serves as a scratch RAM for graphical functions.  All drawing function will first be drawn on this page buffer, only upon calling display() function will transfer the page buffer to the actual LCD controller's memory.
*/
static uint8_t screenmemory [] = {
	// LCD Memory organised in 64 horizontal pixel and 6 rows of byte
	// B  B .............B  -----
	// y  y .............y        \        
	// t  t .............t         \
	// e  e .............e          \
	// 0  1 .............63          \
	//                                \
	// D0 D0.............D0            \
	// D1 D1.............D1            / ROW 0
	// D2 D2.............D2           /
	// D3 D3.............D3          /
	// D4 D4.............D4         /
	// D5 D5.............D5        /
	// D6 D6.............D6       /
	// D7 D7.............D7  ----
	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0,
	0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0,
	0xE0, 0xE0, 0xF0, 0xF0, 0x78, 0x38, 0xDC, 0xDC, 0xFE, 0xFE, 0x6F, 0x6F, 0xE7, 0xE7, 0x63, 0x61,
	0xC1, 0xC1, 0xC3, 0x87, 0x87, 0x0F, 0x0F, 0x1E, 0x1E, 0x3C, 0x3C, 0x78, 0x78, 0xF0, 0xF0, 0xE0,
	0xE0, 0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFE, 0xFE, 0x3F, 0x7F, 0x77, 0xF3, 0xE3,
	0xE1, 0xC1, 0xC0, 0x80, 0x80, 0x07, 0x0F, 0x1F, 0x38, 0x30, 0x36, 0x76, 0x61, 0x61, 0x60, 0x68,
	0x69, 0x66, 0x60, 0x61, 0xB1, 0xBF, 0xDE, 0xC0, 0xE0, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC, 0xBC, 0xBF,
	0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x07, 0x0F, 0xFF, 0xFE, 0xFE, 0x3C,
	0x3D, 0x79, 0x7B, 0xF3, 0xF7, 0xE7, 0xCF, 0xCE, 0x9E, 0x9C, 0x3C, 0xF8, 0xF8, 0xF8, 0x7C, 0xFC,
	0xFE, 0xCE, 0xEF, 0xFF, 0xFF, 0xFB, 0xFB, 0x7F, 0xFF, 0xFE, 0xFE, 0x1F, 0x0F, 0xFF, 0xFF, 0xFF,
	0x01, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF,
	0xFF, 0xFF, 0x03, 0x01, 0x7F, 0xFF, 0x7F, 0x00, 0x1F, 0x3F, 0x1F, 0x00, 0x00, 0x07, 0x0F, 0x07,
	0x00, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
	0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

void MicroView::begin() {
	// default 5x7 font
	setFontType(0);
	setColor(WHITE);
	setDrawMode(NORM);
	setCursor(0,0);

	// Setting up SPI pins
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(DC, OUTPUT);
	pinMode(RESET, OUTPUT);
	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);
	
	sckport     = portOutputRegister(digitalPinToPort(SCK));
	sckpinmask  = digitalPinToBitMask(SCK);
	mosiport    = portOutputRegister(digitalPinToPort(MOSI));
	mosipinmask = digitalPinToBitMask(MOSI);
	ssport      = portOutputRegister(digitalPinToPort(SS));
	sspinmask   = digitalPinToBitMask(SS);
	dcport      = portOutputRegister(digitalPinToPort(DC));
	dcpinmask   = digitalPinToBitMask(DC);
	
	digitalWrite(RESET, HIGH);
	// VDD (3.3V) goes high at start, lets just chill for 5 ms
	delay(5);
	// bring reset low
	digitalWrite(RESET, LOW);
	
	// Setup SPI frequency
	MVSPI.setClockDivider(SPI_CLOCK_DIV2); 
	MVSPI.begin();
	
	// wait 10ms
	delay(10);
	// bring out of reset
	digitalWrite(RESET, HIGH);

	// Init sequence for 64x48 OLED module
	command(DISPLAYOFF);			// 0xAE

	command(SETDISPLAYCLOCKDIV);	// 0xD5
	command(0x80);					// the suggested ratio 0x80

	command(SETMULTIPLEX);			// 0xA8
	command(0x2F);

	command(SETDISPLAYOFFSET);		// 0xD3
	command(0x0);					// no offset

	command(SETSTARTLINE | 0x0);	// line #0

	command(CHARGEPUMP);			// enable charge pump
	command(0x14);

	command(NORMALDISPLAY);			// 0xA6
	command(DISPLAYALLONRESUME);	// 0xA4

	command(SEGREMAP | 0x1);

	command(COMSCANDEC);

	command(SETCOMPINS);			// 0xDA
	command(0x12);

	command(SETCONTRAST);			// 0x81
	command(0x8F);

	command(SETPRECHARGE);			// 0xd9
	command(0xF1);
	
	command(SETVCOMDESELECT);			// 0xDB
	command(0x40);

	command(DISPLAYON);				//--turn on oled panel
	clear(ALL);						// Erase hardware memory inside the OLED controller to avoid random data in memory.
}

void MicroView::command(uint8_t c) {
	// Hardware SPI
	*ssport |= sspinmask;	// SS HIGH
	*dcport &= ~dcpinmask;	// DC LOW
	*ssport &= ~sspinmask;	// SS LOW
	MVSPI.transfer(c);
	*ssport |= sspinmask;	// SS HIGH
}

void MicroView::data(uint8_t c) {
	// Hardware SPI
	*ssport |= sspinmask;	// SS HIGH
	*dcport |= dcpinmask;	// DC HIGH
	*ssport &= ~sspinmask;	// SS LOW
	MVSPI.transfer(c);
	*ssport |= sspinmask;	// SS HIGH
}

void MicroView::setPageAddress(uint8_t add) {
	add=0xb0|add;
	command(add);
	return;
}

void MicroView::setColumnAddress(uint8_t add) {
	command((0x10|(add>>4))+0x02);
	command((0x0f&add));
	return;
}

/* 
	Clear GDRAM inside the LCD controller - mode = ALL
	Clear screen page buffer - mode = PAGE
*/
void MicroView::clear(uint8_t mode) {
	//	uint8_t page=6, col=0x40;
	if (mode==ALL) {
		for (int i=0;i<8; i++) {
			setPageAddress(i);
			setColumnAddress(0);
			for (int j=0; j<0x80; j++) {
				data(0);
			}
		}
	}
	else
	{
		memset(screenmemory,0,384);			// (64 x 48) / 8 = 384
		display();
	}
}

/*	
	Clear GDRAM inside the LCD controller - mode = ALL with c character.
	Clear screen page buffer - mode = PAGE with c character.
*/
void MicroView::clear(uint8_t mode, uint8_t c) {
	//uint8_t page=6, col=0x40;
	if (mode==ALL) {
		for (int i=0;i<8; i++) {
			setPageAddress(i);
			setColumnAddress(0);
			for (int j=0; j<0x80; j++) {
				data(c);
			}
		}
	}
	else
	{
		memset(screenmemory,c,384);			// (64 x 48) / 8 = 384
		display();
	}	
}

// This routine is to transfer the page buffer to the LCD controller's memory.
void MicroView::display(void) {
	uint8_t i, j;
	
	for (i=0; i<6; i++) {
		setPageAddress(i);
		setColumnAddress(0);
		for (j=0;j<0x40;j++) {
			data(screenmemory[i*0x40+j]);
		}
	}
}

#if ARDUINO >= 100
size_t MicroView::write(uint8_t c) {
#else
	void MicroView::write(uint8_t c) {
#endif
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
#if ARDUINO >= 100
		return 1;
#endif
	}

	void MicroView::setCursor(uint8_t x, uint8_t y) {
		cursorX=x;
		cursorY=y;
	}

	void MicroView::pixel(uint8_t x, uint8_t y) {
		pixel(x,y,foreColor,drawMode);
	}

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

	// Draw line using current fore color and current draw mode
	void MicroView::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
		line(x0,y0,x1,y1,foreColor,drawMode);
	}

	// Draw line using color and mode
	// bresenham's algorithm 
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

	//	Draw horizontal line using current fore color and current draw mode
	void MicroView::lineH(uint8_t x, uint8_t y, uint8_t width) {
		line(x,y,x+width,y,foreColor,drawMode);
	}

	//	Draw horizontal line using color and draw mode
	void MicroView::lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode) {
		line(x,y,x+width,y,color,mode);
	}

	//	Draw vertical line using current fore color and current draw mode
	void MicroView::lineV(uint8_t x, uint8_t y, uint8_t height) {
		line(x,y,x,y+height,foreColor,drawMode);
	}

	//	Draw vertical line using color and draw mode
	void MicroView::lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode) {
		line(x,y,x,y+height,color,mode);
	}

	// Draw rectangle using current fore color and current draw mode
	void MicroView::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
		rect(x,y,width,height,foreColor,drawMode);
	}

	// Draw rectangle using color and draw mode
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
	

	// Draw filled rectangle using current fore color and current draw mode
	void MicroView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
		rectFill(x,y,width,height,foreColor,drawMode);
	}
	
	// Draw filled rectangle using color and mode
	void MicroView::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
		// TODO - need to optimise the memory map draw so that this function will not call pixel one by one
		for (int i=x; i<x+width;i++) {
			lineV(i,y, height, color, mode);
		}
	}

	// Draw circle using current fore color and current draw mode
	void MicroView::circle(uint8_t x0, uint8_t y0, uint8_t radius) {
		circle(x0,y0,radius,foreColor,drawMode);
	}

	// Draw circle using color and mode
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


	// Draw filled circle using current fore color and current draw mode
	void MicroView::circleFill(uint8_t x0, uint8_t y0, uint8_t radius) {
		circleFill(x0,y0,radius,foreColor,drawMode);
	}

	// Draw filled circle using color and mode
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

	uint8_t MicroView::getLCDHeight(void) {
		return LCDHEIGHT;
	}
	
	uint8_t MicroView::getLCDWidth(void) {
		return LCDWIDTH;
	}
	
	uint8_t MicroView::getFontWidth(void) {
		return fontWidth;
	}

	uint8_t MicroView::getFontHeight(void) {
		return fontHeight;
	}

	uint8_t MicroView::getFontStartChar(void) {
		return fontStartChar;
	}

	uint8_t MicroView::getFontTotalChar(void) {
		return fontTotalChar;
	}

	uint8_t MicroView::getTotalFonts(void) {
		return TOTALFONTS;
	}

	uint8_t MicroView::getFontType(void) {
		return fontType;
	}

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

	void MicroView::setColor(uint8_t color) {
		foreColor=color;
	}

	void MicroView::setDrawMode(uint8_t mode) {
		drawMode=mode;
	}

	// Draw character using current fore color and current draw mode
	void  MicroView::drawChar(uint8_t x, uint8_t y, uint8_t c) {
		drawChar(x,y,c,foreColor,drawMode);
	}

	// Draw character using color and mode
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

	void MicroView::stopScroll(void){
		command(DEACTIVATESCROLL);
	}

	void MicroView::scrollRight(uint8_t start, uint8_t stop){
		if (stop<start)		// stop must be larger or equal to start
		return;
		stopScroll();		// need to disable scrolling before starting to avoid memory corrupt
		command(RIGHTHORIZONTALSCROLL);
		command(0x00);
		command(start);
		command(0x7);		// scroll speed frames , TODO
		command(stop);
		command(0x00);
		command(0xFF);
		command(ACTIVATESCROLL);
	}

	MicroViewWidget::MicroViewWidget(uint8_t newx, uint8_t newy, int16_t min, int16_t max) {
		setX(newx);
		setY(newy);
		value=0;
		//		if (min>max) {
		//			setMinValue(max);
		//			setMaxValue(min);
		//		}
		//		else {
		setMinValue(min);
		setMaxValue(max);
		//		}
		//drawFace();
		//setValue(min);
	}

	uint8_t MicroViewWidget::getX() { return x; }
	uint8_t MicroViewWidget::getY() { return y; }
	void MicroViewWidget::setX(uint8_t newx) { x = newx; }
	void MicroViewWidget::setY(uint8_t newy) { y = newy; }

	int16_t MicroViewWidget::getMinValue() { return minValue; }
	int16_t MicroViewWidget::getMaxValue() { return maxValue; }

	int16_t MicroViewWidget::getValue() { return value; }

	void MicroViewWidget::setMinValue(int16_t min) { minValue=min; }
	void MicroViewWidget::setMaxValue(int16_t max) { maxValue=max; }
	void MicroViewWidget::setValue(int16_t val) { 
		if (val<=maxValue) { 
			value=val; 
			this->draw();
		}
	}

	// -------------------------------------------------------------------------------------
	// Slider Widget - start
	// -------------------------------------------------------------------------------------

	MicroViewSlider::MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max):MicroViewWidget(newx, newy, min, max) {
		style=0;
		totalTicks=30;
		needFirstDraw=true;
		prevValue=getMinValue();
		drawFace();
		draw();
	}

	MicroViewSlider::MicroViewSlider(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty):MicroViewWidget(newx, newy, min, max) {
		if (sty==WIDGETSTYLE0) {
			style=0;
			totalTicks=30;
		}
		else {
			style=1;
			totalTicks=60;
		}

		needFirstDraw=true;
		prevValue=getMinValue();
		drawFace();
		draw();
	}


	void MicroViewSlider::drawFace() {
		uint8_t offsetX, offsetY, majorLine;
		offsetX=getX();
		offsetY=getY();
		
		if(style>0)
		majorLine=7;
		else
		majorLine=4;
		
		// Draw major tickers
		for (uint8_t i=0; i<majorLine;i++) {
			uView.lineV(offsetX+1+(i*10), offsetY+3, 5);
		}
		// Draw minor tickers
		for (uint8_t i=0; i<4;i++) {
			uView.lineV(offsetX+3+(i*2), offsetY+5, 3);
		}
		for (uint8_t i=0; i<4;i++) {
			uView.lineV(offsetX+13+(i*2), offsetY+5, 3);
		}
		for (uint8_t i=0; i<4;i++) {
			uView.lineV(offsetX+23+(i*2), offsetY+5, 3);
		}
		
		if(style>0) {
			for (uint8_t i=0; i<4;i++) {
				uView.lineV(offsetX+33+(i*2), offsetY+5, 3);
			}
			if (style>0) {
				for (uint8_t i=0; i<4;i++) {
					uView.lineV(offsetX+43+(i*2), offsetY+5, 3);
				}
				for (uint8_t i=0; i<4;i++) {
					uView.lineV(offsetX+53+(i*2), offsetY+5, 3);
				}
			}
		}
		
	}

	void MicroViewSlider::draw() {
		uint8_t offsetX, offsetY;
		uint8_t tickPosition=0;
		char strBuffer[5];
		offsetX=getX();
		offsetY=getY();

		if (needFirstDraw) {
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
			sprintf(strBuffer,"%4d", prevValue);	// we need to force 4 digit so that blank space will cover larger value
			needFirstDraw=false;
		}
		else {
			// Draw previous pointer in XOR mode to erase it
			tickPosition= (((float)(prevValue-getMinValue())/(float)(getMaxValue()-getMinValue()))*totalTicks);
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
			// Draw current pointer
			tickPosition= (((float)(getValue()-getMinValue())/(float)(getMaxValue()-getMinValue()))*totalTicks);
			uView.lineH(offsetX+tickPosition,offsetY, 3, WHITE, XOR);
			uView.pixel(offsetX+1+tickPosition,offsetY+1, WHITE, XOR);
			sprintf(strBuffer,"%4d", getValue());	// we need to force 4 digit so that blank space will cover larger value
			prevValue=getValue();
		}

		// Draw value
		if(style>0) 
		uView.setCursor(offsetX,offsetY+10);
		else
		uView.setCursor(offsetX+34,offsetY+1);
		uView.print(strBuffer);
	}

	// -------------------------------------------------------------------------------------
	// Slider Widget - end
	// -------------------------------------------------------------------------------------

	// -------------------------------------------------------------------------------------
	// Gauge Widget - start
	// -------------------------------------------------------------------------------------

	MicroViewGauge::MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max):MicroViewWidget(newx, newy, min, max) {
		style=0;
		radius=15;
		needFirstDraw=true;
		prevValue=getMinValue();
		drawFace();
		draw();
	}

	MicroViewGauge::MicroViewGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max, uint8_t sty):MicroViewWidget(newx, newy, min, max) {
		if (sty==WIDGETSTYLE0) {
			style=0;
			radius=15;
		}
		else {
			style=1;
			radius=23;
		}
		needFirstDraw=true;
		prevValue=getMinValue();
		drawFace();
		draw();
	}


	void MicroViewGauge::drawFace() {
		uint8_t offsetX, offsetY, majorLine;
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

	void MicroViewGauge::draw() {
		uint8_t offsetX, offsetY;
		uint8_t tickPosition=0;
		float degreeSec, fromSecX, fromSecY, toSecX, toSecY;

		char strBuffer[5];
		offsetX=getX();
		offsetY=getY();

		if (needFirstDraw) {
			degreeSec = (((float)(prevValue-getMinValue())/(float)(getMaxValue()-getMinValue()))*240);	// total 240 degree in the widget
			degreeSec = (degreeSec+150) * (PI/180);		// 150 degree starting point
			toSecX = cos(degreeSec) * (radius / 1.2);
			toSecY = sin(degreeSec) * (radius / 1.2);
			uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);
			sprintf(strBuffer,"%4d", prevValue);	// we need to force 4 digit so that blank space will cover larger value
			needFirstDraw=false;
		}
		else {
			// Draw previous pointer in XOR mode to erase it
			degreeSec = (((float)(prevValue-getMinValue())/(float)(getMaxValue()-getMinValue()))*240);	// total 240 degree in the widget
			degreeSec = (degreeSec+150) * (PI/180);
			toSecX = cos(degreeSec) * (radius / 1.2);
			toSecY = sin(degreeSec) * (radius / 1.2);
			uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);

			degreeSec = (((float)(getValue()-getMinValue())/(float)(getMaxValue()-getMinValue()))*240);	// total 240 degree in the widget
			degreeSec = (degreeSec+150) * (PI/180);		// 150 degree starting point
			toSecX = cos(degreeSec) * (radius / 1.2);
			toSecY = sin(degreeSec) * (radius / 1.2);
			uView.line(offsetX,offsetY,1+offsetX+toSecX,1+offsetY+toSecY, WHITE,XOR);

			sprintf(strBuffer,"%4d", getValue());	// we need to force 4 digit so that blank space will cover larger value
			prevValue=getValue();
		}

		// Draw value
		if(style>0) 
		uView.setCursor(offsetX-10,offsetY+10);
		else
		uView.setCursor(offsetX-11,offsetY+11);
		
		uView.print(strBuffer);
	}

	// -------------------------------------------------------------------------------------
	// Slider Widget - end
	// -------------------------------------------------------------------------------------


	void MVSPIClass::begin() {
		// Set SS to high so a connected chip will be "deselected" by default
		digitalWrite(SS, HIGH);

		// When the SS pin is set as OUTPUT, it can be used as
		// a general purpose output port (it doesn't influence
		// SPI operations).
		pinMode(SS, OUTPUT);

		// Warning: if the SS pin ever becomes a LOW INPUT then SPI
		// automatically switches to Slave, so the data direction of
		// the SS pin MUST be kept as OUTPUT.
		SPCR |= _BV(MSTR);
		SPCR |= _BV(SPE);

		// Set direction register for SCK and MOSI pin.
		// MISO pin automatically overrides to INPUT.
		// By doing this AFTER enabling SPI, we avoid accidentally
		// clocking in a single bit since the lines go directly
		// from "input" to SPI control.  
		// http://code.google.com/p/arduino/issues/detail?id=888
		pinMode(SCK, OUTPUT);
		pinMode(MOSI, OUTPUT);
	}


	void MVSPIClass::end() {
		SPCR &= ~_BV(SPE);
	}

	void MVSPIClass::setBitOrder(uint8_t bitOrder)
	{
		if(bitOrder == LSBFIRST) {
			SPCR |= _BV(DORD);
		} else {
			SPCR &= ~(_BV(DORD));
		}
	}

	void MVSPIClass::setDataMode(uint8_t mode)
	{
		SPCR = (SPCR & ~SPI_MODE_MASK) | mode;
	}

	void MVSPIClass::setClockDivider(uint8_t rate)
	{
		SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
		SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
	}

	MVSPIClass MVSPI;
	MicroView uView;


