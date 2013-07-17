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

#define ENCODER0_TIMEOUT_MS	700

#include "Encoder.h"

Encoder encoder0 (encoder0PinA, encoder0PinB, encoder0ButtonPin);

Encoder encoder1 (encoder1PinA, encoder1PinB, encoder1ButtonPin);

Encoder encoder2 (encoder2PinA, encoder2PinB, encoder2ButtonPin);

// Polling state variables: keep track of previous state for change detection

void setup ()
{
	#ifdef SERIAL_OUTPUT_EN
	// Establish Serial link for text output
	Serial.begin(115200);
	#endif

	// Emulate a Keyboard for application control
	Keyboard.begin();
}

/* 
 * The Arduino Pro Micro (for which this was written) does not have enough
 * hardware interrupt pins for all three encoders and their buttons. So,
 * the loop polls each encoder and its button. This is the only task
 * of the Arduino, so it should be plenty fast.
 */
bool altIsDown = false;

void loop ()
{
	// Taking the place of an interrupt, we manually
	// check for change in each of the pinA pins.
	encoder0.poll();
	encoder1.poll();
	encoder2.poll();

	if (((millis() - encoder0.getLastUpdateTime()) > ENCODER0_TIMEOUT_MS) && altIsDown) {
		Keyboard.release(KEY_LEFT_ALT);
		#ifdef SERIAL_OUTPUT_EN
			Serial.println("Time expired: ALT released");
		#endif
		altIsDown = false;
	}

	// Track movement
	if (encoder0.getDistanceMoved() != 0) {
		long delta = encoder0.getDistanceMoved();		
		
		if (altIsDown) {
			if (delta > 0) {
				// We are still holding ALT
				Keyboard.write(KEY_TAB);
				#ifdef SERIAL_OUTPUT_EN
					Serial.println("TAB");
				#endif
				delay(10);
			} else {
				Keyboard.press(KEY_LEFT_SHIFT);
				Keyboard.write(KEY_TAB);
				#ifdef SERIAL_OUTPUT_EN
					Serial.println("SHIFT+TAB");
				#endif
				delay(10);
				Keyboard.release(KEY_LEFT_SHIFT);
			}
		} else { // time window expired; a new sequence has to start
			// Start the sequence again
			Keyboard.press(KEY_LEFT_ALT);
			#ifdef SERIAL_OUTPUT_EN
				Serial.println("Enc0 new movement: ALT pressed");
			#endif
			altIsDown = true;

			if (delta > 0) {
				Keyboard.write(KEY_TAB);
				#ifdef SERIAL_OUTPUT_EN
					Serial.println("TAB");
				#endif
				delay(10);
			} else {
				Keyboard.press(KEY_LEFT_SHIFT);
				Keyboard.write(KEY_TAB);
				#ifdef SERIAL_OUTPUT_EN
					Serial.println("SHIFT+TAB");
				#endif
				delay(10);
				Keyboard.release(KEY_LEFT_SHIFT);
			}
		}

		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc0 Pos: ");
		Serial.println( encoder0.getPosition() );
		#endif
	}
	if (encoder1.getDistanceMoved() != 0) {
		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc1 Pos: ");
		Serial.println( encoder1.getPosition() );
		#endif
	}
	if (encoder2.getDistanceMoved() != 0) {
		#ifdef SERIAL_OUTPUT_EN
		Serial.print("Enc2 Pos: ");
		Serial.println( encoder2.getPosition() );
		#endif
	}

	#ifdef SERIAL_OUTPUT_EN
	if (encoder0.buttonIsBeingPressed()) {
		Serial.println("Button 0 pressed");
	}
	if (encoder1.buttonIsBeingPressed()) {	
		Serial.println("Button 1 pressed");
	}
	if (encoder2.buttonIsBeingPressed()) {
		Serial.println("Button 2 pressed");
	}
	#endif
}
