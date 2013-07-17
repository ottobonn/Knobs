/*
 * Adapted from code by mikkoh (2010), found on
 * http://playground.arduino.cc/Main/RotaryEncoders#Example1
 *
 * The class defines a quadrature rotary encoder and
 * an interrupt to update its position.
 *
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include <Arduino.h> // New version of WProgram.h (as of v1.0)

class Encoder {
  /*  
    NOTE : Client must call the encoder's update method from an
    interrupt function, i.e. client must attach an interrupt to the
    encoder pin A and call the encoder update method from within the 
    interrupt.

    // ------------------------------------------------------------------------------------------------
    // Example usage :
    // ------------------------------------------------------------------------------------------------
        #include "Encoder.h"

        Encoder encoder(2, 4);

        void setup() { 
            attachInterrupt(0, doEncoder, CHANGE); 
            Serial.begin (115200);
            Serial.println("start");
        } 

        void loop(){
            // do some stuff here - the joy of interrupts is that they take care of themselves
        }

        void doEncoder(){
            encoder.update();
            Serial.println( encoder.getPosition() );
        }    
    // ------------------------------------------------------------------------------------------------
    // Example usage end
    // ------------------------------------------------------------------------------------------------
  */
public:

    /* 
	 * Constructor : sets pins as inputs and
	 * registers the encoder's starting position.
	 */
    Encoder( uint8_t PinA, uint8_t PinB, uint8_t ButtonPin );

	/* Function: poll
	 * --------------
	 * Check pin A for changes, and call update() if needed.
	 * Call poll() every time through the main loop if you
	 * don't have interrupts available. The faster you call
	 * poll(), the more likely you are to catch all encoder
	 * movements.
	 */
	void poll();

	/* Function: getDistanceMoved
	 * --------------------------
	 * Return the distance this encoder has moved since the
	 * last call to poll(). You can use this as a 
	 * check for movement, e.g. if(encoder.getDistanceMoved()) 
	 * will be true when the encoder has moved. No need to do a 
	 * check seperately.
	 *
	 * Note that it doesn't make sense to call this if you are
	 * using interrupts: if the encoder only updates position
	 * when it moves in real life, it will always have moved since
	 * the last call to update(). The function is for use with polling,
	 * where the encoder may or may not have moved since the last poll.
	 * If you are using interrupts, call getPosition() directly.
	 */
	long getDistanceMoved();

    // call this from your interrupt function
	// or from a polling routine, if interrupts are not available.
    void update ();

	long getLastUpdateTime ();

    // returns current position

    long int getPosition ();

    // set the position value

    void setPosition ( const long int p);

	bool buttonIsBeingPressed ();

private:

    long int position, previousPosition;

    uint8_t pin_a;

    uint8_t pin_b;

	uint8_t button_pin;

	bool pinAPreviousVal;

	bool buttonPreviousVal, buttonPressed;

	long int lastUpdateTime;
};

#endif // __ENCODER_H__
