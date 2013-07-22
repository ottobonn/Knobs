#include "Encoder.h"

// constructor : sets pins as inputs

Encoder::Encoder( uint8_t PinA, uint8_t PinB, uint8_t ButtonPin ) : pin_a( PinA ), pin_b( PinB ), button_pin( ButtonPin ) {
    pinMode(pin_a, INPUT);
    pinMode(pin_b, INPUT);
	pinMode(button_pin, INPUT);

	pinAPreviousVal = digitalRead(pin_a);
	position = 0;
};


long Encoder::getDistanceMoved () {
	return position - previousPosition;
}

void Encoder::poll () {
	bool pinAVal = digitalRead(pin_a);
	previousPosition = position;

	if ( pinAVal != pinAPreviousVal ) {
		update();
	}
	pinAPreviousVal = pinAVal;

	bool buttonVal = digitalRead(button_pin);
	buttonPressed = (buttonVal == false && buttonPreviousVal == true) ? true : false;
	buttonPreviousVal = buttonVal;
}

long Encoder::getLastUpdateTime () {
	return lastUpdateTime;
}


// call this from your interrupt function
// or call poll() from the main loop

void Encoder::update () {
    if (digitalRead(pin_a)) digitalRead(pin_b) ? position-- : position++;
    else digitalRead(pin_b) ? position++ : position--;
	lastUpdateTime = millis();
};

bool Encoder::buttonIsBeingPressed () {
	return buttonPressed;
}

// returns current position

long int Encoder::getPosition () { return position; };

// set the position value

void Encoder::setPosition ( const long int p) { position = p; };
