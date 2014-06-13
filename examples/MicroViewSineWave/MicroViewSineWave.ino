/*
	MicroView Sine Wave Example
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

void setup() {
  uView.begin();
  uView.clear(PAGE);
}

void loop () {
  int i;
  float rad,srad, x,y;
  int amp=20;
  int nPhase=360;

  for(i=0; i<nPhase;i++) {
    x=i;
    x=x*0.017453;
    y= sin(x);
    y=y*amp;  // amplitude
    y=y+24;
    x=(double)64/(double)nPhase;
    x=x*i;
    uView.pixel(x, y);
    uView.display();
  }

  amp=10;
  nPhase=3*360;
  
  for(i=0; i<nPhase;i++) {
    x=i;
    x=x*0.017453;
    y= cos(x);
    y=y*amp;  // amplitude
    y=y+24;
    x=(double)64/(double)nPhase;
    x=x*i;
    uView.pixel(x, y);
  }

  uView.display();
  delay(1500);
  uView.clear(PAGE);
}
