#include <MicroView.h>		// include MicroView library
#include <Servo.h>			// include Servo library

Servo servo;				// declare servo object

void setup()
{
	uView.begin();			// start MicroView 
	uView.clear(PAGE);		// clear page
	servo.attach(6);		// servo control pin at D6 	
}

void loop()
{
	uView.setCursor(0,0);	// set cursor to 0,0
	uView.print("Mid  ");	// display Mid
	uView.display();
	servo.write(90);	// about 90 degree
	delay(2000);		// delay 2 seconds

	uView.setCursor(0,0);
	uView.print("Left ");
	uView.display();
	servo.write(20);	//	about 20 degree
	delay(2000);

	uView.setCursor(0,0);
	uView.print("Mid  ");
	uView.display();
	servo.write(90);	// about 90 degree
	delay(2000);

	uView.setCursor(0,0);
	uView.print("Right");
	uView.display();
	servo.write(160);	// about 160 degree
	delay(2000);
}