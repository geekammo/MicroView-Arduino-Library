#include <MicroView.h>			// include MicroView library

int relayPin = 2;				// set relayPin as pin 2 of Arduino

void setup() {                
	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	pinMode(relayPin, OUTPUT);	// initialize the digital pin as an output.
}

void loop() {
	uView.setCursor(0,0);			// set cursor at 0,0
	uView.print("YELLOW");			// print YELLOW text
	uView.display();				// display 
	digitalWrite(relayPin, HIGH);   // turn the RELAY ON (HIGH is the voltage level)
	delay(1000);    				// wait for a second
	
	uView.setCursor(0,0);		
	uView.print("RED   ");
	uView.display();
	digitalWrite(relayPin, LOW);    // turn the RELAY off by making the voltage LOW
	delay(1000);               		// wait for a second
}