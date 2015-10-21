///
/// @file		I2C_Software.ino
/// @brief		Main sketch
/// Project 	I2C_Software Library
///
/// @details	Software I2C Library
/// @n @a		Developed with [embedXcode+](http://embedXcode.weebly.com)
/// 
/// @author		Rei Vilo
/// @author		http://embeddedcomputing.weebly.com
///
/// @date		Feb 10, 2014 21:36
/// @version	Software I2C Library
/// 
/// @copyright	(c) Rei Vilo, 2013-2014
/// @copyright	All rights reserved
///
/// @see		ReadMe.txt for references
/// * HMC6352 2-Axis Compass
/// http://www.sparkfun.com/datasheets/Components/HMC6352.pdf
///

// Core library for code-sense
#if defined(WIRING) // Wiring specific
#include "Wiring.h"
#elif defined(MAPLE_IDE) // Maple specific
#include "WProgram.h"
#elif defined(MPIDE) // chipKIT specific
#include "WProgram.h"
#elif defined(DIGISPARK) // Digispark specific
#include "Arduino.h"
#elif defined(ENERGIA) // LaunchPad MSP430 G2 and F5529, Stellaris and Tiva, Experimeter Board FR5739 specific
#include "Energia.h"
#elif defined(MICRODUINO) // Microduino specific
#include "Arduino.h"
#elif defined(TEENSYDUINO) // Teensy specific
#include "Arduino.h"
#elif defined(ARDUINO) // Arduino 1.0 and 1.5 specific
#include "Arduino.h"
#else // error
#error Platform not defined
#endif

// Include application, user and local libraries

///
/// @brief	I2C_KIND
/// @{

#define I2C_HARDWARE 1 ///< 1=hardware
#define I2C_SOFTWARE 2 ///< 2=software

#define I2C_KIND I2C_SOFTWARE ///< Select mode

/// @}

#if (I2C_KIND == I2C_HARDWARE)

#include "Wire.h"

#else

#include "I2C_SoftwareLibrary.h"

///
/// @brief	SoftwareWire initialisation
/// @{
///
#define SCL_PIN P2_4 ///< pin for SCL
#define SDA_PIN P2_3 ///< pin for SDA
SoftwareWire Wire(SDA_PIN, SCL_PIN); ///< Instantiate SoftwareWire
/// @}

#endif

///
/// @brief	Address of the sensor
///
#define _address 0x18

///
/// @brief	Read value
///
uint16_t _reading;



//------------------------------------------------------------------------------
///
/// @brief	setup
///
void setup(void) {
    
    Serial.begin(9600);
    Serial.println("*** START");
    
    Serial.print("I2C begin... ");
    
    Wire.begin();
    
#if (I2C_KIND == I2C_HARDWARE)
#else
#endif
    
    Serial.println("done");
    
    
    Serial.println("PUSH2 to end.");
    pinMode(PUSH2, INPUT_PULLUP);
}

///
/// @brief	loop
///
void loop(void)
{
    Wire.beginTransmission(_address);
    Wire.write('A');
    Wire.endTransmission();
    
    delay(10);
    
    Wire.requestFrom(_address, 2);
    while (Wire.available() < 2);
    
    _reading = Wire.read();
    _reading = _reading << 8;
    _reading += Wire.read();
    
    
    Serial.print(_reading/10, DEC);
    Serial.print(".");
    Serial.println(_reading%10, DEC);
    
    if (digitalRead(PUSH2)==0) {
        Serial.println("*** END");
        Serial.end();
        while (true);
    }
    
    delay(500);
}




