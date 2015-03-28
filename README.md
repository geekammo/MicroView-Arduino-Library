# MicroView Arduino Library

Developed by [Geek Ammo Pty Ltd](http://www.geekammo.com) based on Arduino and other Open Source libraries.  

## Description

Arduino library for MicroView.  

## Required Libraries

1. [Time.h](http://www.pjrc.com/teensy/td_libs_Time.html) NOTE: Only required when using clock/time functions.  For example the MicroViewDemo in the example folder.

## Installation

1. Change directory to Arduino's main directory
2. cd libraries
3. mkdir MicroView
4. cd MicroView
5. git clone https://github.com/geekammo/MicroView-Arduino-Library.git .
6. Start Arduino IDE.
7. MicroView example is located at, File--->Example--->MicroView--->MicroViewDemo

### Example 1 - Hello World!
<pre><code>
#include &lt;MicroView.h&gt;

void setup() {
    uView.begin();
}

void loop() {
    uView.print("HelloWorld");
    uView.display();		// display current page buffer
}
</code></pre>

### Example 2 - Basic Drawing
<pre><code>
#include &lt;MicroView.h&gt;

void setup() {
    uView.begin();
    uView.clear(PAGE);		// clear the page buffer
}

void loop() {
    uView.line(0,0,64,48);
    uView.circle(32,24,10);
    uView.rect(10,10,20,20);
    uView.pixel(50,5);
    uView.setCursor(0,40);
    uView.print(" MicroView");
    uView.display();		// display current page buffer
}
</code></pre>

### Example 3 - Widgets
<pre><code>
#include &lt;MicroView.h&gt;

MicroViewWidget *widget,*widget2;

void setup() {
    uView.begin();
    uView.clear(PAGE);
    widget= new MicroViewGauge(32,30,0,100);  // draw Gauge widget at x=32,y=30,min=0, max=100
    widget2= new MicroViewSlider(0,0,0,100);  // draw Slider widget at x=0,y=0,min=0, max=100
}

void loop() {
    for(int i=0; i&lt;=100;i++) {
        widget->setValue(i);	// give a value to widget
        widget2->setValue(i);
        uView.display();		// display current page buffer
    }
}
</code></pre>

### Example 4 - Communication
<pre><code>
#include &lt;MicroView.h&gt;

void setup() {
    uView.begin();
    uView.clear(PAGE);
	Serial.begin(115200);	// user decide the baud rate
}

void loop() {
    uView.checkComm();
}
</code></pre>

## History
**v1.23b: 29th March 2015 by Scott Allen**
* added overloaded setValue() function to add a boolean argument doDraw which controls whether the display buffer is updated.

**v1.22b: 4th October 2014 by Scott Allen and Ben Lewis**
* replaced circleFill() with a faster algorithm, better looking circle - Ben Lewis
* replaced sprintf with dedicated code, reduced program size - Scott Allen
* added getValLen method for widgets to return length of current value - Scott Allen
* initialised widget with minimum value instead of 0 - Scott Allen
* added getValLen and getMaxValLen keywords
* modified MicroViewDemo example to use getValLen method - Scott Allen

**v1.21b: 22nd September 2014 by Esmit Pérez and Scott Allen**
* re-factored code if/else with switch - Esmit Pérez
* simplified draw() to remove redundant code - Esmit Pérez
* added WIDGETNOVALUE to widgets - Scott 
* fixed compiler warning - Scott
* improved gauge minor ticks - Scott
* added destructor for MicroViewWidget base class - Scott

**v1.20b: 27th August 2014 by Scott Allen, Emil Ong**
* added Flashing Heart Example - JP
* added getScreenBuffer() for access to screen RAM - Scott
* added keywords  for existing functions - Scott
* fixed circleFill() - Emil

**v1.19b: 19th August 2014 by Scott Allen**
* added uView.scrollLeft() function
* simplified Silder Widget drawFace() function
* added full signed 16 bit ranges for widgets
* improved drawing for minor and major ticks

**v1.18b: 5th August 2014 by Scott Allen**
* fixed compiler warning in MicroViewSlider
* changed vertical slider direction.

**v1.17b: 4th August 2014 by JP Liew**
* added reDraw() for MicroViewWidget class
* removed Serial.begin() from uView.begin() so that user can have control
* added MicroViewWidgetRedraw example

**v1.16b: 3rd August 2014 by czetie**
* added vertical slider widget

**v1.15b: 3rd August 2014 by Scott Allen**
* improved lots of low level routines, Fast SPI
* fixed some compilation warnings
* reduced overdriving display inputs
* added uView.end() to power off the display
* improved speed of display() and clear() functions
* fixed positionning of "3" on clock face

**v1.14b: 26th July 2014 by JP Liew**
* added Learning Kit Circuit Sketch

**v1.13b: 13th June 2014 by JP Liew**
* added Sine Wave Example
* inserted license to example code

**v1.12b: 11th June 2014 by JP Liew**
* added comments for SparkFun Logo
* added Rotating Cube example by Jim Lindblom @ SparkFun Electronics
* changed git clone instruction to use https

**v1.11b: 9th June 2014 by JP Liew**
* added simple tutorials for production sketch
* modified OLED RESET pin to 7
* added SparkFun Logo

**v1.10b: 22th April 2014 by JP Liew**
* changed SS, RESET, DC pins to use weak internal pull-up resistors

**v1.09b: 17th April 2014 by JP Liew**
* changed verticalFlip() to flipVertical() and horizontalFlip() to flipHorizontal() for consistency
* added debug messages for doCmd()

**v1.08b: 18th February 2014 by JP Liew**
* added verticalFlip(), horizontalFlip()

**v1.07b: 15th February 2014 by JP Liew**
* changed function name stopScroll to scrollStop for consistency
* added contrast function
* added invert function
* added KEYWORD to keywords.txt
* added checkComm() function to communicate with host PC

**v1.06b: 9th February 2014 by JP Liew**
* fixed Slider negative value not working
* added round Gauge widget
* changed Example 3 to show round Gauge

**v1.05b: 6th February 2014 by JP Liew**
* changed MICROVIEW class name to MicroView
* created MicroViewWidget class
* added routines to draw widget
* added slider widget
* merged MicroViewWidget into MicroView
* merged SPI.h into MicroView 

**v1.04b: 3rd February 2014 by JP Liew**
* declared permanent uView variable.
* cleaned code and added some remarks.
* added drawing functions that make use of default color and draw mode.
* added example in README.md

**v1.03b: 1st February 2014 by JP Liew**  
* added 7 segment number only font.

**v1.02b: 31th January 2014 by JP Liew**  
* added sprite animation demo.  

**v1.01b:	30th January 2014 by JP Liew**  
* fixed font draw XOR mode bug.  
* added analog clock demo.

**v1.00b:	30th January 2014 by JP Liew**  
* Initial commit.  Beta with minor bugs.

## License
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
