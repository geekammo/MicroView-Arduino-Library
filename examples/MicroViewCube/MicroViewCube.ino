/******************************************************************************
MicroViewCube.ino
Rotating a 3-D Cube on the MicroView Display
Jim Lindblom @ SparkFun Electronics
Original Creation Date: June 9, 2014

This sketch uses the MicroView library to draw a 3-D projected
cube, and rotate it along all three axes.

Development environment specifics:
	IDE: Arduino 1.0.5
	Hardware Platform: MicroView

This code is beerware; if you see me (or any other SparkFun employee) at the
local, and you've found our code helpful, please buy us a round!

Distributed as-is; no warranty is given.
******************************************************************************/
#include <MicroView.h>

#define SHAPE_SIZE 600
#define ROTATION_SPEED 0 // ms delay between cube draws

int SCREEN_WIDTH = uView.getLCDWidth();
int SCREEN_HEIGHT = uView.getLCDHeight();

float d = 3;
float px[] = { -d,  d,  d, -d, -d,  d,  d, -d };
float py[] = { -d, -d,  d,  d, -d, -d,  d,  d };
float pz[] = { -d, -d, -d, -d,  d,  d,  d,  d };

float p2x[] = {0,0,0,0,0,0,0,0};
float p2y[] = {0,0,0,0,0,0,0,0};

float r[] = {0,0,0};

void setup()
{
	uView.begin();
	uView.clear(ALL);
	uView.display();  
}

void loop()
{
	drawCube();
	delay(ROTATION_SPEED);
}

void drawCube()
{
	r[0]=r[0]+PI/180.0; // Add a degree
	r[1]=r[1]+PI/180.0; // Add a degree
	r[2]=r[2]+PI/180.0; // Add a degree
	if (r[0] >= 360.0*PI/180.0) r[0] = 0;
	if (r[1] >= 360.0*PI/180.0) r[1] = 0;
	if (r[2] >= 360.0*PI/180.0) r[2] = 0;

	for (int i=0;i<8;i++)
	{
		float px2 = px[i];
		float py2 = cos(r[0])*py[i] - sin(r[0])*pz[i];
		float pz2 = sin(r[0])*py[i] + cos(r[0])*pz[i];

		float px3 = cos(r[1])*px2 + sin(r[1])*pz2;
		float py3 = py2;
		float pz3 = -sin(r[1])*px2 + cos(r[1])*pz2;

		float ax = cos(r[2])*px3 - sin(r[2])*py3;
		float ay = sin(r[2])*px3 + cos(r[2])*py3;
		float az = pz3-150;
		
		p2x[i] = SCREEN_WIDTH/2+ax*SHAPE_SIZE/az;
		p2y[i] = SCREEN_HEIGHT/2+ay*SHAPE_SIZE/az;
	}

	uView.clear(PAGE);
	for (int i=0;i<3;i++) 
	{
		uView.line(p2x[i],p2y[i],p2x[i+1],p2y[i+1]);
		uView.line(p2x[i+4],p2y[i+4],p2x[i+5],p2y[i+5]);
		uView.line(p2x[i],p2y[i],p2x[i+4],p2y[i+4]);
	}    
	uView.line(p2x[3],p2y[3],p2x[0],p2y[0]);
	uView.line(p2x[7],p2y[7],p2x[4],p2y[4]);
	uView.line(p2x[3],p2y[3],p2x[7],p2y[7]);
	uView.display();
}