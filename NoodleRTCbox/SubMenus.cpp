#include "SubMenus.h"

//*************//
//CHECK GIT COMMITS FOR NOTES AND THINGS TO DO
//CHECK ONENOTE FOR PROBLEMS I'VE LOGGED
//*************//


////------GENERAL UTILITY FUNCTIONS------////
//Starts the display object and checks for successful begin()
bool SubMenu::initializeDisplay() {
	if (!subMenuDisplayObject.OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C))
		return false;
	subMenuDisplayObject.OLED.display();
	subMenuDisplayObject.OLED.clearDisplay();
	subMenuDisplayObject.OLED.display();
	subMenuDisplayObject.OLED.setCursor(0, 0);
	subMenuDisplayObject.OLED.setTextColor(WHITE);
	return true;
}
//Initializes the relay objects with the physical pins they control
void SubMenu::initializePins() {
	for (int i = 0; i <= 7; i++) {
		pinMode(relayArrayPins[i], OUTPUT);
		powerArray[i].schedules.relayPin = relayArrayPins[i];
		powerArray[i].off();
	}
}
//Turns off and sets appropriate flags for ALL relay objects
void SubMenu::allOff() {
	for (int i = 0; i <= 7; i++) {
		powerArray[i].off();
	}
}
//Turns off and sets appropriate flags for the calling object
void SubMenu::off() {
	Relay::off();
}

//Causes a delay without using delay(), because it uses interrupts
void delayWithoutDelay(int time) {
	unsigned long delayTime = millis();
	while ((millis() - time) < delayTime) {
	}
}
//saves the state of the button as a byte, to be decoded elsewhere
byte buttonPoll() {
	byte buttonPress = 0b00000000;
	byte rowBits = (PINC & ROW_BITS);
	DDRC = COL_IN_ROW_OUT;
	PORTC = ROW_LOW_COL_PULLUP;
	delayWithoutDelay(50);				//GETS STUCK IN LOOP HERE but not anymore? Possible cause for error
	byte colBits = (PINC & COL_BITS);
	buttonPress = rowBits | colBits;
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;
	delayWithoutDelay(50);
	return buttonPress;
}
//Menu that pauses sub menu flow until A, B, C, or D is pressed
void waitForAnyLetterPress() {
	delayWithoutDelay(175);
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4)
			return;
	}
	delayWithoutDelay(175);
}
//Compares schedule/temp override times and controls relay objects accordingly. Called during interrupt function
void SubMenu::checkSchedule() {
	for (int relay = 0; relay <= 7; relay++) {
		if (powerArray[relay].getTempOverrideStatus() == true) {

		}
		powerArray[relay].getTempOverrideStatus() == true;
	}
}
////------END GENERAL UTILITY FUNCTIONS------////

////------subMenuDisplayObject DISPLAYING FUNCTIONS------////
//Main Menu --> A. Goes into options regarding set schedules
void SubMenu::displaySchedulesSubMenuDisplay() {
	subMenuDisplayObject.schedulesSubMenu();
}
//Main Menu --> A, press C. Header information for display that shows status of all relay objects schedule set flags
void SubMenu::displayEnableDisableScheduleScreen() {
	subMenuDisplayObject.enableDisableScheduleScreen();
}
//Main Menu --> A. Time is displayed in the upper left. When it comes time to update it, this erases just the time, so the whole screen does not flash
void SubMenu::clearCurrentTime() {
	subMenuDisplayObject.clearCurrentTime();
}
//Main Menu --> A. Time is displayed in the upper left. This prints the time when the time is right
void SubMenu::displayCurrentTime(int hour, int min) {
	subMenuDisplayObject.currentTime(hour, min);
}
//Displays the main menu. What else do you want from it?
void SubMenu::displayMainMenu() {
	subMenuDisplayObject.mainMenu();
}
//Displays 1-8 across the screen, used along side display status functions
void SubMenu::displayEightRelayNumbers() {
	subMenuDisplayObject.eightRelayNumbers();
}
//Main Menu --> C --> 2. Displays the header info for the screen where you turn relays on and off. (NOT the flags, but FLAGS need to be true to turn on)
void SubMenu::displayManualOnOffScreen() {
	subMenuDisplayObject.manualOnOff();
}
//Main Menu --> C. Displays the menu items for the OverrideFlagSubMenu
void SubMenu::displayManualOverrideSubMenuDisplay() {
	subMenuDisplayObject.overrideSubMenuDisplay();
}
//Main Menu --> C --> 1. Displays the header info for the screen where you flip the overrideFlag for the objects. (NOT the relays, these allow relays to go ON/OFF)
void SubMenu::displayEnableDisableRelayScreen() {
	subMenuDisplayObject.enableDisableRelayScreen();
}
//Main Menu --> B --> 1. Displays the header for the screen where you choose an object to set a temporary override
void SubMenu::displayTempOverrideScreen() {
	subMenuDisplayObject.temporaryOverrideDisplay();
}
//Clears the screen and displays "Cleared". So far, used to confirm that a variable/flag has been cleared
void SubMenu::displayCleared() {
	subMenuDisplayObject.displayCleared();
	delayWithoutDelay(1000);
}
//Main Menu --> B. Displays the header info and options for the TempOverride Sub Menu
void SubMenu::displayTempOverrideSubMenu() {
	subMenuDisplayObject.tempOverrideSubMenu();
}
//Main Menu --> B --> 2. Displays the header info for the screen where you select an object to have its Temp Override status/info shown on screen
void SubMenu::displayTempOverrideInfoScreen() {
	subMenuDisplayObject.tempOverrideStatus();
}
//Main Menu --> A --> 4. Displays the header info for the screen where the selected relay is completely reset
void SubMenu::displayCompleteOffScreen() {
	subMenuDisplayObject.completeOffScreen();
}
//Displays "are you sure?"
void SubMenu::displayConfirmationScreen() {
	subMenuDisplayObject.confirmationScreen();
}
////------END subMenuDisplayObject DISPLAYING FUNCTIONS------////

