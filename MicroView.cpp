#include <avr/pgmspace.h>
#include <MicroView.h>
#include <SPI.h>

// Change to add fonts
#include <font5x7.h>
#include <font8x16.h>
#define TOTALFONTS			2
const unsigned char *MICROVIEW::fontsPointer[]={font5x7,font8x16};
// Change to add fonts


/*
Screen memory buffer 64 x 48 divided by 8 = 384 bytes
Screen memory buffer is required because in SPI mode, the host cannot read the SSD1306's GDRAM of the controller.  This buffer serves as a scratch RAM for graphical functions.
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
	
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0,	0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80,	0xC0, 0xC0, 0xE0, 0xE0, 0xF0, 0x78, 0xF8, 0xBC, 0xFC, 0xDE, 0xDE, 0xCF, 0xCF, 0xC7, 0xC3, 0xC3,	0xC7, 0x87, 0x8F, 0x0F, 0x1E, 0x1E, 0x3C, 0x78, 0x78, 0xF0, 0xF0, 0xE0, 0xE0, 0xC0, 0xC0, 0x80,	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFF, 0xEF, 0xE7, 0xC7,	0xC3, 0x83, 0x01, 0x01, 0x00, 0x0F, 0x1F, 0x3F, 0x71, 0x6E, 0xEE, 0xEF, 0xF3, 0xF7, 0xF6, 0xF9,	0xFD, 0xDD, 0xEB, 0x7F, 0x7E, 0xBC, 0xC0, 0xC0, 0xE0, 0xE0, 0xF0, 0xF1, 0x79, 0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0F, 0x0F, 0x1F, 0x1E, 0xFC, 0xFD, 0xF9, 0x7B,	0xF3, 0xE7, 0xEF, 0xCF, 0xDE, 0x9E, 0xBC, 0x3C, 0x78, 0x78, 0xF0, 0xF0, 0xF0, 0xF8, 0xF8, 0xFC,	0xBC, 0xFE, 0xFE, 0xFF, 0xE7, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,	0xFF, 0xFF, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00,	0x00, 0x01, 0x01, 0x03, 0x03, 0x07, 0x07, 0x0F, 0x0F, 0x1E, 0x3F, 0x3F, 0x3F, 0x1F, 0xFF, 0xFF,	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0x7F, 0x1F, 0x3F, 0x3F, 0x1F, 0x03, 0x07,	0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x0F,	0x0F, 0x07, 0x03, 0x03, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};


void MICROVIEW::begin() {
	
	// default 5x7 font
	/*
	fontType=0;
	fontWidth=pgm_read_byte(fontsPointer[fontType]+1);
	fontHeight=pgm_read_byte(fontsPointer[fontType]+2);
	fontStartChar=pgm_read_byte(fontsPointer[fontType]+3);
	fontTotalChar=pgm_read_byte(fontsPointer[fontType]+4);
	*/
	setFontType(0);
	
	// Setting up SPI pins
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(DC, OUTPUT);
	pinMode(RESET, OUTPUT);
	pinMode(CS, OUTPUT);
	digitalWrite(CS, HIGH);
	
	sckport     = portOutputRegister(digitalPinToPort(SCK));
	sckpinmask  = digitalPinToBitMask(SCK);
	mosiport    = portOutputRegister(digitalPinToPort(MOSI));
	mosipinmask = digitalPinToBitMask(MOSI);
	csport      = portOutputRegister(digitalPinToPort(CS));
	cspinmask   = digitalPinToBitMask(CS);
	dcport      = portOutputRegister(digitalPinToPort(DC));
	dcpinmask   = digitalPinToBitMask(DC);
	
	digitalWrite(RESET, HIGH);
	// VDD (3.3V) goes high at start, lets just chill for 5 ms
	delay(5);
	// bring reset low
	digitalWrite(RESET, LOW);
	
	// Setup SPI frequency
	SPI.setClockDivider(SPI_CLOCK_DIV2); 
	SPI.begin();
	
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
}

void MICROVIEW::command(uint8_t c) {
	// Hardware SPI
	*csport |= cspinmask;	// CS HIGH
	*dcport &= ~dcpinmask;	// DC LOW
	*csport &= ~cspinmask;	// CS LOW
	SPI.transfer(c);
	*csport |= cspinmask;	// CS HIGH
}

void MICROVIEW::data(uint8_t c) {
	// Hardware SPI
	*csport |= cspinmask;	// CS HIGH
	*dcport |= dcpinmask;	// DC HIGH
	*csport &= ~cspinmask;	// CS LOW
	SPI.transfer(c);
	*csport |= cspinmask;	// CS HIGH
}

void MICROVIEW::setPageAddress(uint8_t add) {
	add=0xb0|add;
	command(add);
	return;
}

void MICROVIEW::setColumnAddress(uint8_t add) {
	command((0x10|(add>>4))+0x02);
	command((0x0f&add));
	return;
}


