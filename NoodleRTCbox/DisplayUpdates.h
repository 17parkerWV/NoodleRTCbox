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

	//GENERAL/COMMON DISPLAY UPDATES
	//This will print the numbers of the relays on the screen
	void eightRelayNumbers();
	//When printing status of relays, this prints black rectangles over them to prevent having to refresh the whole screen
	void clearRelayUpdate();
	//In case they input a bogus time
	void invalidTime(String);

	//SUBMENU DISPLAY WITH OPTIONS
	//From the main menu - Press C (control relays) ->enable/disable or turn on/off
	void overrideSubMenuDisplay();
	//From the main menu - Press A (schedules sub menu)
	void schedulesSubMenu();
	//Temporary override sub menu
	void tempOverrideSubMenu();

	//SUBMENUS THAT DISPLAY THE 8 RELAYS
	//Prints the screen where pressing 1-8 turns on/off the corresponding relay
	void manualOnOff();
	//Prints the screen where pressing 1-8 enables/disables manual control of the coreesponding relay
	void enableDisableRelayScreen();
	//Prints the screen where 1-8 enables/disables schedules for that relay
	void enableDisableScheduleScreen();

	//TEMPORARY OVERRIDE SUB MENUS (they get their own because it's going to be nested probably
	void temporaryOverrideDisplay();
	void enterStartingHour();
	void enterStartingMinute();
	void enterDuration();
	void enterPowerState();
	void tempOverrideStatus();
	void displaySingleObjectTempOverrideStatus(int, int, byte);

	//This is to print the time as you type it, just kinda nice looking
	void printTime(int, int);
	void printTime(int);

	//CLOCK DISPLAYS
	//takes hour and min as arguments and displays them at the top of the screen
	void currentTime(int, int);
	//draws a black rectangle over the time to avoid clearing the whole screen
	void clearCurrentTime();
	//When you select a relay that already has tempOverrideFlag = true
	void confirmClearTempOverride();


private:
	Adafruit_SSD1306 OLED;
};

#endif