////------DISPLAY STATUS UPDATE FUNCTIONS------////
//Print the status of the 8 objects' scheduleSetFlag, YES if schedule is set, NO if not. Used with displayEightRelayNumbers
void SubMenu::displayScheduleSetFlagStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].getScheduleSetFlagStatus() ? "YES" : "NO");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].getScheduleSetFlagStatus() ? "YES" : "NO");
	}
	subMenuDisplayObject.OLED.display();
}
//Print the status of the 8 objects' manualOverrideFlag. ON means manual control (ON/OFF) is allowed, OFF means it is not. Used with displayEightRelayNumbers
void SubMenu::displayOverrideFlagStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].getManualOverrideFlagStatus() ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].getManualOverrideFlagStatus() ? "ON" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}
//Print the status of the 8 objects' powered status. ON means the relay is ON, OFF means it is OFF. Used with displayEightRelayNumbers
void SubMenu::displayOnOffStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].getPowerStatus() ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].getPowerStatus() ? "ON" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}
//Print the status of the 8 objects' tempOverrideFlag. SET means a temp override is SET, OFF means one is NOT SET. Used with displayEightRelayNumbers
void SubMenu::displayTempOverrideStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].getTempOverrideStatus() ? "SET" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].getTempOverrideStatus() ? "SET" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}
////------END DISPLAY STATUS UPDATE FUNCTIONS------////


