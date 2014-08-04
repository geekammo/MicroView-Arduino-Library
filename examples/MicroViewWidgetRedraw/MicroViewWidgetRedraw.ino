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

MicroViewWidget *widget[2];		// declaring an array of 4 MicroViewWidget

void setup() {
	uView.begin();		// init and start MicroView
	widget[0] = new MicroViewSlider(0,0,0,255);		// declare widget0 as a Slider at x=0, y=0, min=0, max=100
	widget[1] = new MicroViewGauge(32,24,0,255,WIDGETSTYLE0);	// declare widget0 as a Slider at x=0, y=10, min=0, max=150
	uView.clear(PAGE);	// erase the memory buffer, when next uView.display() is called, the OLED will be cleared.
}

void loop() {

	widget[0]->reDraw();
	for (int i=0;i<=255;i++) {
		widget[0]->setValue(i);		// set value i to widget0
		uView.display();
	}

	delay(500);
	uView.clear(PAGE);

	widget[1]->reDraw();
	for (int i=0;i<=255;i++) {
		widget[1]->setValue(i);		// set value i to widget0
		uView.display();
	}

	delay(500);
	uView.clear(PAGE);
}
