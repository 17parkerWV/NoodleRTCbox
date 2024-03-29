#include "SubMenus.h"
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
//Prints the header for the screen
void SubMenu::displayHeader(String message) {
	subMenuDisplayObject.printHeader(message);
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
////------END GENERAL UTILITY FUNCTIONS------////

////------FUNCTIONS FOR THE ISR------////
//It is going to be typed out so that I do not have to put it in a FOR loop, it'll look gross though
//If the current time is WITHIN scheduled time, relay is ON
void SubMenu::timeControl(int currentDay, int currentHour, int currentMinute) {
	int relay;
	for (relay = 0; relay <= 7; relay++) {
		if (powerArray[relay].getTempOverrideStatus() == false && powerArray[relay].getScheduleSetFlagStatus() == false)
			continue;
		if (powerArray[relay].getTempOverrideStatus() == true) {
			byte tempState = powerArray[relay].getTempOverrideState();
			if ((powerArray[relay].schedules.tempOverrideHour <= currentHour) && (powerArray[relay].schedules.tempOverrideMinute <= currentMinute) && (powerArray[relay].schedules.tempOverrideOffHour >= currentHour) && (powerArray[relay].schedules.tempOverrideOffMinute > currentMinute)) {
				powerArray[relay].setTempOverrideStarted();
				powerArray[relay].clearPoweredState();
				digitalWrite(powerArray[relay].schedules.relayPin, powerArray[relay].getTempOverrideState());
			}
			if ((powerArray[relay].schedules.tempOverrideOffHour <= currentHour) && (powerArray[relay].schedules.tempOverrideOffMinute <= currentMinute)) {
				powerArray[relay].clearTempOverrideFlag();
				powerArray[relay].clearTempOverrideStarted();
				if (powerArray[relay].getScheduleSetFlagStatus() == false) {
					digitalWrite(powerArray[relay].schedules.relayPin, HIGH);
					continue;
				}
			}
		}
		if (powerArray[relay].getScheduleSetFlagStatus() == true) {
			byte schedState = powerArray[relay].getScheduleState();
			if ((powerArray[relay].schedules.hourOn <= currentHour) && (powerArray[relay].schedules.minuteOn <= currentMinute) && (powerArray[relay].schedules.hourOff >= currentHour) && (powerArray[relay].schedules.minuteOff > currentMinute)) {
				powerArray[relay].setPoweredState();
				digitalWrite(powerArray[relay].schedules.relayPin, schedState);
			}
			if ((powerArray[relay].schedules.hourOff <= currentHour) && (powerArray[relay].schedules.minuteOff <= currentMinute)) {
				powerArray[relay].clearPoweredState();
				digitalWrite(powerArray[relay].schedules.relayPin, !schedState);
			}
		}
	}
}

////------END FUNCTIONS FOR THE ISR------////

////------subMenuDisplayObject DISPLAYING FUNCTIONS------////
//Main Menu --> A. Goes into options regarding set schedules
void SubMenu::displaySchedulesSubMenuDisplay() {
	subMenuDisplayObject.schedulesSubMenu();
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

//Main Menu --> C. Displays the menu items for the OverrideFlagSubMenu
void SubMenu::displayManualOverrideSubMenuDisplay() {
	subMenuDisplayObject.overrideSubMenuDisplay();
}

//Main Menu --> B. Displays the header info and options for the TempOverride Sub Menu
void SubMenu::displayTempOverrideSubMenu() {
	subMenuDisplayObject.tempOverrideSubMenu();
}


//Displays "are you sure?"
void SubMenu::displayConfirmationScreen() {
	subMenuDisplayObject.confirmationScreen();
}
////------END subMenuDisplayObject DISPLAYING FUNCTIONS------////

////------DISPLAY STATUS UPDATE FUNCTIONS------////
void SubMenu::displayStatuses(bool (Relay::* func)()) {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println((powerArray[spot].*func)() ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println((powerArray[spot + 4].*func)() ? "ON" : "OFF");
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
					displayStatuses(getManualFlag);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getPowerStatus() == false && powerArray[3].getScheduleSetFlagStatus() == false && powerArray[3].getTempOverrideStatus() == false) {
					powerArray[3].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getPowerStatus() == false && powerArray[6].getScheduleSetFlagStatus() == false && powerArray[6].getTempOverrideStatus() == false) {
					powerArray[6].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
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
					displayStatuses(getManualFlag);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (powerArray[4].getPowerStatus() == false && powerArray[4].getScheduleSetFlagStatus() == false && powerArray[4].getTempOverrideStatus() == false) {
					powerArray[4].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getPowerStatus() == false && powerArray[7].getScheduleSetFlagStatus() == false && powerArray[7].getTempOverrideStatus() == false) {
					powerArray[7].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getPowerStatus() == false && powerArray[2].getScheduleSetFlagStatus() == false && powerArray[2].getTempOverrideStatus() == false) {
					powerArray[2].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getPowerStatus() == false && powerArray[5].getScheduleSetFlagStatus() == false && powerArray[5].getTempOverrideStatus() == false) {
					powerArray[5].flipManualOverrideFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getManualFlag);
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
					displayStatuses(getPower);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getManualOverrideFlagStatus() == true || (powerArray[3].getTempOverrideStatus() == false && powerArray[3].getScheduleSetFlagStatus() == false)) {
					powerArray[3].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getManualOverrideFlagStatus() == true || (powerArray[6].getTempOverrideStatus() == false && powerArray[6].getScheduleSetFlagStatus() == false)) {
					powerArray[6].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
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
					displayStatuses(getPower);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				//Number 5 on the numpad
				if (powerArray[4].getManualOverrideFlagStatus() == true || (powerArray[4].getTempOverrideStatus() == false && powerArray[4].getScheduleSetFlagStatus() == false)) {
					powerArray[4].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getManualOverrideFlagStatus() == true || (powerArray[7].getTempOverrideStatus() == false && powerArray[7].getScheduleSetFlagStatus() == false)) {
					powerArray[7].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getManualOverrideFlagStatus() == true || (powerArray[2].getTempOverrideStatus() == false && powerArray[2].getScheduleSetFlagStatus() == false)) {
					powerArray[2].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getManualOverrideFlagStatus() == true || (powerArray[5].getTempOverrideStatus() == false && powerArray[5].getScheduleSetFlagStatus() == false)) {
					powerArray[5].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayStatuses(getPower);
				}
			}
		}
	}
}
//Main Menu --> A --> 2. Waits for input. Displays current schedule on chosen relay
void SubMenu::scheduleSetStatusWhileLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (powerArray[0].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(0, powerArray[0].schedules.hourOn, powerArray[0].schedules.minuteOn, powerArray[0].schedules.hourOff, powerArray[0].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (powerArray[3].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(3, powerArray[3].schedules.hourOn, powerArray[3].schedules.minuteOn, powerArray[3].schedules.hourOff, powerArray[3].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (powerArray[6].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(6, powerArray[6].schedules.hourOn, powerArray[6].schedules.minuteOn, powerArray[6].schedules.hourOff, powerArray[6].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				return;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (powerArray[1].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(1, powerArray[1].schedules.hourOn, powerArray[1].schedules.minuteOn, powerArray[1].schedules.hourOff, powerArray[1].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (powerArray[4].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(4, powerArray[4].schedules.hourOn, powerArray[4].schedules.minuteOn, powerArray[4].schedules.hourOff, powerArray[4].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (powerArray[7].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(7, powerArray[7].schedules.hourOn, powerArray[7].schedules.minuteOn, powerArray[7].schedules.hourOff, powerArray[7].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (powerArray[2].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(2, powerArray[2].schedules.hourOn, powerArray[2].schedules.minuteOn, powerArray[2].schedules.hourOff, powerArray[2].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (powerArray[5].getScheduleSetFlagStatus() == true) {
					subMenuDisplayObject.displaySingleObjectScheduleStatus(5, powerArray[5].schedules.hourOn, powerArray[5].schedules.minuteOn, powerArray[5].schedules.hourOff, powerArray[5].schedules.minuteOff);
					waitForAnyLetterPress();
					return;
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
void SubMenu::chooseRelay(bool (Relay::* statusFunc)(), void (Relay::* clearingFunc)(), void (SubMenu::* promptFunc)(int relayNum)) {
	int chosenArray = -1;
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if ((powerArray[0].*statusFunc)() == true) {
				confirmClear(&(powerArray[0]), clearingFunc);
				return;
			}
			(this->*promptFunc)(0);
			return;
		}
		if (buttonByte == NUM_PAD_2) {
			if ((powerArray[1].*statusFunc)() == true) {
				confirmClear(&(powerArray[1]), clearingFunc);
				return;
			}
			(this->*promptFunc)(1);
			return;
		}
		if (buttonByte == NUM_PAD_3) {
			if ((powerArray[2].*statusFunc)() == true) {
				confirmClear(&(powerArray[2]), clearingFunc);
				return;
			}
			(this->*promptFunc)(2);
			return;
		}
		if (buttonByte == NUM_PAD_4) {
			if ((powerArray[3].*statusFunc)() == true) {
				confirmClear(&(powerArray[3]), clearingFunc);
				return;
			}
			(this->*promptFunc)(3);
			return;
		}
		if (buttonByte == NUM_PAD_5) {
			if ((powerArray[4].*statusFunc)() == true) {
				confirmClear(&(powerArray[4]), clearingFunc);
				return;
			}
			(this->*promptFunc)(4);
			return;
		}
		if (buttonByte == NUM_PAD_6) {
			if ((powerArray[5].*statusFunc)() == true) {
				confirmClear(&(powerArray[5]), clearingFunc);
				return;
			}
			(this->*promptFunc)(5);
			return;
		}
		if (buttonByte == NUM_PAD_7) {
			if ((powerArray[6].*statusFunc)() == true) {
				confirmClear(&(powerArray[6]), clearingFunc);
				return;
			}
			(this->*promptFunc)(6);
			return;
		}
		if (buttonByte == NUM_PAD_8) {
			if ((powerArray[7].*statusFunc)() == true) {
				confirmClear(&(powerArray[7]), clearingFunc);
				return;
			}
			(this->*promptFunc)(7);
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
			if (buttonByte == NUM_PAD_D) {
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
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_2) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[1].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_3) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[2].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_4) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[3].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_5) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[4].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_6) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[5].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_7) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[6].off();
				subMenuDisplayObject.displayError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_8) {
			bool confirmed = confirmationSubMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				powerArray[7].off();
				subMenuDisplayObject.displayError(F("Cleared"));
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
void SubMenu::confirmClear(Relay* arrObj, void (Relay::* clearFunc)()) {
	subMenuDisplayObject.confirmClearFlag();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_4) {
			if (buttonByte == NUM_PAD_A) {
				(arrObj->*clearFunc)();
				subMenuDisplayObject.displayError(F("Cleared"));
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
			return 0;
		if (buttonPress == NUM_PAD_0)
			return 1;
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
	subMenuDisplayObject.enterTime((F("Enter starting hour  using 24 hour format Press STAR to cancel Press D when done")), (F("24 HOUR FORMAT ONLY")));
	int hour = inputTime();
	if (hour == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		delayWithoutDelay(1500);
		return;
	}
	hour = verifyHour(hour);
	if (hour == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nhour"));
		delayWithoutDelay(1500);
		return;
	}
	//Get the starting MINUTE//
	subMenuDisplayObject.enterTime((F("Enter starting\nminutes   * - back\nPress D when done")), F(""));
	int minute = inputTime();
	if (minute == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	minute = verifyMinute(minute);
	if (minute == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nminutes"));
		return;
	}
	//Get the DURATION of the override//
	subMenuDisplayObject.enterDuration();
	int duration = inputDuration();
	if (duration == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	duration = verifyDuration(duration);
	if (duration == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nDuration"));
		return;
	}

	int minuteOff = (duration + minute) % 60;	//stores the minute it will turn OFF;
	int tempHourVar = (duration / 60);
	if (((duration % 60) + minute) > 59)
		tempHourVar++;		//Add one hour if the minutes overflow
	int hourOff = (hour + tempHourVar) % 24;
	//Get the POWER STATE//
	subMenuDisplayObject.enterPowerState();
	byte powerState = inputPowerState();
	if (powerState == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	//FINAL INPUT CHECKS
	if ((hour == hourOff) && (minute == minuteOff)) {
		subMenuDisplayObject.displayError(F("Invalid Input"));
		return;
	}
	//Set the flag and swap the times
	if ((hour > hourOff) || ((hour >= hourOff) && (minute > minuteOff))) {
		powerState = true;
		int tempMinute = minute;
		minute = minuteOff;
		minuteOff = tempMinute;
		int tempHour = hour;
		hour = hourOff;
		hourOff = tempHour;
	}
	powerArray[object].schedules.tempOverrideDuration = duration;
	powerArray[object].schedules.tempOverrideState = powerState;
	powerArray[object].schedules.tempOverrideOffMinute = minuteOff;
	powerArray[object].schedules.tempOverrideOffHour = hourOff;
	powerArray[object].schedules.tempOverrideHour = hour;
	powerArray[object].schedules.tempOverrideMinute = minute;
	//FINALIZE - set the flag
	subMenuDisplayObject.displayError(F("Set"));
	powerArray[object].setTempOverrideFlag();
	return;
}

void SubMenu::promptScheduleTime(int object) {
	//RESET THE TIME
	powerArray[object].schedules.hourOn = 0;
	powerArray[object].schedules.minuteOn = 0;
	powerArray[object].schedules.hourOff = 0;
	powerArray[object].schedules.minuteOff = 0;
	subMenuDisplayObject.enterTime((F("Enter hour to TURN ONusing 24 hour format Press STAR to cancel Press D when done")), (F("24 HOUR FORMAT ONLY")));
	int startHour = inputTime();
	if (startHour == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	startHour = verifyHour(startHour);
	if (startHour == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nhour"));
		return;
	}
	subMenuDisplayObject.enterTime((F("Enter minute to TURN ON   * - back\nPress D when done")), F(""));
	int startMinute = inputTime();
	if (startMinute == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	startMinute = verifyMinute(startMinute);
	if (startMinute == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nminute"));
		return;
	}
	subMenuDisplayObject.enterTime((F("What hour should it  turn off? Press STAR to cancel Press D    when done")), (F("24 HOUR FORMAT ONLY")));	//I think this is too long but we'll see
	int stopHour = inputTime();
	if (stopHour == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	stopHour = verifyHour(stopHour);
	if (stopHour == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nhour"));
		return;
	}
	subMenuDisplayObject.enterTime((F("Enter minute to TURN OFF   * - back\nPress D when done")), F(""));
	int stopMinute = inputTime();
	if (stopMinute == -1) {
		subMenuDisplayObject.displayError(F("Canceled"));
		return;
	}
	stopMinute = verifyMinute(stopMinute);
	if (stopMinute == -1) {
		subMenuDisplayObject.displayError(F("Invalid\nminute"));
		return;
	}
	//FINAL INPUT CHECKS
	if ((startHour == stopHour) && (startMinute == stopMinute)) {
		subMenuDisplayObject.displayError(F("Invalid Input"));
		return;
	}
	byte powerState = 0;
	//Set the flag and switch the times
	if (((startHour > stopHour) || ((startHour >= stopHour) && (startMinute > stopMinute)))) {
		powerState = 1;
		int tempHour = startHour;
		startHour = stopHour;
		stopHour = tempHour;
		int tempMinute = startMinute;
		startMinute = stopMinute;
		stopMinute = tempMinute;
	}
	//FINALIZE - set the flag
	powerArray[object].schedules.scheduleState = powerState;
	powerArray[object].schedules.minuteOff = stopMinute;
	powerArray[object].schedules.hourOn = startHour;
	powerArray[object].schedules.hourOff = stopHour;
	powerArray[object].schedules.minuteOn = startMinute;
	subMenuDisplayObject.displayError(F("Set"));
	powerArray[object].setScheduleSetFlag();
	return;
}

////------END NON-LOOPING MENU FUNCTIONS/SUB MENU FLOW CONTROL FUNCTIONS------////