////------SUB MENU WHILE LOOPS------////
//Main Menu --> C --> 1. Waits for input. Selecting a relay will flip the manualOverrideFlag, if (powered == scheduleSetFlag == tempOverrideFlag == false)
void SubMenu::enableDisableRelaySubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (powerArray[0].getPowerStatus() == false && powerArray[0].getScheduleSetFlagStatus() == false && powerArray[0].getTempOverrideStatus() == false) {
					powerArray[0].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getPowerStatus() == false && powerArray[3].getScheduleSetFlagStatus() == false && powerArray[3].getTempOverrideStatus() == false) {
					powerArray[3].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getPowerStatus() == false && powerArray[6].getScheduleSetFlagStatus() == false && powerArray[6].getTempOverrideStatus() == false) {
					powerArray[6].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (powerArray[1].getPowerStatus() == false && powerArray[1].getScheduleSetFlagStatus() == false && powerArray[1].getTempOverrideStatus() == false) {
					powerArray[1].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (powerArray[4].getPowerStatus() == false && powerArray[4].getScheduleSetFlagStatus() == false && powerArray[4].getTempOverrideStatus() == false) {
					powerArray[4].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getPowerStatus() == false && powerArray[7].getScheduleSetFlagStatus() == false && powerArray[7].getTempOverrideStatus() == false) {
					powerArray[7].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getPowerStatus() == false && powerArray[2].getScheduleSetFlagStatus() == false && powerArray[2].getTempOverrideStatus() == false) {
					powerArray[2].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getPowerStatus() == false && powerArray[5].getScheduleSetFlagStatus() == false && powerArray[5].getTempOverrideStatus() == false) {
					powerArray[5].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideFlagStatus();
				}
			}
		}
	}
}
//Main Menu --> C --> 2. Waits for input. Flips the power state of the relay (ON/OFF) if (manualOverrideFlag == true && tempOverrideFlag == false)
void SubMenu::manualOnOffSubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (powerArray[0].getManualOverrideFlagStatus() == true || (powerArray[0].getTempOverrideStatus() == false && powerArray[0].getScheduleSetFlagStatus() == false)) {
					powerArray[0].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getManualOverrideFlagStatus() == true || (powerArray[3].getTempOverrideStatus() == false && powerArray[3].getScheduleSetFlagStatus() == false)) {
					powerArray[3].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getManualOverrideFlagStatus() == true || (powerArray[6].getTempOverrideStatus() == false && powerArray[6].getScheduleSetFlagStatus() == false)) {
					powerArray[6].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (powerArray[1].getManualOverrideFlagStatus() == true || (powerArray[1].getTempOverrideStatus() == false && powerArray[1].getScheduleSetFlagStatus() == false)) {
					powerArray[1].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_5) {
				//Number 5 on the numpad
				if (powerArray[4].getManualOverrideFlagStatus() == true || (powerArray[4].getTempOverrideStatus() == false && powerArray[4].getScheduleSetFlagStatus() == false)) {
					powerArray[4].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getManualOverrideFlagStatus() == true || (powerArray[7].getTempOverrideStatus() == false && powerArray[7].getScheduleSetFlagStatus() == false)) {
					powerArray[7].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getManualOverrideFlagStatus() == true || (powerArray[2].getTempOverrideStatus() == false && powerArray[2].getScheduleSetFlagStatus() == false)) {
					powerArray[2].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getManualOverrideFlagStatus() == true || (powerArray[5].getTempOverrideStatus() == false && powerArray[5].getScheduleSetFlagStatus() == false)) {
					powerArray[5].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffStatus();
				}
			}
		}
	}
}
//Main Menu --> A --> 3. Waits for input. Flips the scheduleSetFlag if (powered == false)
void SubMenu::enableDisableSchedulesSubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (powerArray[0].getPowerStatus() == false) {
					powerArray[0].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getPowerStatus() == false) {
					powerArray[3].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getPowerStatus() == false) {
					powerArray[6].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (powerArray[1].getPowerStatus() == false) {
					powerArray[1].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (powerArray[4].getPowerStatus() == false) {
					powerArray[4].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getPowerStatus() == false) {
					powerArray[7].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getPowerStatus() == false) {
					powerArray[2].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getPowerStatus() == false) {
					powerArray[5].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
	}
}
//Main Menu --> B --> 2. Waits for input. Displays the selected object's temporary Override status and variables, if a temporary override exists
void SubMenu::tempOverrideStatusWhileLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if (powerArray[0].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[0].schedules.tempOverrideHour), (powerArray[0].schedules.tempOverrideMinute), (powerArray[0].schedules.tempOverrideDuration), (powerArray[0].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_2) {
			if (powerArray[1].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[1].schedules.tempOverrideHour), (powerArray[1].schedules.tempOverrideMinute), (powerArray[1].schedules.tempOverrideDuration), (powerArray[1].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_3) {
			if (powerArray[2].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[2].schedules.tempOverrideHour), (powerArray[2].schedules.tempOverrideMinute), (powerArray[2].schedules.tempOverrideDuration), (powerArray[2].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_4) {
			if (powerArray[3].getTempOverrideStatus() == true) {

				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[3].schedules.tempOverrideHour), (powerArray[3].schedules.tempOverrideMinute), (powerArray[3].schedules.tempOverrideDuration), (powerArray[3].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_5) {
			if (powerArray[4].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[4].schedules.tempOverrideHour), (powerArray[4].schedules.tempOverrideMinute), (powerArray[4].schedules.tempOverrideDuration), (powerArray[4].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_6) {
			if (powerArray[5].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[5].schedules.tempOverrideHour), (powerArray[5].schedules.tempOverrideMinute), (powerArray[5].schedules.tempOverrideDuration), (powerArray[5].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_7) {
			if (powerArray[6].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[6].schedules.tempOverrideHour), (powerArray[6].schedules.tempOverrideMinute), (powerArray[6].schedules.tempOverrideDuration), (powerArray[6].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_8) {
			if (powerArray[7].getTempOverrideStatus() == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[7].schedules.tempOverrideHour), (powerArray[7].schedules.tempOverrideMinute), (powerArray[7].schedules.tempOverrideDuration), (powerArray[7].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_STAR)
			return;
	}
}
//Main Menu --> B --> 1. Waits for input. Displays status of temporary override flag (SET/OFF). Waits for parameters to new temp override, or clears if an override is active already
void SubMenu::chooseRelay() {
	int chosenArray = -1;
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if (powerArray[0].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[0]));
				return;
			}
			promptTempOverrideTime(0);
			return;
		}
		if (buttonByte == NUM_PAD_2) {
			if (powerArray[1].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[1]));
				return;
			}
			promptTempOverrideTime(1);
			return;
		}
		if (buttonByte == NUM_PAD_3) {
			if (powerArray[2].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[2]));
				return;
			}
			promptTempOverrideTime(2);
			return;
		}
		if (buttonByte == NUM_PAD_4) {
			if (powerArray[3].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[3]));
				return;
			}
			promptTempOverrideTime(3);
			return;
		}
		if (buttonByte == NUM_PAD_5) {
			if (powerArray[4].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[4]));
				return;
			}
			promptTempOverrideTime(4);
			return;
		}
		if (buttonByte == NUM_PAD_6) {
			if (powerArray[5].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[5]));
				return;
			}
			promptTempOverrideTime(5);
			return;
		}
		if (buttonByte == NUM_PAD_7) {
			if (powerArray[6].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[6]));
				return;
			}
			promptTempOverrideTime(6);
			return;
		}
		if (buttonByte == NUM_PAD_8) {
			if (powerArray[7].getTempOverrideStatus() == true) {
				confirmClear(&(powerArray[7]));
				return;
			}
			promptTempOverrideTime(7);
			return;
		}
		if (buttonByte == NUM_PAD_STAR)
			return;
	}
}
////------END SUB MENU WHILE LOOPS------////

