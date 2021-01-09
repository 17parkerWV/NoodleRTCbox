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

class SubMenu: public Relay {
public:
	void initializeDisplay();
	void initializePins();
	void displayMainMenu();
	void displayEightRelayNumbers();
	void displayManualOnOff();
	void displayOverrideScreenStatus();
	void displayOnOffScreenStatus();
	void displayOverrideSubMenuDisplay();
	void displayEnableDisableRelayScreen();
	void enableDisableRelaySubMenu();
	void allOff();
	void off();
	void manualOnOffSubMenu();	

private:
	Relay powerArray[8];
	DisplayClass subMenuDisplayObject;
};

#endif