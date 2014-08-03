#include <MicroView.h>

int buttonPin = A0;     // push button pin
int buttonState = 0;	// variable for reading the pushbutton status
int onCount = 0;
MicroViewWidget *hWidget, *vWidget1, *vWidget2;

void setup() {
	uView.begin();
	uView.clear(PAGE);
	
	// initialize the pushbutton pin as an input:
	pinMode(buttonPin, INPUT);  
	digitalWrite(buttonPin,HIGH);  
	
	//Create a slider to count how long the switch is on
//	hWidget = new MicroViewSlider(0, 0, 0, 99, WIDGETSTYLE0);
	vWidget1 = new MicroViewSlider(0, 0, 0, 255, WIDGETSTYLE2);
	vWidget2 = new MicroViewSlider(31, 0, 0, 255, WIDGETSTYLE3);
}

void loop() {
	// read the state of the pushbutton value:
	buttonState = digitalRead(buttonPin);

	// check if the pushbutton is pressed.
	// if it is not pressed, the buttonState is HIGH:
	if (buttonState == HIGH) {     
		onCount = min(onCount+1, 255);
//		hWidget->setValue(onCount);
		vWidget1->setValue(onCount);
		vWidget2->setValue(onCount);
//		uView.setCursor(0,0);
//		uView.print("OFF");
		uView.display();
 	} 
	else {
		onCount = max(onCount-1, 0);
//		hWidget->setValue(onCount);
		vWidget1->setValue(onCount);
		vWidget2->setValue(onCount);
//		uView.setCursor(0,0);
//		uView.print("ON ");
		uView.display();
  }
}

