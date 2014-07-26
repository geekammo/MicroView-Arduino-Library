#include <MicroView.h>		// include MicroView library

MicroViewWidget *redWidget, *greenWidget, *blueWidget;	// declare 3 widget pointers

int RED = 6;			// declare RED LED pin 6
int GREEN = 5;			// declare GREEN LED pin 5
int BLUE = 3;			// declare BLUE LED pin 3
int fadeStep = 10;		// declare fading steps
int dly=20;				// declare delay

void setup()
{
	uView.begin();									// start MicroView
	uView.clear(PAGE);								// clear page
	redWidget = new MicroViewSlider(0,0,0,255);		// declare RED widget as slider
	greenWidget = new MicroViewSlider(0,10,0,255);	// declare GREEN widget as slider
	blueWidget = new MicroViewSlider(0,20,0,255);	// declare BLUE widget as slider
	
	pinMode(RED, OUTPUT);							// set RED LED pin as OUTPUT
	pinMode(GREEN, OUTPUT);							// set GREEN LED pin as OUTPUT
	pinMode(BLUE, OUTPUT);							// set BLUE LED pin as OUTPUT
}

void loop()
{
	int i;											// init i variable for general use
	// brightening
	for (i=0;i<=255;i+=fadeStep) {					// step i from 0 to 255 by fadeSteps
		redWidget->setValue(i);						// set brightness value for RED LED to widget
		uView.display();							// display the content of the screen buffer
		analogWrite(RED,i);							// set brightness value for RED LED to the pin
		delay(dly);
	}
	// dimming
	for (i=255;i>=0;i-=fadeStep) {					// step i from 255 to 0 by fadeSteps
		redWidget->setValue(i);
		uView.display();
		analogWrite(RED,i);
		delay(dly);
	}

	// brightening
	for (i=0;i<=255;i+=fadeStep) {
		greenWidget->setValue(i);
		uView.display();
		analogWrite(GREEN,i);
		delay(dly);
	}
	// dimming
	for (i=255;i>=0;i-=fadeStep) {
		greenWidget->setValue(i);
		uView.display();
		analogWrite(GREEN,i);
		delay(dly);
	}

	// brightening
	for (i=0;i<256;i+=fadeStep) {
		blueWidget->setValue(i);
		uView.display();
		analogWrite(BLUE,i);
		delay(dly);
	}
	// dimming
	for (i=255;i>=0;i-=fadeStep) {
		blueWidget->setValue(i);
		uView.display();
		analogWrite(BLUE,i);
		delay(dly);
	}
}