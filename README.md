# MicroView Arduino Library

Developed by [Geek Ammo Pty Ltd](http://www.geekammo.com) based on Arduino and other Open Source libraries.  

## Description

Arduino library for MicroView.  

## Required Libraries

1. [Time.h](http://www.pjrc.com/teensy/td_libs_Time.html) NOTE: Only required when using clock/time functions.  For example the MicroViewDemo in the example folder.

## Installation

1. Extract / Check out to Arduino's libraries folder.
2. Start Arduino IDE.
3. MicroView example is located at, File--->Example--->MicroView--->MicroViewDemo

### Example 1
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

### Example 2
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

### Example 3
<pre><code>
#include &lt;MicroView.h&gt;

MicroViewWidget *widget;

void setup() {
	uView.begin();
	widget = new MicroViewSlider(0,0,0,100);	// draw Slider widget at x=0,y=0,min=0, max=100
}

void loop() {
	for(int i=0; i<=100;i++) {
		widget->setValue(i);	// give a value to widget
		uView.display();		// display current page buffer
	}
}
</code></pre>

## History
**v1.06b:  by JP Liew**
* fixed Slider negative value not working

**v1.05b: 6th February by JP Liew**
* changed MICROVIEW class name to MicroView
* created MICROVIEWWIDGET class
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