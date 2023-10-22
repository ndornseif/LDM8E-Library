/*
  LDM8EDriver.cpp
  LDM8EDriver - Arduino library for using LDM8E LED display drivers.
  Created 2023-06-04 by N.Dornseif
  Writen for the LDM8E hardware only.
  More info on this library:
  https://github.com/ndornseif/LDM8E-Library
  More info on the LDM8E hardware:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
  Published under GPL-3.0 license.
*/
#include "Arduino.h"
#include "LDM8EDriver.h"

LDM8EDisplay::LDM8EDisplay(uint8_t dimmingMode, uint8_t numModules, uint8_t pinSCK, uint8_t pinRCK, uint8_t pinSDA, uint8_t pinRCL, uint8_t pinREN, uint8_t pinACT, uint8_t pinBRC)
{
  _dimmingMode = dimmingMode;
  _numModules = numModules;
  //Shift register control pins. Brought out to J9 (Data).
  _pinSCK = pinSCK;
  _pinRCK = pinRCK;
  _pinSDA = pinSDA;
  //RegisterCLR and RegisterOutputEN are only used with LDM2 modules.
  _pinRCL = pinRCL;
  _pinREN = pinREN;
  //Activity LED
  _pinACT = pinACT;
  //Brightness controll can be used for PWM or analog dimming, depending on populated components.
  _pinBRC = pinBRC;
}

//Sets up the MCU pins
void LDM8EDisplay::begin()
{
  pinMode(_pinSCK, OUTPUT);
  pinMode(_pinRCK, OUTPUT);
  pinMode(_pinSDA, OUTPUT);
  pinMode(_pinRCL, OUTPUT);
  pinMode(_pinREN, OUTPUT);
  pinMode(_pinACT, OUTPUT);
  pinMode(_pinBRC, OUTPUT);

  //Putting pins into known state
  digitalWrite(_pinSCK, LOW);
  digitalWrite(_pinRCK, LOW);
  digitalWrite(_pinSDA, LOW);
  digitalWrite(_pinRCL, HIGH);
  digitalWrite(_pinACT, LOW);
  digitalWrite(_pinREN, LOW);

  /*
    If chip select is used for PWM (mode 1)
    set brightness control pin to max value 
    213 is used as 255 would overvolt the displays slightly
  */
  if (_dimmingMode == 1) {
    analogWrite(_pinBRC, 213);
  } else {
    dacWrite(_pinBRC, 213);
  }

  //Max brightness
  setBrightness(255);
  //Turn off all segments
  setDisplay(0x00000000);
}

/*
  Latches data in the shift registers
  Use this to make a display configuration visible 
  after shifting it into the display modules
*/
void LDM8EDisplay::latchData(){
  //Clock shift register output register
  digitalWrite(_pinRCK, HIGH);
  delayMicroseconds(10);
  digitalWrite(_pinRCK, LOW);
}

/*
  Shift out a byte to the display modules
  Each of the bits corresponds to a digit of the display module.
  (A is MSB, DP is LSB): A, B, C, D, E, F, G, DP
*/
void LDM8EDisplay::sendByte(uint8_t displayByte){
  //Iterate through the byte bit by bit
  for (uint8_t i = 0, mask = 1; i < 8; i++, mask = mask << 1) {
    //Set data out pin
    digitalWrite(_pinSDA, displayByte & mask);
    delayMicroseconds(10);
    //Clock display shift registers serial clock
    digitalWrite(_pinSCK, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pinSCK, LOW);
  }
}

//Shift 1-4 bytes out to the display, depending on the number of modules defined
void LDM8EDisplay::setDisplay(uint32_t displayData){
  uint32_t mask = 0xFF;
  //Move the 8 mask bits across the 32 input bits
  for(uint8_t i = 0; i < _numModules; i++)
  {
    sendByte((mask & displayData) >> (8 * i));
    mask = mask << 8;
  }
  //Update display with newly shifted data
  latchData();
}

/*
  Sets display brightness to a value between 0 and 255
  255 corresponds to full brightness and 0 to barely visible
  Increase or remove DAC offset resistor (R4) to allow for lower brightness settings when using analog dimming
*/
void LDM8EDisplay::setBrightness(uint8_t brightness){
  /*
    Scale the input value of 0-255 to the range 0-213
    213 is used as 255 would overvolt the displays slightly
    Conversion does not apply to chip enable PWM
  */
  uint16_t longBrightness = ((brightness * 100) / 124);
  switch(_dimmingMode)
  {
    case 0: //Chip enable PWM
      //Invert the value since the enable pin is active low
      analogWrite(_pinREN, 0xFF ^ brightness);
      break;
      
    case 1: //Split ground PWM
      analogWrite(_pinBRC, (uint8_t)longBrightness);
      break;
      
    case 2: //Analog dimming
      dacWrite(_pinBRC, (uint8_t)longBrightness);
      break;
  }
}

/*
  Takes in an integer and writes it out to the display as a number
  If the integer is shorter than the configured number of modules
  it will be padded with zeros
  If it is longer only the last digits that fit on the display will be displayed
  On a four module display:
  123456  -> 3456
  12      -> 0012
*/
void LDM8EDisplay::writeDisplay(uint16_t displayNumber){
  //Take in a number and convert it into a 32bit display configuraton
  //Each byte coresponds to one seven segment display
  uint32_t output = 0;
  for(uint8_t i = 0; i < _numModules; i++)
  {
    uint8_t digit = displayNumber % 10;
    displayNumber /= 10;
    uint32_t digitByte = LDM8E_DIGIT_BYTES[digit];
    output += digitByte << (i * 8);
  }
  setDisplay(output);
}

/*
  Clear all connected shift registers and output registers. 
  NOTE: Only works with LDM2 modules.
*/
void LDM8EDisplay::clearDisplay(){
  //Pulses the CLR pin to low since it is active low.
  digitalWrite(_pinRCL, LOW);
  delayMicroseconds(10);
  digitalWrite(_pinRCL, HIGH);
}

/*
  Set the Activity LED to the specified state.
*/
void LDM8EDisplay::setActyLED(bool ledState){
  //Pulses the CLR pin to low since it is active low.
  digitalWrite(_pinACT, ledState);
}