/* 
	Clear GDRAM inside the LCD controller - mode = ALL
	Clear screen page buffer - mode = PAGE
*/
void MICROVIEW::clear(uint8_t mode) {
	uint8_t page=6, col=0x40;
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

void MICROVIEW::display(void) {
	uint8_t i, j;
	
	for (i=0; i<6; i++) {
		setPageAddress(i);
		setColumnAddress(0);
		for (j=0;j<0x40;j++) {
			data(screenmemory[i*0x40+j]);
		}
	}
}

void MICROVIEW::pixel(uint8_t x, uint8_t y, uint8_t color, uint8_t mode) {
	if ((x<0) ||  (x>LCDWIDTH-1) || (y<0) || (y>LCDHEIGHT-1))
	return;
	
	if (mode==XOR) {
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

// bresenham's algorithm 
void MICROVIEW::line(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color, uint8_t mode) {
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

void MICROVIEW::lineH(uint8_t x, uint8_t y, uint8_t width, uint8_t color, uint8_t mode) {
	line(x,y,x+width,y,color,mode);
}

void MICROVIEW::lineV(uint8_t x, uint8_t y, uint8_t height, uint8_t color, uint8_t mode) {
	line(x,y,x,y+height,color,mode);
}

void MICROVIEW::rect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
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

void MICROVIEW::rectFill(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color , uint8_t mode) {
	// TODO - need to optimise the memory map draw so that this function will not call pixel one by one
	for (int i=x; i<x+width;i++) {
		lineV(i,y, height, color, mode);
	}
}

void MICROVIEW::circle(uint8_t x0, uint8_t y0, uint8_t radius, uint8_t color, uint8_t mode) {
	
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


void MICROVIEW::circleFill(uint8_t x0, uint8_t y0, uint8_t r, uint8_t color, uint8_t mode) {
	// TODO - - find a way to check for no overlapping of pixels so that XOR draw mode will work perfectly 
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t x = 0;
	int8_t y = r;

	// Temporary disable fill circle for XOR mode.
	if (mode==XOR) return;
	
	for (uint8_t i=y0-r; i<=y0+r; i++) {
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

uint8_t MICROVIEW::getFontWidth(void) {
	return fontWidth;
}

uint8_t MICROVIEW::getFontHeight(void) {
	return fontHeight;
}

uint8_t MICROVIEW::getFontStartChar(void) {
	return fontStartChar;
}

uint8_t MICROVIEW::getFontTotalChar(void) {
	return fontTotalChar;
}

uint8_t MICROVIEW::getTotalFonts(void) {
	return TOTALFONTS;
}

uint8_t MICROVIEW::getFontType(void) {
	return fontType;
}

uint8_t MICROVIEW::setFontType(uint8_t type) {
	if ((type>=TOTALFONTS) || (type<0))
	return -1;

	fontType=type;
	fontWidth=pgm_read_byte(fontsPointer[fontType]+1);
	fontHeight=pgm_read_byte(fontsPointer[fontType]+2);
	fontStartChar=pgm_read_byte(fontsPointer[fontType]+3);
	fontTotalChar=pgm_read_byte(fontsPointer[fontType]+4);
	fontMapWidth=(pgm_read_byte(fontsPointer[fontType]+5)*100)+pgm_read_byte(fontsPointer[fontType]+6); // two bytes values into integer 16

}

void  MICROVIEW::drawChar(uint8_t x, uint8_t line, uint8_t c, uint8_t mode) {
	uint8_t rowsToDraw,row;
	uint8_t i,tempX;
	uint16_t charPerBitmapRow,charColPositionOnBitmap,charRowPositionOnBitmap,charBitmapStartPosition;
	// TODO - char must be able to be drawn anywhere, not limited by line
	// TODO - char must be able to XOR on background
	
	if ((line >= LCDHEIGHT/fontHeight) || (x > (LCDWIDTH - fontWidth)))
	return;
	
	// TODO - check if char is not in memory


	tempX=x;
	// each row (in datasheet is call page) is 8 bits high, 16 bit high character will have 2 rows to be drawn
	rowsToDraw=fontHeight/8;	// 8 is LCD's page size, see SSD1306 datasheet
	if (rowsToDraw<=1) rowsToDraw=1;

	// only 1 row to draw for font with 8 bit height
	if (rowsToDraw==1) {
		for (i=0; i<fontWidth; i++ ) {
			screenmemory[tempX + (line*LCDWIDTH) ] = pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(c*fontWidth)+i);
			tempX++;
		}
		return;
	}


	// font height over 8 bit
	// take character "0" ASCII 48 as example
	charPerBitmapRow=fontMapWidth/fontWidth;  // 256/8 =32 char per row
	charColPositionOnBitmap=c % charPerBitmapRow;  // =16
	charRowPositionOnBitmap=int(c/charPerBitmapRow); // =1
	charBitmapStartPosition=(fontMapWidth * (fontHeight/8)) + (charColPositionOnBitmap * fontWidth);
	
	tempX=x;
	for (row=0; row<rowsToDraw; row++) {
		for (i=0; i<fontWidth; i++ ) {
			screenmemory[tempX + (( (line*(fontHeight/8)) +row)*LCDWIDTH) ] = pgm_read_byte(fontsPointer[fontType]+FONTHEADERSIZE+(charBitmapStartPosition+i+(row*fontMapWidth)));
			tempX++;
		}
		tempX=x;
	}
}

void MICROVIEW::stopScroll(void){
	command(DEACTIVATESCROLL);
}

void MICROVIEW::scrollRight(uint8_t start, uint8_t stop){
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



