#include <MicroView.h>
// adapted from SparkFun Inventor Kit https://www.sparkfun.com/products/12001
// Please download the original source code for detail comment of the source code
// http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Kits/SIK%20Guide%20Code.zip

const int buzzerPin = A0;
const int songLength = 18;
char notes[] = "cdfda ag cdfdg gf "; // a space represents a rest
int beats[] = {1,1,1,1,1,1,4,4,2,1,1,1,1,1,1,4,4,2};

int tempo = 150;

void setup() 
{
	uView.begin();
	uView.clear(PAGE);
	pinMode(buzzerPin, OUTPUT);
}

void loop() 
{
	int i, duration;

	for (i = 0; i < songLength; i++) // step through the song arrays
	{
		duration = beats[i] * tempo;  // length of note/rest in ms
		
		if (notes[i] == ' ')          // is this a rest? 
		{
			uView.print(" ");
			uView.display();
			delay(duration);            // then pause for a moment
		}
		else                          // otherwise, play the note
		{
			uView.print(notes[i]);
			uView.display();
			tone(buzzerPin, frequency(notes[i]), duration);
			delay(duration);            // wait for tone to finish
		}
		delay(tempo/10);              // brief pause between notes
	}

	// We only want to play the song once, so we'll pause forever:
	while(true){}
	// If you'd like your song to play over and over,
	// remove the above statement
}


int frequency(char note) 
{
	// This function takes a note character (a-g), and returns the
	// corresponding frequency in Hz for the tone() function.

	int i;
	const int numNotes = 8;  // number of notes we're storing

	// The following arrays hold the note characters and their
	// corresponding frequencies. The last "C" note is uppercase
	// to separate it from the first lowercase "c". If you want to
	// add more notes, you'll need to use unique characters.

	// For the "char" (character) type, we put single characters
	// in single quotes.

	char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
	int frequencies[] = {262, 294, 330, 349, 392, 440, 494, 523};

	// Now we'll search through the letters in the array, and if
	// we find it, we'll return the frequency for that note.

	for (i = 0; i < numNotes; i++)  // Step through the notes
	{
		if (names[i] == note)         // Is this the one?
		{
			return(frequencies[i]);     // Yes! Return the frequency
		}
	}
	return(0);  // We looked through everything and didn't find it,
	// but we still need to return a value, so return 0.
}