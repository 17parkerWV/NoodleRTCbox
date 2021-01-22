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
void waitForAnyLetterPress();

class SubMenu : public Relay {
public:
	//return the status of the shecduleSetFlag
	bool (Relay::* getSchedFlag)() = &Relay::getScheduleSetFlagStatus;
	//return the status of the tempOverrideFlag (if an override exists)
	bool (Relay::* getTempFlag)() = &Relay::getTempOverrideStatus;
	//returns the status of the manualOnOffFlag (if manual on/off is permitted)
	bool (Relay::* getManualFlag)() = &Relay::getManualOverrideFlagStatus;
	//return if the relay is on/off
	bool (Relay::* getPower)() = &Relay::getPowerStatus;
	//return if the relay is currently in an override
	bool (Relay::* getTempStarted)() = &Relay::getTempOverrideStartedStatus;
	//clear the status of the temp override flag
	void (Relay::* clearTempOverrideFlag)() = &Relay::clearTempOverrideFlag;
	//clear the status of the schedule set flag
	void (Relay::* clearScheduleSetFlag)() = &Relay::clearScheduleSetFlag;
	//points to function to set schedule up
	void (SubMenu::* promptSchedFunc)(int) = &SubMenu::promptScheduleTime;
	//points to function to set tempOverride up
	void (SubMenu::* promptOverrideFunc)(int) = &SubMenu::promptTempOverrideTime;

	//New display header function
	void displayHeader(String);
	//new status printer
	void displayStatuses(bool (Relay::*)());

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
	//when you pick option C from the main menu - prints the options to go to on/off menu or enable/disable menu. This redirects to DisplayObject's function
	void displayManualOverrideSubMenuDisplay();

	//prints a black bar over the time to avoid having to clear the display
	void clearCurrentTime();
	//displays the current time
	void displayCurrentTime(int, int);
	//displays the schedules sub menu and the options
	void displaySchedulesSubMenuDisplay();
	//Asks the user "are you sure?" //Currently unused
	void displayConfirmationScreen();

	//RELAY OBJECTS
	//goes through the 8 Relay powerArray[8] objects and makes each one call the member function off()
	void allOff();
	//turns off the powerArray[] object that calls it
	void off();

	//SUBMENUS
	//the submenu that waits for button input and changes manualOverrideFlag if conditions are met (this is a while(1) loop)
	void enableDisableRelaySubMenu();
	//waits for button input and turns the relays on/off if conditions are met (this is a while(1) loop)
	void manualOnOffSubMenu();
	//The submenu that shows the status of set schedules
	void scheduleSetStatusWhileLoop();
	//sub menu where selected relay is turned OFF and flags set FALSE
	void completeOffSubMenu();

	//SCHEDULE RELATED FUNCTIONS
	//Submenu for getting answer to "Are you sure?"
	bool confirmationSubMenu();

	//TEMPORARY OVERRIDE FUNCTIONS
	void displayTempOverrideSubMenu();

	void tempOverrideStatusWhileLoop();
	//This is the first while(1) menu, waiting for a relay to be selected (or a cancel)
	void chooseRelay(bool (Relay::* statusFunc)(), void (Relay::* clearingFunc)(), void (SubMenu::*promptFunc)(int relayNum));
	//this is in case you want to clear the tempoverrideflag
	void confirmClear(Relay*, void (Relay::* clearingFunc)());
	//Prompt user for starting hour, might be able to overload for use with the schedules setting menu
	int inputTime();
	int inputDuration();
	byte inputPowerState();
	void promptTempOverrideTime(int);
	//promts the user to set up schedule
	void promptScheduleTime(int);

	//INPUT VERIFICATION
	//For making sure the inputs are legal
	int verifyHour(int);
	int verifyMinute(int);
	int verifyDuration(int);

	//FUNCTIONS FOR THE ISR
	void timeControl(int, int, int);

private:
	Relay powerArray[8];
	DisplayClass subMenuDisplayObject;
};

#endif