# LDM8E-Library
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/ndornseif/LDM8E-Library)  


Arduino library for using LDM8E LED display drivers. 

## Overview
Use this library to write code for the LDM8E.  
See the main GitHub [repo](https://github.com/ndornseif/LDM8E-LEDDisplayController) for more information on the LDM8E hardware.  
Since the LDM8E is an ESP32 architecture, this is the only architecture supported by this library.

## Installation
Install it like any other Arduino library.
1. Download the latest ZIP from [releases](https://github.com/ndornseif/LDM8E-Library/releases).
2. In the Arduino IDE, click *Sketch > Include Library > Add .ZIP Library*.
3. Select the ZIP you downloaded.
4. Upload the "DisplayTest" example sketch to your LDM8E. 
5. Verify that the display works.

## Usage
The "BareMinimum" example showcases the simplest display setup.
See LDM8EDriver.cpp and the "DisplayTest" example for more in-depth functionality.

## LDM8EDisplay class
`LDM8EDisplay mydisplay(dimmingMode, numModules)`
The `dimmingMode` parameter defines the dimming mode.  
- 0 = Chip Enable PWM (LDM2)
- 1 = Split ground PWM (LDM1)
- 2 = Analog dimming (both)
See the main GitHub [repo](https://github.com/ndornseif/LDM8E-LEDDisplayController) for more information on dimming.  
The `numModules` parameter defines the number of LDM modules connected.  

### void `begin()`
Initializes the display and sets up all MCU pins.
Initial state is all segments off and display at max brightness.
### void `writeDisplay(int number)`
Takes in a number and writes its decimal representation to the display.  
If the number is shorter than the display it will be padded with zeros.  
If it is longer, only the digits that fit will be displayed, starting from the end.  
Examples for a four-module display:   
"1234567" will be displayed as "4567".  
"12" will be displayed as "0012".  
### void `setBrightness(byte brightness)`
Sets the entire display to the specified brightness using the configured dimming mode.  
255 is the maximum brightness, and 0 is barely visible.  
### void `setDisplay(long displayState)`
Shifts the specified 32 bits out to the display.  
Every byte corresponds to a display module, and each bit to a segment.  
The bits map to the segments as follows:  
(A is MSB, decimal point is LSB): A, B, C, D, E, F, G, decimal point   
The least significant byte will end up in the last display module on the string.  
If less than four modules are configured, only as many bytes as necessary will be sent out.  
Examples for a four-module display:   
`setDisplay(0x00000000)` turns all segments off.  
`setDisplay(0x01010101)` turn on the decimal point on all displays.  
`setDisplay(0x06060606)` turn on segments F and G on all displays.  
### void `clearDisplay()`
**Only works on LDM2 modules!**  
Clears the display shift registers and turns off all segments. 

## Other
Published under GPL-3.0 license.  