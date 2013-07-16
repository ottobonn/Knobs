#define encoder0PinB		3
#define encoder0PinA		4
#define encoder0ButtonPin	15

#define encoder1PinB		5
#define encoder1PinA		6
#define encoder1ButtonPin	14

#define encoder2PinB		7
#define encoder2PinA		8
#define encoder2ButtonPin	16

#define SERIAL_OUTPUT_EN

#include "Encoder.h"

Encoder encoder0 (encoder0PinA, encoder0PinB);

Encoder encoder1 (encoder1PinA, encoder1PinB);

Encoder encoder2 (encoder2PinA, encoder2PinB);

// Polling state variables: keep track of previous state for change detection

bool encoder0PinAPreviousVal,
	 encoder1PinAPreviousVal,
	 encoder2PinAPreviousVal = 0;

void setup ()
{
	#ifdef SERIAL_OUTPUT_EN
	// Establish Serial link for text output
	Serial.begin(115200);
	#endif

	// Emulate a Keyboard for application control
	Keyboard.begin();

	// Read current pinA values, so encoders don't
	// send spurious movement on startup
	encoder0PinAPreviousVal = digitalRead (encoder0PinA);
	encoder1PinAPreviousVal = digitalRead (encoder1PinA);
	encoder2PinAPreviousVal = digitalRead (encoder2PinA);
}

/* 
 * The Arduino Pro Micro (for which this was written) does not have enough
 * hardware interrupt pins for all three encoders and their buttons. So,
 * the loop polls each encoder and its button. This is the only task
 * of the Arduino, so it should be plenty fast.
 */

long encoder0PreviousPos,
	 encoder1PreviousPos,
	 encoder2PreviousPos = 0;

bool encoder0ButtonPreviousVal,
	 encoder1ButtonPreviousVal,
	 encoder2ButtonPreviousVal = true;

void loop ()
{
	// Taking the place of an interrupt, we manually
	// check for change in each of the pinA pins.
	bool encoder0PinAVal = digitalRead (encoder0PinA);
	bool encoder1PinAVal = digitalRead (encoder1PinA);
	bool encoder2PinAVal = digitalRead (encoder2PinA);

	if ( encoder0PinAVal != encoder0PinAPreviousVal ) {
		encoder0.update();
		encoder0PinAPreviousVal = encoder0PinAVal;
	}
	if ( encoder1PinAVal != encoder1PinAPreviousVal ) {
		encoder1.update();
		encoder1PinAPreviousVal = encoder1PinAVal;
	}
	if ( encoder2PinAVal != encoder2PinAPreviousVal ) {
		encoder2.update();
		encoder2PinAPreviousVal = encoder2PinAVal;
	}

	// Get a local copy of the encoder position
	long encoder0Pos, encoder1Pos, encoder2Pos;
	encoder0Pos = encoder0.getPosition ();
	encoder1Pos = encoder1.getPosition ();
	encoder2Pos = encoder2.getPosition ();

	// Track movement based on previous positions

	if (encoder0Pos != encoder0PreviousPos) {
		long delta = encoder0Pos - encoder0PreviousPos;

		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Delta 0: ");
		Serial.println(delta);	
		#endif

		if (delta > 0)
			sendAltTab(1); // For now, only roll back and forth between two recent windows. Later, add more tabs based on wheel speed
		else if (delta < 0)
			sendShiftAltTab(1);

		encoder0PreviousPos = encoder0Pos;

		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc0 Pos: ");
		Serial.println( encoder0Pos );
		#endif

	}
	if (encoder1Pos != encoder1PreviousPos) {
		encoder1PreviousPos = encoder1Pos;
		
		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc1 Pos: ");
		Serial.println( encoder1Pos );
		#endif

	}
	if (encoder2Pos != encoder2PreviousPos) {
		encoder2PreviousPos = encoder2Pos;

		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc2 Pos: ");
		Serial.println( encoder2Pos );
		#endif
	}

	// Now poll the axial pushbuttons ------------------------
	bool encoder0ButtonVal = digitalRead (encoder0ButtonPin);
	bool encoder1ButtonVal = digitalRead (encoder1ButtonPin);
	bool encoder2ButtonVal = digitalRead (encoder2ButtonPin);

	// Only trigger a button press event on the falling signal edge
	// (buttons cause short to ground)
	bool encoder0ButtonPress = ( encoder0ButtonVal == false && encoder0ButtonPreviousVal == true ) ? true : false;
    bool encoder1ButtonPress = ( encoder1ButtonVal == false && encoder1ButtonPreviousVal == true ) ? true : false;
	bool encoder2ButtonPress = ( encoder2ButtonVal == false && encoder2ButtonPreviousVal == true ) ? true : false;

	// Store the current button values for next round of the loop
	encoder0ButtonPreviousVal = encoder0ButtonVal;
	encoder1ButtonPreviousVal = encoder1ButtonVal;
	encoder2ButtonPreviousVal = encoder2ButtonVal;

	#ifdef SERIAL_OUTPUT_EN
	if (encoder0ButtonPress) {
		Serial.println("Button 0 pressed");
	}
	if (encoder1ButtonPress) {	
		Serial.println("Button 1 pressed");
	}
	if (encoder2ButtonPress) {
		Serial.println("Button 2 pressed");
	}
	#endif
}

void sendAltTab (int tabRepeat)
{
	
	Keyboard.press(KEY_LEFT_ALT);
	for (int i = 0; i < tabRepeat; i++){
		Keyboard.write(KEY_TAB);
		delay(10);
	}
	Keyboard.release(KEY_LEFT_ALT);
}

void sendShiftAltTab (int tabRepeat)
{
	Keyboard.press(KEY_LEFT_SHIFT);
	sendAltTab(tabRepeat);
	Keyboard.release(KEY_LEFT_SHIFT);
}
