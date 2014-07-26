#include <MicroView.h>				// include MicroView library

MicroViewWidget  *widget;			// declare widget pointer

int sensorValue;					// declare variable to store sensor value
int sensorPin=A0;					// declare sensor pin as A0 of Arduino	

void setup() {
  uView.begin();					// start MicroView	
  uView.clear(PAGE);				// clear page
  widget = new MicroViewSlider(0,0,0,1024, WIDGETSTYLE1);	// declare widget as slider
  pinMode(sensorPin, INPUT);				// set sensor pin as INPUT
}

void loop () {
  sensorValue=analogRead(sensorPin);		// read and store sensor value
  widget->setValue(sensorValue);			// set sensor value to widget
  uView.display();							// display widget
  delay(20);								// delay 20 ms
}