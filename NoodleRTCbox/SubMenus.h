// SubMenus.h

#ifndef _SUBMENUS_h
#define _SUBMENUS_h

#if defined(ARDUINO) && ARDUINO >= 100
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
#include "arduino.h"
#else
#include "WProgram.h"
#endif

byte buttonPoll(void);		//this will get byte data from the number pad to reduce duplicated code
void delayWithoutDelay(int);	//This is just to get rid of delay() which uses interrupts
void waitForAnyLetterPress();

Adafruit_SSD1306 disp(128, 64);


void chooseRelay(bool(*statusFunc)(outlets&), void(*clearFunc)(outlets&), void(*promptFunc)(outlets&));
void dispSingleScheduleStatus(outlets& obj, int index);
void dispSingleTempOverrideStatus(outlets& obj, int index);
int inputTime();
int verifyHour(int hour) {
	return (hour < 0 || hour > 24 ? -1 : hour % 24);
}
int verifyMinute(int min) {
	return (min  <60  && min >= 0);
}
int verifyDuration(int dur) {
	return (dur > 0 && dur <= 1440);
}
byte inputPowerState();
int inputDuration();
void scheduleSetStatusLoop();
void tempOverrideStatusLoop();
bool confirmationSubMenu();
void completeOffSubMenu();
void dispEightRelays(int mode, String message = "", bool header = false);
void initializePins();
void promptTempOverrideTime(outlets&);
void promptSchedTime(outlets&);
void allOff();
void confirmClear(outlets&, void (*clearFunc)(outlets&));
//SubMenu loops
void enableDisableRelaySubMenu();
void manualOnOffSubMenu();
bool getScheduleSetFlag(outlets& obj) {
	return obj.getSchedFlag();
}
void clearScheduleFlag(outlets& obj) {
	obj.clearSchedSetFlag();
}
bool getTempFlag(outlets& obj) {
	return obj.getTempOverrideFlag();
}
void clearTempFlag(outlets& obj) {
	obj.clearTempOverrideFlag();
}
void clearCurrentTime();
void displayCurrentTime(int hour, int min);

//Functions that display stuff




#endif