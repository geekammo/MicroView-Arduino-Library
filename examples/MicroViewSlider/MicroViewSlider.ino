/*
	MicroView Arduino Library
	Copyright (C) 2014 GeekAmmo

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <MicroView.h>

MicroViewWidget *widget[4];		// declaring an array of 4 MicroViewWidget

void setup() {
	uView.begin();		// init and start MicroView
	uView.clear(PAGE);	// erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
	widget[0] = new MicroViewSlider(0,0,0,100);		// declare widget0 as a Slider at x=0, y=0, min=0, max=100
	widget[1] = new MicroViewSlider(0,10,0,150);	// declare widget0 as a Slider at x=0, y=10, min=0, max=150
	widget[2] = new MicroViewSlider(0,20,0,50);		// declare widget0 as a Slider at x=0, y=20, min=0, max=50
	widget[3] = new MicroViewSlider(0,30,0,200);	// declare widget0 as a Slider at x=0, y=30, min=0, max=200
}

void loop() {
	for (int i=0;i<=100;i++) {
		widget[0]->setValue(i);		// set value i to widget0
		widget[1]->setValue(100-i);
		widget[2]->setValue(i);
		widget[3]->setValue(100-i);
		uView.display();
	}

	for(int i=100; i>=0;i--) {
		widget[0]->setValue(i);
		widget[1]->setValue(100-i);
		widget[2]->setValue(i);
		widget[3]->setValue(100-i);
		uView.display();
	}
}
