#include <MicroView.h>
/*
	MicroView Blink
	Draw a circle for one second, then off for one second, repeatedly.
 
	This example code is in the public domain.
 */
 
// the setup routine runs once when you press reset:
void setup() {                
	uView.begin();
	uView.clear(PAGE);
}

// the loop routine runs over and over again forever:
void loop() {
	uView.circleFill(32,24,10,WHITE,NORM);
	uView.display();
	delay(1000);               // wait for a second

	uView.circleFill(32,24,10,BLACK,NORM);  
	uView.display();
	delay(1000);               // wait for a second
}