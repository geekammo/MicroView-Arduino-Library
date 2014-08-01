#include <MicroView.h>		// include MicroView library

MicroViewWidget *widget;	// declare widget pointer

int sensorPin = A2;		// select the input pin for the photo resistor
int sensorValue = 0;	// variable to store the value coming from the sensor

void setup() {
	pinMode(sensorPin,INPUT);		// set sensor pin as INPUT
	digitalWrite(sensorPin,HIGH);	// set Internal pull-up
	uView.begin();					// start MicrView
	uView.clear(PAGE);				// clear page
	widget = new MicroViewGauge(32,24,0,1023,WIDGETSTYLE1);	// set widget as gauge STYLE1
}

void loop() {
	sensorValue= analogRead(sensorPin); 	// read value from sensorPin
	widget->setValue(sensorValue);			// set the sensorValue to the gauge widget
	uView.display();						// display the widget
}