////------INPUTTING FUNCTIONS------////
int SubMenu::inputTime() {
	int NumberOfInputs = 2;
	int timeInput = 0;
	while (NumberOfInputs > 0) {
		while (1) {
			byte buttonByte = buttonPoll();
			if (buttonByte == NUM_PAD_1) {
				NumberOfInputs--;
				timeInput += (1 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_2) {
				NumberOfInputs--;
				timeInput += (2 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_3) {
				NumberOfInputs--;
				timeInput += (3 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_4) {
				NumberOfInputs--;
				timeInput += (4 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_5) {
				NumberOfInputs--;
				timeInput += (5 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_6) {
				NumberOfInputs--;
				timeInput += (6 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_7) {
				NumberOfInputs--;
				timeInput += (7 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_8) {
				NumberOfInputs--;
				timeInput += (8 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_9) {
				NumberOfInputs--;
				timeInput += (9 * (pow(10, NumberOfInputs)));
				break;
			}
			if (buttonByte == NUM_PAD_0) {
				NumberOfInputs--;
				timeInput += 0;
				break;
			}
			if (buttonByte == NUM_PAD_SHARP) {
				timeInput /= 10;
				subMenuDisplayObject.printTime(timeInput, NumberOfInputs);
				return timeInput;
			}
			if (buttonByte == NUM_PAD_STAR)
				return -1;
		}
		subMenuDisplayObject.printTime(timeInput, NumberOfInputs);
		delayWithoutDelay(175);
	}
	return timeInput;
}

int SubMenu::inputDuration() {
	int durationInput = 0;
	int loopCount = 0;
	while (1) {
		while (1) {
			byte buttonByte = buttonPoll();
			if (buttonByte == NUM_PAD_1) {
				if (loopCount == 0) {
					durationInput = 1;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 1;
				break;
			}
			if (buttonByte == NUM_PAD_2) {
				if (loopCount == 0) {
					durationInput = 2;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 2;
				break;
			}
			if (buttonByte == NUM_PAD_3) {
				if (loopCount == 0) {
					durationInput = 3;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 3;
				break;
			}
			if (buttonByte == NUM_PAD_4) {
				if (loopCount == 0) {
					durationInput = 4;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 4;
				break;
			}
			if (buttonByte == NUM_PAD_5) {
				if (loopCount == 0) {
					durationInput = 5;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 5;
				break;
			}
			if (buttonByte == NUM_PAD_6) {
				if (loopCount == 0) {
					durationInput = 6;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 6;
				break;
			}
			if (buttonByte == NUM_PAD_7) {
				if (loopCount == 0) {
					durationInput = 7;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 7;
				break;
			}
			if (buttonByte == NUM_PAD_8) {
				if (loopCount == 0) {
					durationInput = 8;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 8;
				break;
			}
			if (buttonByte == NUM_PAD_9) {
				if (loopCount == 0) {
					durationInput = 9;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 9;
				break;
			}
			if (buttonByte == NUM_PAD_0) {
				if (loopCount == 0) {
					continue;
				}
				durationInput *= 10;
				durationInput += 0;
				break;
			}
			if (buttonByte == NUM_PAD_D)
				return durationInput;
			if (buttonByte == NUM_PAD_STAR)
				return -1;
		}
		subMenuDisplayObject.printTime(durationInput);
		delayWithoutDelay(175);
	}
}
//Sub menu that will reset the relay
void SubMenu::completeOffSubMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[0].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_2) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[1].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_3) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[2].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_4) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[3].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_5) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[4].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_6) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[5].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_7) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[6].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_8) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[7].off();
				displayCleared();
				return;
			}
		}
		if (buttonPress == NUM_PAD_STAR) {
			return;
		}
	}
}
//Confirmation menu that is displayed when a set temp override flag is selected to be deleted
//I had it pass the object so I can keep the clearing functions AND the display function wrapped up
void SubMenu::confirmClear(Relay* arrObj) {
	subMenuDisplayObject.confirmClearTempOverride();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_4) {
			if (buttonByte == NUM_PAD_A) {
				clearTempOverrideFlag();
				displayCleared();
				break;
			}
			if (buttonByte == NUM_PAD_B)
				break;
		}
	}
}
//Menu that gets the answer to "Are you sure?"
bool SubMenu::confirmationSubMenu() {
	subMenuDisplayObject.confirmationScreen();
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4) {
			if (buttonPress == NUM_PAD_A)
				return true;
			if (buttonPress == NUM_PAD_B)
				return false;
		}
	}
}
//Input menu for power state when creating new TempOverride
byte SubMenu::inputPowerState() {	//this should only allow 0 or 1 (or # to cancel) as possible inputs to cut down on verifying functions
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)
			return 1;
		if (buttonPress == NUM_PAD_0)
			return 0;
		if (buttonPress == NUM_PAD_SHARP)
			return -1;
	}
	return -1;		//redundant and unnecessary, get over it
}
////------END INPUTTING FUNCTIONS------////

