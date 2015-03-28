/*
  MicroView Arduino Library
  Copyright (C) 2015 GeekAmmo

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

/*
  An example of using the optional second argument of the widget
  setVal() function to update a widget without drawing the changes
  to the screen buffer.

  This, along with the widget reDraw() function, allows widgets to be
  maintained in the "background" while other things are being displayed.

  In this example,
  a guage and a slider are both continually updated with the same
  incrementing value. The widgets are alternately switched between the
  "background" and being displayed, at random intervals.
*/

#include <MicroView.h>

const int highVal = 100; // high value for widget range

int wValue = 0; // value for widgets

MicroViewWidget *guage1, *slider1;
boolean guage1On, slider1On; // true if widget is being displayed

void setup() {
  uView.begin();

  guage1 = new MicroViewGauge(32, 20, 0, highVal);
  guage1On = false; // begin with guage1 off screen
  uView.clear(PAGE);
  
  slider1 = new MicroViewSlider(8, 16, 0, highVal);
  slider1On = true; // begin with slider1 on screen

  // Init the random number generator using an
  // unconnected analog pin.
  randomSeed(analogRead(A0));
}

void loop() {
  // Display a widget for a random number of value increments
  for (int c = random(5, 16); c > 0; c--) {
    uView.display();
    delay(500);

    wValue++;
    if (wValue > highVal) {
      wValue = 0;
    }

    // If the second setValue() argument is false, then
    // the display buffer is not updated.
    //
    // In a practical application, the widgets would be set with
    // the values that are being monitored.
    guage1->setValue(wValue, guage1On);
    slider1->setValue(wValue, slider1On);
  }

  // Switch which widget is being displayed
  uView.clear(PAGE);

  guage1On = !guage1On;
  slider1On = !slider1On;

  if (guage1On) {
    guage1->reDraw();
  }

  if (slider1On) {
    slider1->reDraw();
  }
}
