///
/// @mainpage I2C_Software
///
/// @details Software I2C for MSP430
/// @n
/// @n @b   Contribute!
/// @n  Help me for my developments: http://embeddedcomputing.weebly.com/contact
///
/// @n @a Developed with [embedXcode](http://embedXcode.weebly.com)
///
/// @author Rei VILO
/// @author http://embedXcode.weebly.com
/// @date Feb 10, 2014
/// @version 104
///
/// @copyright (c) Rei VILO, 2013-2014
/// @copyright CC = BY NC SA
///
/// @see
/// * ReadMe.txt for references
/// * TwoWire.h - TWI/I2C library for Arduino & Wiring
/// @n Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
/// * Arduino core files for MSP430
/// @n Copyright (c) 2012 Robert Wessels. All right reserved.
#ifndef I2C_MASTER_H
#define I2C_MASTER_H
#include "Energia.h"
#define BUFFER_LENGTH 16
class SoftwareWire {
public:
    SoftwareWire(uint8_t pinSDA, uint8_t pinSCL);
    void begin();
    void beginTransmission(uint8_t address);
    uint8_t endTransmission(void);
    virtual size_t write(uint8_t data);
    virtual size_t write(const uint8_t *data , size_t length);
    uint8_t requestFrom(uint8_t address, uint8_t length);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
private:
    uint8_t readI2C(uint8_t last=false);
    uint8_t readI2C(uint8_t* data, uint8_t length);
    bool restartI2C(uint8_t address, uint8_t RW);
    bool startI2C(uint8_t address, uint8_t RW);
    void stopI2C();
    bool writeI2C(uint8_t data);
    bool writeI2C(uint8_t* data, size_t length);
    uint8_t _pinSDA, _pinSCL;
    static uint8_t txAddress;
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;
    static uint8_t transmitting;
};
#endif
