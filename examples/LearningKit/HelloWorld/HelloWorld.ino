#include <MicroView.h>

void setup() {
	uView.begin();				// start MicroView
	uView.clear(PAGE);			// clear page
	uView.print("HelloWorld");	// display HelloWorld
	uView.display();
}

void loop () {}