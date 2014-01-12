#include <avr/pgmspace.h>
#include <MicroView.h>
#include <SPI.h>

void MICROVIEW::begin() {
	// SPI
	SPI.setClockDivider(SPI_CLOCK_DIV2); 
	SPI.begin();
	pinMode(MOSI, OUTPUT);
	pinMode(SCK, OUTPUT);
	pinMode(DC, OUTPUT);
	pinMode(RESET, OUTPUT);
	pinMode(cs, OUTPUT);
	digitalWrite(cs, HIGH);
	
	sckport     = portOutputRegister(digitalPinToPort(SCK));
	sckpinmask  = digitalPinToBitMask(SCK);
	mosiport    = portOutputRegister(digitalPinToPort(MOSI));
	mosipinmask = digitalPinToBitMask(MOSI);
	csport      = portOutputRegister(digitalPinToPort(cs));
	cspinmask   = digitalPinToBitMask(cs);
	dcport      = portOutputRegister(digitalPinToPort(DC));
	dcpinmask   = digitalPinToBitMask(DC);
	
	digitalWrite(RESET, HIGH);
	// VDD (3.3V) goes high at start, lets just chill for a ms
	delay(1);
	// bring reset low
	digitalWrite(RESET, LOW);
	// wait 10ms
	delay(10);
	// bring out of reset
	digitalWrite(RESET, HIGH);
	// turn on VCC 
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


//	command(MEMORYMODE);			// 0x20 
//	command(0x00);					// Horizontal Memory Addressing mode

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

void MICROVIEW::clear(void) {
	for (int i=0;i<8; i++) {
		setPageAddress(i);
		setColumnAddress(0);
		for (int j=0; j<0x80; j++) {
			data(0);
		}
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