////------INTEGRITY CHECK FUNCTIONS------////
int SubMenu::verifyHour(int hour) {
	if (hour == 24)
		hour = 0;
	if (hour < 0 || hour > 24)
		return -1;
	return hour;
}

int SubMenu::verifyMinute(int min) {
	if (min > 59 || min < 0)
		return -1;
	return min;
}

int SubMenu::verifyDuration(int dur) {
	if (dur <= 0 || dur > 1440)
		return -1;
	return dur;
}
////------END INTEGRITY CHECK FUNCTIONS------////

////------NON-LOOPING MENU FUNCTIONS/SUB MENU FLOW CONTROL FUNCTIONS------////
void SubMenu::promptTempOverrideTime(int object) {
	//Get the starting HOUR//
	powerArray[object].schedules.tempOverrideHour = 0;
	powerArray[object].schedules.tempOverrideMinute = 0;
	powerArray[object].schedules.tempOverrideDuration = 0;
	subMenuDisplayObject.enterStartingHour();
	int hours = inputTime();
	if (hours == -1) {
		subMenuDisplayObject.displayError("Cancelled");
		delayWithoutDelay(1500);
		return;
	}
	hours = verifyHour(hours);
	if (hours == -1) {
		subMenuDisplayObject.displayError("Invalid\nhour");
		delayWithoutDelay(1500);
		return;
	}
	powerArray[object].schedules.tempOverrideHour = hours;
	//Get the starting MINUTE//
	subMenuDisplayObject.enterStartingMinute();
	powerArray[object].schedules.tempOverrideHour = hours;
	int minutes = inputTime();
	minutes = verifyMinute(minutes);
	if (minutes == -1) {
		subMenuDisplayObject.displayError("Invalid\nminutes");
		delayWithoutDelay(1500);
		return;
	}
	powerArray[object].schedules.tempOverrideMinute = minutes;
	//Get the DURATION of the override//
	subMenuDisplayObject.enterDuration();
	int duration = inputDuration();
	if (duration == -1) {
		subMenuDisplayObject.displayError("Cancelled");
		delayWithoutDelay(1500);
		return;
	}
	duration = verifyDuration(duration);
	if (duration == -1) {
		subMenuDisplayObject.displayError("Invalid\nDuration");
		delayWithoutDelay(1500);
		return;
	}
	powerArray[object].schedules.tempOverrideDuration = duration;
	//Get the POWER STATE//
	subMenuDisplayObject.enterPowerState();
	byte powerState = inputPowerState();
	if (powerState == -1) {
		return;
	}
	powerArray[object].schedules.tempOverrideState = powerState;
	//FINALIZE - set the flag
	powerArray[object].setTempOverrideFlag();
	return;
}
////------END NON-LOOPING MENU FUNCTIONS/SUB MENU FLOW CONTROL FUNCTIONS------////