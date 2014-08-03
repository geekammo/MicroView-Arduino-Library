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

MicroViewWidget *vWidget1, *vWidget2;

void setup() {
	uView.begin();
	uView.clear(PAGE);

	vWidget1 = new MicroViewSlider(0, 0, 0, 255, WIDGETSTYLE2);
	vWidget2 = new MicroViewSlider(31, 0, 0, 255, WIDGETSTYLE3);
}

void loop() {
	for (int i=0;i<=255;i++) {
		vWidget1->setValue(i);
		vWidget2->setValue(255-i);
		uView.display();
	}

	for(int i=255; i>=0;i--) {
		vWidget1->setValue(i);		
		vWidget2->setValue(255-i);
		uView.display();
	}
}

