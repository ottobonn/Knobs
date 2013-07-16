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

    // constructor : sets pins as inputs

    Encoder( uint8_t PinA, uint8_t PinB );

    // call this from your interrupt function
	// or from a polling routine, if interrupts are not available.

    void update ();

    // returns current position

    long int getPosition ();

    // set the position value

    void setPosition ( const long int p);

private:

    long int position;

    uint8_t pin_a;

    uint8_t pin_b;
};

#endif // __ENCODER_H__
