#include "Encoder.h"

// constructor : sets pins as inputs

Encoder::Encoder( uint8_t PinA, uint8_t PinB ) {// : pin_a ( PinA ), pin_b( PinB ) {
	pin_a = PinA;
	pin_b = PinB;
    // set pin a and b to be input
    pinMode(pin_a, INPUT);
    pinMode(pin_b, INPUT);
	position = 0;
};

// call this from your interrupt function
// or from a polling routine,
// explicitly checking corresponding pinA for change.

void Encoder::update () {
    if (digitalRead(pin_a)) digitalRead(pin_b) ? position-- : position++;
    else digitalRead(pin_b) ? position++ : position--;
};

// returns current position

long int Encoder::getPosition () { return position; };

// set the position value

void Encoder::setPosition ( const long int p) { position = p; };
