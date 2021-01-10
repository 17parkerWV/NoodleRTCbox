// SubMenus.h

#ifndef _SUBMENUS_h
#define _SUBMENUS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
#include "DisplayUpdates.h"
#include "RelayObjects.h"

byte buttonPoll(void);		//this will get byte data from the number pad to reduce duplicated code
void delayWithoutDelay(int);	//This is just to get rid of delay() which uses interrupts

class SubMenu : public Relay {
public:
	//INITIALIZING
	//begin() the display and initialize font and text color
	bool initializeDisplay();
	//assign the relays their corresponding pin INPUT
	void initializePins();

	//DISPLAYING
	//displays the selections on the main menu
	void displayMainMenu();
	//displays the relay numbers - 1 through 4 on the left, 5 through 8 on the right
	void displayEightRelayNumbers();
	//prints the top part of the manual on/off screen, where pressing 1-8 turns on/off corresponding relay. This redirects to the DisplayObject's function
	void displayManualOnOffScreen();
	//when you pick option C from the main menu - prints the options to go to on/off menu or enable/disable menu. This redirects to DisplayObject's function
	void displayManualOverrideSubMenuDisplay();
	//prints the screen header where pressing 1-8 enables/disables manual control or the relay's power status
	void displayEnableDisableRelayScreen();
	//prints a black bar over the time to avoid having to clear the display
	void clearCurrentTime();
	//displays the current time
	void displayCurrentTime(int, int);
	//displays the schedules sub menu and the options
	void displaySchedulesSubMenuDisplay();
	//displays the header information for the schedules enable/disable sub menu
	void displayEnableDisableScheduleScreen();

	//STATUS UPDATES
	//prints the status of the 8 relay's overrideFlag variable. works in tandem with displayEightRelayNumbers
	void displayOverrideScreenStatus();
	//prints the status of the 8 relay's powered variable. works in tandem with displayEightRelayNumbers
	void displayOnOffScreenStatus();
	//prints the status of the scheduleFlag
	void displayScheduleSetFlagStatus();


	//RELAY OBJECTS
	//goes through the 8 Relay powerArray[8] objects and makes each one call the member function off()
	void allOff();
	//turns off the powerArray[] object that calls it
	void off();

	//SUBMENUS
	//the submenu that waits for button input and changes overrideFlag if conditions are met (this is a while(1) loop)
	void enableDisableRelaySubMenu();
	//waits for button input and turns the relays on/off if conditions are met (this is a while(1) loop)
	void manualOnOffSubMenu();
	//The submenu that allows/disallows schedules to be set
	void enableDisableSchedulesSubMenu();

private:
	Relay powerArray[8];
	DisplayClass subMenuDisplayObject;
};

#endif