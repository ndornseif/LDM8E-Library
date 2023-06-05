/*
  LDM8EDriver.h
  LDM8EDriver - Arduino library for using LDM8E LED display drivers.
  !!UNFINISHED ALPHA VERSION!!
  Created 23-06-04 by N.Dornseif
  Writen for the LDM8E hardware only.
  More info on this library:
  https://github.com/ndornseif/LDM8E-Library
  More info on the LDM8E hardware:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
  Published under GPL-3.0 license.
*/
#ifndef LDM8EDriver_h
#define LDM8EDriver_h

#include "Arduino.h"

//Each byte corresponds to a decimal digit that it will display if shifted into the register of a display module.
static constexpr uint8_t LDM8E_DIGIT_BYTES[10] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6};


//Represents a string of LDM display modules
class LDM8EDisplay
{
  public:
    /*
      dimmingMode tells the library what dimming mode to use.
      0=Chip Enable PWM (LDM2)
      1=Split ground PWM (LDM1)
      2=Analog dimming (Both)
      See github for more info on dimming:
      https://github.com/ndornseif/LDM8E-LEDDisplayController
      Pin definitions: SerialCLK, RegisterCLK, DataOUT, RegisterCLR, OutputENA, ActyLED, BrightnessControl
    */
    LDM8EDisplay(uint8_t dimmingMode, uint8_t numModules, uint8_t pinSCK = 32, uint8_t pinRCK = 33, uint8_t pinSDA = 27, uint8_t pinRCL = 17, uint8_t pinREN = 16, uint8_t pinACT = 2, uint8_t pinBRC = 25);
    void begin();
    void latchData();
    void sendByte(uint8_t displayByte);
    void setDisplay(uint32_t displayData);
    void writeDisplay(uint16_t displayNumber);
    void setBrightness(uint8_t brightness);
    void clearDisplay();
  private:
    uint8_t _dimmingMode;
    uint8_t _numModules;
    uint8_t _pinSCK;
    uint8_t _pinRCK;
    uint8_t _pinSDA;
    uint8_t _pinRCL;
    uint8_t _pinREN;
    uint8_t _pinACT;
    uint8_t _pinBRC;
};
#endif //LDM8EDriver_h