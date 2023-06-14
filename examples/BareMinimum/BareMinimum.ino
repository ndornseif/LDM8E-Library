/*
  BareMinimum.ino
  LDM8EDriver library example code.
  This code displays 9999 on the LEDs.
  Created 2023-06-14 by N.Dornseif
  Writen for the LDM8E hardware only.
  More info on this library:
  https://github.com/ndornseif/LDM8E-Library
  More info on the LDM8E hardware:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
  Published under GPL-3.0 license.
*/
#include <LDM8EDriver.h>

/*
  This parameter defines the dimming mode
  0=Chip Enable PWM (LDM2)
  1=Split ground PWM (LDM1)
  2=Analog dimming (Both)
  See github for more info on dimming:
  https://github.com/ndornseif/LDM8E-LEDDisplayController
*/
const byte dimmingMode = 2;

//Defines the number of diplay modules connected.
const byte numModules = 4;

//Set up a display named "mydisplay"
LDM8EDisplay mydisplay(dimmingMode, numModules);

void setup() {
  //Initalize the display
  mydisplay.begin();

  //Max Brightness
  mydisplay.setBrightness(255);

  //Set all digits to nine
  mydisplay.writeDisplay(9999);
}

void loop() {
}
