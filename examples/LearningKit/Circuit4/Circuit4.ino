#include <MicroView.h>		// include MicroView library

int buttonPin = A0;     	// push button pin
int buttonState = 0;		// variable to store the pushbutton status

void setup() {
	uView.begin();			// start MicroView
	uView.clear(PAGE);		// clear page
	
	pinMode(buttonPin, INPUT);  	// initialize the pushbutton pin as an input
	digitalWrite(buttonPin,HIGH);  	// set Internal pull-up
}

void loop() {
	buttonState = digitalRead(buttonPin);	// read the state of the pushbutton value

	// check if the pushbutton is pressed.
	// if it is not pressed, the buttonState is HIGH:
	if (buttonState == HIGH) {     
		uView.setCursor(0,0);		// set cursor at 0,0
		uView.print("OFF");			// print OFF
		uView.display();
 	} 
	else {
		uView.setCursor(0,0);
		uView.print("ON ");
		uView.display();
  }
}