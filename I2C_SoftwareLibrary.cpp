//
// SoftwareWire.cpp
// Library C++ code
// ----------------------------------
// Developed with embedXcode
// http://embedXcode.weebly.com
//
// Project embed1
//
// Created by Rei VILO, mars 29, 2013 09:59
// http://embedXcode.weebly.com
//
//
// Copyright © Rei VILO, 2013-2014
// License CC = BY NC SA
//
// See I2C_SoftwareLibrary.h and ReadMe.txt for references
//
#include "I2C_SoftwareLibrary.h"
///
/// @brief Delay values for software I2C
///
const uint8_t DELAY_LONG = 32;
const uint8_t DELAY_FULL = 8;
const uint8_t DELAY_HALF = 4;
const uint8_t DELAY_PART = 2;
#define I2C_READ 1
#define I2C_WRITE 0
#define TICKS_PER_MS (F_CPU / 1000)
#define TICKS_PER_US (TICKS_PER_MS / 1000)
void delayI2Cms(uint16_t delay)
{
    while (delay--) __delay_cycles(TICKS_PER_MS);
}
void delayI2Cus(uint16_t delay)
{
    while (delay--) __delay_cycles(TICKS_PER_US);
}
uint8_t SoftwareWire::rxBuffer[BUFFER_LENGTH];
uint8_t SoftwareWire::rxBufferIndex = 0;
uint8_t SoftwareWire::rxBufferLength = 0;
uint8_t SoftwareWire::txAddress = 0;
uint8_t SoftwareWire::txBuffer[BUFFER_LENGTH];
uint8_t SoftwareWire::txBufferIndex = 0;
uint8_t SoftwareWire::txBufferLength = 0;
uint8_t SoftwareWire::transmitting = 0;
SoftwareWire::SoftwareWire(uint8_t pinSDA, uint8_t pinSCL) {
    _pinSDA = pinSDA;
    _pinSCL = pinSCL;
}
void SoftwareWire::begin() {
    pinMode(_pinSDA, OUTPUT);
    digitalWrite(_pinSDA, HIGH);
    pinMode(_pinSCL, OUTPUT);
    digitalWrite(_pinSCL, HIGH);
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
}
void SoftwareWire::beginTransmission(uint8_t address)
{
    transmitting = 1;
    txAddress = address;
    txBufferIndex = 0;
    txBufferLength = 0;
}
uint8_t SoftwareWire::endTransmission(void)
{
    uint8_t result = 0;
    startI2C(txAddress, I2C_WRITE);
    result += writeI2C(txBuffer, txBufferLength);
    stopI2C();
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;
    return result;
}
uint8_t SoftwareWire::requestFrom(uint8_t address, uint8_t length)
{
    if (length > BUFFER_LENGTH) length = BUFFER_LENGTH;
    startI2C(address, I2C_READ);
    if (length > 1) {
        for (uint8_t i=0; i<length-1; i++) rxBuffer[i] = readI2C(false);
    }
    rxBuffer[length-1] = readI2C(true);
    stopI2C();
    rxBufferIndex = 0;
    rxBufferLength = length;
    return length;
}
size_t SoftwareWire::write(uint8_t data)
{
    if (transmitting){
        if (txBufferLength >= BUFFER_LENGTH) {
            return 0;
        }
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        txBufferLength = txBufferIndex;
    } else {
        writeI2C(data);
    }
    return 1;
}
size_t SoftwareWire::write(const uint8_t *data, size_t length)
{
    if (transmitting) {
        for (size_t i = 0; i < length; ++i) write(data[i]);
    } else {
        for (size_t i = 0; i < length; ++i) writeI2C(data[i]);
    }
    return length;
}
int SoftwareWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}
int SoftwareWire::read(void)
{
    int value = -1;
    if(rxBufferIndex < rxBufferLength){
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}
int SoftwareWire::peek(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength) value = rxBuffer[rxBufferIndex];
    return value;
}
void SoftwareWire::flush(void)
{
    ;
}
uint8_t SoftwareWire::readI2C(uint8_t last) {
    uint8_t data = 0;
    digitalWrite(_pinSDA, LOW);
    delayI2Cus(DELAY_LONG);
    pinMode(_pinSDA, INPUT);
    delayI2Cus(DELAY_HALF);
    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        digitalWrite(_pinSCL, HIGH);
        delayI2Cus(DELAY_PART);
        if (digitalRead(_pinSDA)) data |= 1;
        delayI2Cus(DELAY_PART);
        digitalWrite(_pinSCL, LOW);
        delayI2Cus(DELAY_HALF);
    }
    pinMode(_pinSDA, OUTPUT);
    digitalWrite(_pinSDA, last);
    digitalWrite(_pinSCL, HIGH);
    delayI2Cus(DELAY_HALF);
    digitalWrite(_pinSCL, LOW);
    digitalWrite(_pinSDA, LOW);
    delayI2Cus(DELAY_LONG);
    return data;
}
uint8_t SoftwareWire::readI2C(uint8_t* data, uint8_t length)
{
    if (length > 1) {
        while (--length) *data++= readI2C(false);
    }
    *data = readI2C(true);
}
bool SoftwareWire::restartI2C(uint8_t address, uint8_t RW) {
    digitalWrite(_pinSDA, HIGH);
    digitalWrite(_pinSCL, HIGH);
    delayI2Cus(DELAY_FULL);
    return startI2C(address, RW);
}
bool SoftwareWire::startI2C(uint8_t address, uint8_t RW) {
    pinMode(_pinSDA, OUTPUT);
    digitalWrite(_pinSDA, LOW);
    digitalWrite(_pinSCL, LOW);
    delayI2Cus(DELAY_FULL);
    return writeI2C((address << 1) + RW);
}
void SoftwareWire::stopI2C(void) {
    pinMode(_pinSDA, OUTPUT);
    digitalWrite(_pinSDA, LOW);
    delayI2Cus(DELAY_FULL);
    digitalWrite(_pinSCL, HIGH);
    delayI2Cus(DELAY_FULL);
    digitalWrite(_pinSDA, HIGH);
    delayI2Cus(DELAY_FULL);
}
bool SoftwareWire::writeI2C(uint8_t data) {
    pinMode(_pinSDA, OUTPUT);
    for (uint8_t i=0; i < 8; ++i ){   
        if (data & 0x80) digitalWrite(_pinSDA, HIGH);
        else digitalWrite(_pinSDA, LOW);
        digitalWrite(_pinSCL, HIGH);
        data <<= 1;  
        delayI2Cus(DELAY_HALF);
        digitalWrite(_pinSCL, LOW);
        delayI2Cus(DELAY_HALF);
    }
    pinMode(_pinSDA, INPUT);
    digitalWrite(_pinSCL, HIGH);
    uint8_t result = !digitalRead(_pinSDA);
    if (result) digitalWrite(_pinSDA, LOW);
    delayI2Cus(DELAY_HALF);
    digitalWrite(_pinSCL, LOW);
    delayI2Cus(DELAY_FULL);
    return result;
}
bool SoftwareWire::writeI2C(uint8_t* data, size_t length)
{
    uint8_t result;
    for (uint8_t i=0; i < length; i++) result = writeI2C(data[i]);
    return result;
}
