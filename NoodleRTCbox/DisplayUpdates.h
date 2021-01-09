// DisplayUpdates.h

#ifndef _DISPLAYUPDATES_h
#define _DISPLAYUPDATES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#else
#include "WProgram.h"
#endif

class DisplayClass {
public:
	friend class SubMenu;
	DisplayClass() : OLED(128, 64) {}
	//menu that displays all relays
	void mainMenu(void);
	//This will print the numbers of the relays on the screen
	void eightRelayNumbers();
	//From the main menu - Press C (control relays) ->enable/disable or turn on/off
	void overrideSubMenuDisplay();
	//When printing status of relays, this prints black rectangles over them to prevent having to refresh the whole screen
	void clearRelayUpdate();
	//goes in tandem with clearRelayUpdate - prints out status of each relay - Used for enable/disable and turned on/off manually
	void overrideScreenUpdate(int*);
	//Prints the screen where pressing 1-8 turns on/off the corresponding relay
	void manualOnOff();					
	//Prints the screen where pressing 1-8 enables/disables manual control of the coreesponding relay
	void enableDisableRelayScreen();	
private:
	Adafruit_SSD1306 OLED;
};

#endif