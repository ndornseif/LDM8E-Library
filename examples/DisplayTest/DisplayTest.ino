/*
  DisplayTest.ino
  LDM8EDriver library example code.
  This program performs a simple display test.
  Created 2023-06-04 by N.Dornseif
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

  //Turn off all segments
  mydisplay.setDisplay(0);
}

void displayTest(){
  //Reset to minimum brightness
  mydisplay.setBrightness(0);

  //Set all digits to eight to activate all segments except the decimal points
  mydisplay.writeDisplay(8888);
  
  //Ramp brightness up.
  int i = 0;
  while (i < 256){
    mydisplay.setBrightness(i);
    i++;
    delay(10);
  }
  delay(1000);

  //Ramp brightness down.
  i = 255;
  while (i > -1){
    mydisplay.setBrightness(i);
    i--;
    delay(10);
  }
  delay(1000);

  //Reset to full brightness
  mydisplay.setBrightness(255);

  /*
    Turn on each of the segments once in order.
    Since every bit represents exactly one segment
    a one is shifted left to activate them individually
  */
  unsigned long displayState = 1;
  for(byte i = 0; i < (numModules * 8); i++){
    mydisplay.setDisplay(displayState);
    displayState = displayState << 1;
    delay(400);
  }
}

void loop() {
  displayTest();
}
