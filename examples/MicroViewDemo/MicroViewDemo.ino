#include <MicroView.h>
#include <Time.h>

#define clocksize 24

uint8_t onDelay=5;		// this is the on delay in milliseconds, if there is no on delay, the erase will be too fast to clean up the screen.

void setup() {
	uView.begin();		// begin of MicroView
	uView.clear(ALL);	// erase hardware memory inside the OLED controller
	uView.display();	// display the content in the buffer memory, by default it is the MicroView logo
	setTime(10,10,01,17,1,2014);
	delay(500);
	uView.clear(PAGE);	// erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
}

void loop() {
	int i;
	static double counter=99999;
	static unsigned long mSec=millis()+1000;
	static uint8_t x0,y0,x1,y1;
	static float degresshour,degressmin,degresssec,hourx,houry,minx,miny,secx,secy;
	static boolean drawnFirst=false;

	uView.setFontType(0); 		// set font type 0, please see declaration in MicroView.cpp
	uView.setCursor(27,0);		// points cursor to x=27 y=0
	uView.print(12);			
	uView.setCursor(30,uView.getLCDHeight()-uView.getFontHeight());
	uView.print(6);
	uView.setCursor(0,uView.getLCDHeight() /2-(uView.getFontHeight()/2));
	uView.print(9);
	uView.setCursor(uView.getLCDWidth()-uView.getFontWidth(),uView.getLCDWidth()/2-(uView.getFontHeight()/2));
	uView.print(3);
	uView.display();			// display the memory buffer drawn

	while ((second() % 11 )!=0) {
		if (mSec!=(unsigned long)second()) {
			if (drawnFirst) {
				uView.line(32,24,32+hourx,24+houry,WHITE,XOR);
				uView.line(32,24,32+minx,24+miny,WHITE,XOR);
				uView.line(32,24,32+secx,24+secy,WHITE,XOR);
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
			

			uView.line(32,24,32+hourx,24+houry,WHITE,XOR);
			uView.line(32,24,32+minx,24+miny,WHITE,XOR);
			uView.line(32,24,32+secx,24+secy,WHITE,XOR);
			drawnFirst=true;
			uView.display();
			
			mSec=second();
		}
	}
	drawnFirst=false;
	uView.clear(PAGE);

	int maxX=40;
	onDelay=30;
	uView.setFontType(0);
	uView.setCursor(0,40);
	uView.print("  SPRITE  ");
	for (int x=0; x<maxX;x+=2) {
		uView.setFontType(4);    
		uView.drawChar(x,0,48,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(maxX-x,10,48,WHITE,XOR);

		uView.setFontType(6);
		uView.drawChar(x,32,48,WHITE,XOR);

		uView.display();
		delay(onDelay);
		uView.setFontType(4);
		uView.drawChar(x,0,48,WHITE, XOR);
		uView.setFontType(5); 
		uView.drawChar(maxX-x,10,48,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,48,WHITE,XOR);

		uView.display();
		uView.setFontType(4);
		uView.drawChar(x,0,49,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(maxX-x,10,49,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,49,WHITE,XOR);

		uView.display();
		delay(onDelay);
		uView.setFontType(4);
		uView.drawChar(x,0,49,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(maxX-x,10,49,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,49,WHITE,XOR);
		uView.display();
	}

	for (int x=maxX; x>0;x-=2) {
		uView.setFontType(4);
		uView.drawChar(x,10,48,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(40-x,0,48,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,48,WHITE,XOR);

		uView.display();
		delay(onDelay);
		uView.setFontType(4);
		uView.drawChar(x,10,48,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(40-x,0,48,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,48,WHITE,XOR);

		uView.display();
		uView.setFontType(4);
		uView.drawChar(x,10,49,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(40-x,0,49,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,49,WHITE,XOR);

		uView.display();
		delay(onDelay);
		uView.setFontType(4);
		uView.drawChar(x,10,49,WHITE, XOR);
		uView.setFontType(5);
		uView.drawChar(40-x,0,49,WHITE,XOR);
		uView.setFontType(6);
		uView.drawChar(x,32,49,WHITE,XOR);

		uView.display();
	}


	onDelay=5;
	uView.setFontType(0);
	uView.setCursor(0,40);
	uView.print("   LINE   ");    
	uView.display();
	delay(500);
	
	for (i=0; i<150;i++) {
		x0=random(64);
		x1=random(64);
		y0=random(48);
		y1=random(48);
		
		uView.line(x0,y0,x1,y1, WHITE, XOR);		// draw line from x0,y0 to x1,y1 using WHITE color and XOR draw mode
		uView.display();
		delay(onDelay);    
		uView.line(x0,y0,x1,y1, WHITE,XOR);
		uView.display();
	}

	uView.setCursor(0,40);
	uView.print("RECTANGLE ");    
	uView.display();
	delay(500);

	x0=0;y0=0;x1=0;y1=0;
	for (i=1; i<64;i++) {
		y1=i;
		if (y1>47) y1=47;
		uView.rect(x0,y0,i,y1,WHITE,XOR);			// draw rectangle from x0,y0 with width of i and height of y1 using WHITE color and XOR draw mode
		uView.display();
		delay(onDelay);
		uView.rect(x0,y0,i,y1,WHITE,XOR);
		uView.display();
	}

	uView.setCursor(0,40);
	uView.print("  CIRCLE  ");    
	uView.display();
	delay(500);
	
	x0=32;y0=24;
	for (i=0;i<32;i++) {
		uView.circle(x0,y0,i,WHITE,XOR);			// draw circle at x0,y0 with radius of i using WHITE color and XOR draw mode
		uView.display();
		delay(onDelay);
		uView.circle(x0,y0,i,WHITE,XOR);
		uView.display();
		delay(onDelay);
		
	}
	delay(500);
	
	uView.clear(PAGE);
	uView.setCursor(0,40);
	uView.print("  Font 0  ");    
	uView.display();

	uView.setFontType(0);
	uView.setCursor(0,0);
	uView.print("01234567890ABCDabcd01234567890ABCDabcd");
	uView.display();
	delay(1500);

	uView.clear(PAGE);
	uView.setCursor(0,40);
	uView.print("  Font 1  ");    
	uView.display();

	uView.setFontType(1);
	uView.setCursor(0,0);
	uView.print("0123ABCDabcd");
	uView.display();
	delay(1500);
	uView.clear(PAGE);
	
	counter=99999;
	while (counter>99970) {
		
		if (millis()>=mSec) {
			
			uView.setFontType(3);
			uView.setCursor(0,0);
			uView.print(counter);

			counter--;
			uView.display();
			mSec=millis()+100;
		}
	}
	uView.clear(PAGE);
	
	counter=19.99;
	while (counter<20.2) {
		
		if (millis()>=mSec) {
			
			uView.setFontType(2);
			uView.setCursor(0,0);
			uView.print(counter + ((int)(counter*100)%20));
			
			uView.setCursor(0,uView.getFontHeight());
			uView.print(50-counter);
			
			uView.setCursor(0,(uView.getFontHeight()*2));
			uView.print(75+counter+0.02);
			
			counter+=0.01;
			uView.display();
			mSec=millis()+100;
		}
	}
	uView.clear(PAGE);
}


