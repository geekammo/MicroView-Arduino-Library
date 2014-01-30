#include <MicroView.h>
#include <SPI.h>
#include <Time.h>

#define PI 3.141592654
#define clocksize 24

MICROVIEW mv;

uint8_t dly=5;		// This is the erase delay in milliseconds, if there is no delay, the draw/erase will be too fast to be seen.

void setup() {
	mv.begin();		// Begin of MicroView
	mv.clear(ALL);	// Erase hardware memory inside the OLED controller
	mv.display();	// Display the content in the buffer memory, by default it is the MicroView logo
	setTime(10,10,01,17,1,2014);
	delay(500);
	mv.clear(PAGE);	// Erase the memory buffer, when next mv.display() is called, the OLED will be cleared.
}

void loop() {
	int i;
	static long counter=99999;
	static long mSec=millis()+1000;
	static uint8_t x0,y0,x1,y1;
	static float degresshour,degressmin,degresssec,hourx,houry,minx,miny,secx,secy;
	static boolean drawnFirst=false;

	mv.setFontType(0); 		// set font type 0, please see declaration in MicroView.cpp
	mv.setCursor(27,0);		// points cursor to x=27 y=0
	mv.print(12);			
	mv.setCursor(30,mv.getLCDHeight()-mv.getFontHeight());
	mv.print(6);
	mv.setCursor(0,mv.getLCDHeight() /2-(mv.getFontHeight()/2));
	mv.print(9);
	mv.setCursor(mv.getLCDWidth()-mv.getFontWidth(),mv.getLCDWidth()/2-(mv.getFontHeight()/2));
	mv.print(3);
	mv.display();			// display the memory buffer drawn

	while ((second() % 10 )!=0) {
		if (mSec!=second()) {
			if (drawnFirst) {
				mv.line(32,24,32+hourx,24+houry,WHITE,XOR);
				mv.line(32,24,32+minx,24+miny,WHITE,XOR);
				mv.line(32,24,32+secx,24+secy,WHITE,XOR);
			} 
			
			degresshour = (((hour() * 360) / 12) + 270) * (PI / 180);
			degressmin = (((minute() * 360) / 60) + 270) * (PI / 180);
			degresssec = (((second() * 360) / 60) + 270) * (PI / 180);
			
			hourx = cos(degresshour) * (clocksize / 2.5);
			houry = sin(degresshour) * (clocksize / 2.5);

			minx = cos(degressmin) * (clocksize / 1.4);
			miny = sin(degressmin) * (clocksize / 1.4);

			secx = cos(degresssec) * (clocksize / 1.1);
			secy = sin(degresssec) * (clocksize / 1.1);
			

			mv.line(32,24,32+hourx,24+houry,WHITE,XOR);
			mv.line(32,24,32+minx,24+miny,WHITE,XOR);
			mv.line(32,24,32+secx,24+secy,WHITE,XOR);
			drawnFirst=true;
			mv.display();
			
			mSec=second();
		}
	}
	drawnFirst=false;
	mv.clear(PAGE);

	mv.setFontType(0);
	mv.setCursor(0,40);
	mv.print("   LINE   ");    
	mv.display();
	delay(500);
	
	for (i=0; i<150;i++) {
		x0=random(64);
		x1=random(64);
		y0=random(48);
		y1=random(48);
		
		mv.line(x0,y0,x1,y1, WHITE, XOR);		// draw line from x0,y0 to x1,y1 using WHITE color and XOR draw mode
		mv.display();
		delay(dly);    
		mv.line(x0,y0,x1,y1, WHITE,XOR);
		mv.display();
	}

	mv.setCursor(0,40);
	mv.print("RECTANGLE ");    
	mv.display();
	delay(500);

	x0=0;y0=0;x1=0;y1=0;
	for (i=1; i<64;i++) {
		y1=i;
		if (y1>47) y1=47;
		mv.rect(x0,y0,i,y1,WHITE,XOR);			// draw rectangle from x0,y0 with width of i and height of y1 using WHITE color and XOR draw mode
		mv.display();
		delay(dly);
		mv.rect(x0,y0,i,y1,WHITE,XOR);
		mv.display();
	}

	mv.setCursor(0,40);
	mv.print("  CIRCLE  ");    
	mv.display();
	delay(500);
	
	x0=32;y0=24;
	for (i=0;i<32;i++) {
		mv.circle(x0,y0,i,WHITE,XOR);			// draw circle at x0,y0 with radius of i using WHITE color and XOR draw mode
		mv.display();
		delay(dly);
		mv.circle(x0,y0,i,WHITE,XOR);
		mv.display();
		delay(dly);
		
	}
	delay(500);
	
	mv.clear(PAGE);
	mv.setCursor(0,40);
	mv.print("  Font 0  ");    
	mv.display();

	mv.setFontType(0);
	mv.setCursor(0,0);
	mv.print("01234567890ABCDabcd01234567890ABCDabcd");
	mv.display();
	delay(1500);


	mv.clear(PAGE);
	mv.setCursor(0,40);
	mv.print("  Font 1  ");    
	mv.display();

	mv.setFontType(1);
	mv.setCursor(0,0);
	mv.print("0123ABCDabcd");
	mv.display();
	delay(1500);
	mv.clear(PAGE);
	
	counter=99999;
	while (counter>99970) {
		
		if (millis()>=mSec) {
			
			mv.setFontType(2);
			mv.setCursor(0,0);
			mv.print(counter);

			counter--;
			mv.display();
			mSec=millis()+100;
		}
	}
	mv.clear(PAGE);
}


