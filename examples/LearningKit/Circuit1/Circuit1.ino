int LED = A3;				// declare LED as pin A3 of MicroView

void setup()
{
	pinMode(LED, OUTPUT);	// set LED pin as OUTPUT
}

void loop()
{
	digitalWrite(LED, HIGH);	// set LED pin HIGH voltage, LED will be on
	delay(1000);				// delay 1000 ms 
	digitalWrite(LED, LOW);		// set LED pin LOW voltage, LED will be off
	delay(1000);				// delay 1000 ms
}
