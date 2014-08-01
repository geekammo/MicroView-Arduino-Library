#include <MicroView.h>		// include MicroView library

MicroViewWidget *widget;	// declare widget pointer

int sensorPin = A0;		// select the input pin for the temperature sensor
int sensorValue = 0;	// variable to store the value coming from the sensor

void setup() {
	pinMode(sensorPin,INPUT);	// set sensor pin as INPUT
	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	widget = new MicroViewGauge(32,24,0,255,WIDGETSTYLE1);		// declare as gauge widget
	uView.drawChar(47,33,67);	// Character C is ASCII code 67
}

void loop() {
	sensorValue= analogRead(sensorPin);				// read sensor pin value
	float voltage = sensorValue * 5.0;				// voltage at pin in volt
	voltage /= 1024.0; 								// voltage = sensorValue x (5/1024)
	float temperatureC = (voltage - 0.5) * 100 ; 	// C = (voltage - 0.5) x 100
	widget->setValue(temperatureC);					// set temperature value to the gauge
	uView.display();								// display gauge tick
}