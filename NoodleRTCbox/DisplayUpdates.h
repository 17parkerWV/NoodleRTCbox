// DisplayUpdates.h

#ifndef _DISPLAYUPDATES_h
#define _DISPLAYUPDATES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif




class DisplayClass {
	Adafruit_SSD1306 OLED;
	DisplayClass(int w, int h);
};

#endif

