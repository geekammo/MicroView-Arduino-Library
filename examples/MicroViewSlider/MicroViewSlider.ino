#include <SPI.h>
#include <MicroView.h>

MicroViewWidget *widget[4];

void setup() {

	widget[0] = new MicroViewSlider(0,0,0,100);
	widget[1] = new MicroViewSlider(0,10,0,150);
	widget[2] = new MicroViewSlider(0,20,0,50);
	widget[3] = new MicroViewSlider(0,30,0,200);
	Serial.begin(115200);
	Serial.println("start");
	uView.begin();
	uView.clear(PAGE);
	widget[0]->draw();
	widget[1]->draw();
	widget[2]->draw();
	widget[3]->draw();
}

void loop() {
	for (int i=0;i<101;i++) {
		widget[0]->setValue(i);
		widget[1]->setValue(100-i);
		widget[2]->setValue(i);
		widget[3]->setValue(100-i);
		uView.display();
	}
	
	for(int i=100; i>-1;i--) {
		widget[0]->setValue(i);
		widget[1]->setValue(100-i);
		widget[2]->setValue(i);
		widget[3]->setValue(100-i);
		uView.display();
	}
	
}
