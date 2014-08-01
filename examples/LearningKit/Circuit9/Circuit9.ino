#include <MicroView.h>		// include MicroView library

int motorPIN = 3;			// set motor control pin
MicroViewWidget *widget;	// declare widget pointer

void setup() {                
	uView.begin();			// start MicroView
	uView.clear(PAGE);		// clear page
	pinMode(motorPIN, OUTPUT);		// initialize the digital pin as an output.
	widget = new MicroViewGauge(32,24,90,255,WIDGETSTYLE1);		// set widget as gauge STYLE1
	setPwmFrequency(motorPIN,1);	// set PWM frequency to about 31K
}

void loop() {
	for (int i=90;i<255;i+=10) {	// step i from 90 to 255 by step of 10
		widget->setValue(i);		// set i value to gauge
		uView.display();			// display gauge
		analogWrite(motorPIN, i);	// set the DUTY cycle of the motorPIN
		delay(500);    				// delay 500 ms
	}
}

// function to set the frequency of the PWM pin
// adapted from http://playground.arduino.cc/Code/PwmFrequency
void setPwmFrequency(int pin, int divisor) {
	byte mode;
	if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
		switch(divisor) {
		case 1: mode = 0x01; break;
		case 8: mode = 0x02; break;
		case 64: mode = 0x03; break;
		case 256: mode = 0x04; break;
		case 1024: mode = 0x05; break;
		default: return;
		}
		if(pin == 5 || pin == 6) {
			TCCR0B = TCCR0B & 0b11111000 | mode;
		} else {
			TCCR1B = TCCR1B & 0b11111000 | mode;
		}
	} else if(pin == 3 || pin == 11) {
		switch(divisor) {
		case 1: mode = 0x01; break;
		case 8: mode = 0x02; break;
		case 32: mode = 0x03; break;
		case 64: mode = 0x04; break;
		case 128: mode = 0x05; break;
		case 256: mode = 0x06; break;
		case 1024: mode = 0x7; break;
		default: return;
		}
		TCCR2B = TCCR2B & 0b11111000 | mode;
	}
}