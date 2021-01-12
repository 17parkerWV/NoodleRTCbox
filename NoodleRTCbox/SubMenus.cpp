#include "SubMenus.h"

//*************//
//CHECK GIT COMMITS FOR NOTES AND THINGS TO DO
//CHECK ONENOTE FOR PROBLEMS I'VE LOGGED
//*************//

void delayWithoutDelay(int time) {
	unsigned long delayTime = millis();
	while ((millis() - time) < delayTime) {
	}
}

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

void waitForAnyLetterPress() {
	delayWithoutDelay(175);
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4)
			return;
	}
	delayWithoutDelay(175);
}

void SubMenu::displaySchedulesSubMenuDisplay() {
	subMenuDisplayObject.schedulesSubMenu();
}

void SubMenu::displayEnableDisableScheduleScreen() {
	subMenuDisplayObject.enableDisableScheduleScreen();
}

void SubMenu::displayScheduleSetFlagStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].schedules.scheduleSetFlag ? "YES" : "NO");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].schedules.scheduleSetFlag ? "YES" : "NO");
	}
	subMenuDisplayObject.OLED.display();
}

bool SubMenu::initializeDisplay() {
	if (!subMenuDisplayObject.OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		return false;
	}
	subMenuDisplayObject.OLED.display();
	subMenuDisplayObject.OLED.clearDisplay();
	subMenuDisplayObject.OLED.display();
	subMenuDisplayObject.OLED.setCursor(0, 0);
	subMenuDisplayObject.OLED.setTextColor(WHITE);
	return true;
}

void SubMenu::initializePins() {
	for (int i = 0; i <= 7; i++) {
		pinMode(relayArrayPins[i], OUTPUT);
		powerArray[i].schedules.relayPin = relayArrayPins[i];
		powerArray[i].off();
	}
}

void SubMenu::off() {
	Relay::off();
}

void SubMenu::clearCurrentTime() {
	subMenuDisplayObject.clearCurrentTime();
}

void SubMenu::displayCurrentTime(int hour, int min) {
	subMenuDisplayObject.currentTime(hour, min);
}

void SubMenu::displayMainMenu() {
	subMenuDisplayObject.mainMenu();
}

void SubMenu::displayEightRelayNumbers() {
	subMenuDisplayObject.eightRelayNumbers();
}

void SubMenu::displayManualOnOffScreen() {
	subMenuDisplayObject.manualOnOff();
}

void SubMenu::displayOverrideScreenStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].schedules.manualOverrideFlag ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].schedules.manualOverrideFlag ? "ON" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}

void SubMenu::displayOnOffScreenStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].schedules.powered ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].schedules.powered ? "ON" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}

void SubMenu::displayManualOverrideSubMenuDisplay() {
	subMenuDisplayObject.overrideSubMenuDisplay();
}

void SubMenu::displayEnableDisableRelayScreen() {
	subMenuDisplayObject.enableDisableRelayScreen();
}

void SubMenu::enableDisableRelaySubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].schedules.powered == false && powerArray[0].schedules.scheduleSetFlag == false && powerArray[0].schedules.tempOverrideFlag == false) {
					powerArray[0].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.powered == false && powerArray[3].schedules.scheduleSetFlag == false && powerArray[3].schedules.tempOverrideFlag == false) {
					powerArray[3].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.powered == false && powerArray[6].schedules.scheduleSetFlag == false && powerArray[3].schedules.tempOverrideFlag == false) {
					powerArray[6].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_4) {		//Exit with the * sign
				break;
			}
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 2 on the numpad
				if (powerArray[1].schedules.powered == false && powerArray[1].schedules.scheduleSetFlag == false && powerArray[1].schedules.tempOverrideFlag == false) {
					powerArray[1].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.powered == false && powerArray[4].schedules.scheduleSetFlag == false && powerArray[4].schedules.tempOverrideFlag == false) {
					powerArray[4].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.powered == false && powerArray[7].schedules.scheduleSetFlag == false && powerArray[7].schedules.tempOverrideFlag == false) {
					powerArray[7].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.powered == false && powerArray[2].schedules.scheduleSetFlag == false && powerArray[2].schedules.tempOverrideFlag == false) {
					powerArray[2].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.powered == false && powerArray[5].schedules.scheduleSetFlag == false && powerArray[5].schedules.tempOverrideFlag == false) {
					powerArray[5].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
		}
	}
}

void SubMenu::manualOnOffSubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].schedules.manualOverrideFlag == true && powerArray[0].schedules.tempOverrideFlag == false) {
					powerArray[0].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.manualOverrideFlag == true && powerArray[3].schedules.tempOverrideFlag == false) {
					powerArray[3].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.manualOverrideFlag == true && powerArray[6].schedules.tempOverrideFlag == false) {
					powerArray[6].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_4) {
				break;
			}
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 2 on the numpad
				if (powerArray[1].schedules.manualOverrideFlag == true && powerArray[1].schedules.tempOverrideFlag == false) {
					powerArray[1].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.manualOverrideFlag == true && powerArray[4].schedules.tempOverrideFlag == false) {
					powerArray[4].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.manualOverrideFlag == true && powerArray[7].schedules.tempOverrideFlag == false) {
					powerArray[7].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.manualOverrideFlag == true && powerArray[2].schedules.tempOverrideFlag == false) {
					powerArray[2].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.manualOverrideFlag == true && powerArray[5].schedules.tempOverrideFlag == false) {
					powerArray[5].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
		}
	}
}

void SubMenu::enableDisableSchedulesSubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].schedules.powered == false && powerArray[1].schedules.tempOverrideFlag == false) {
					powerArray[0].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.powered == false && powerArray[3].schedules.tempOverrideFlag == false) {
					powerArray[3].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.powered == false && powerArray[6].schedules.tempOverrideFlag == false) {
					powerArray[6].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_4) {		//press * to exit
				break;
			}
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 2 on the numpad
				if (powerArray[1].schedules.powered == false && powerArray[1].schedules.tempOverrideFlag == false) {
					powerArray[1].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.powered == false && powerArray[4].schedules.tempOverrideFlag == false) {
					powerArray[4].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.powered == false && powerArray[7].schedules.tempOverrideFlag == false) {
					powerArray[7].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.powered == false && powerArray[2].schedules.tempOverrideFlag == false) {
					powerArray[2].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.powered == false && powerArray[5].schedules.tempOverrideFlag == false) {
					powerArray[5].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
	}
}

void SubMenu::checkSchedule() {

}

void SubMenu::allOff() {
	for (int i = 0; i <= 7; i++) {
		powerArray[i].off();
	}
}


//TEMPORARY OVERRIDE FUNCTIONS (more below)//
void SubMenu::displayTemporaryOverrideDisplay() {
	subMenuDisplayObject.temporaryOverrideDisplay();
}

void SubMenu::displayTempOverrideStatus() {
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(22, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot].schedules.tempOverrideFlag ? "SET" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].schedules.tempOverrideFlag ? "SET" : "OFF");
	}
	subMenuDisplayObject.OLED.display();
}

void SubMenu::displayTempOverrideStatusDisplay() {
	subMenuDisplayObject.tempOverrideStatus();
}

void SubMenu::confirmClear(Relay* arrObj) {
	subMenuDisplayObject.confirmClearTempOverride();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_4) {
			if ((buttonByte & ROW_BITS) == ROW_1) {		//clear the flag and go back //Button A
				arrObj->clearTempOverrideFlag();
				break;
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {		//Cancel //Button B
				break;
			}
		}
	}
}

void SubMenu::tempOverrideStatusWhileLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte == NUM_PAD_1)) {
			if (powerArray[0].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[0].schedules.tempOverrideHour), (powerArray[0].schedules.tempOverrideMinute), (powerArray[0].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_2)) {
			if (powerArray[1].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[1].schedules.tempOverrideHour), (powerArray[1].schedules.tempOverrideMinute), (powerArray[1].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_3)) {
			if (powerArray[2].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[2].schedules.tempOverrideHour), (powerArray[2].schedules.tempOverrideMinute), (powerArray[2].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_4)) {
			if (powerArray[3].schedules.tempOverrideFlag == true) {
				if (powerArray[3].schedules.tempOverrideFlag == true) {
					subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[3].schedules.tempOverrideHour), (powerArray[3].schedules.tempOverrideMinute), (powerArray[3].schedules.tempOverrideState));
					waitForAnyLetterPress();
					return;
				}
			}
		}
		if ((buttonByte == NUM_PAD_5)) {
			if (powerArray[4].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[4].schedules.tempOverrideHour), (powerArray[4].schedules.tempOverrideMinute), (powerArray[4].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_6)) {
			if (powerArray[5].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[5].schedules.tempOverrideHour), (powerArray[5].schedules.tempOverrideMinute), (powerArray[5].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_7)) {
			if (powerArray[6].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[6].schedules.tempOverrideHour), (powerArray[6].schedules.tempOverrideMinute), (powerArray[6].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_8)) {
			if (powerArray[7].schedules.tempOverrideFlag == true) {
				subMenuDisplayObject.displaySingleObjectTempOverrideStatus((powerArray[7].schedules.tempOverrideHour), (powerArray[7].schedules.tempOverrideMinute), (powerArray[7].schedules.tempOverrideState));
				waitForAnyLetterPress();
				return;
			}
		}
		if ((buttonByte == NUM_PAD_D)) {
			return;
		}
	}
}

//INPUTTING MENUS//
int SubMenu::inputTime() {
	int NumberOfInputs = 2;
	int timeInput = 0;
	while (NumberOfInputs > 0) {
		while (1) {
			byte buttonByte = buttonPoll();
			if ((buttonByte == NUM_PAD_1)) {
				NumberOfInputs--;
				timeInput += (1 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_2)) {
				NumberOfInputs--;
				timeInput += (2 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_3)) {
				NumberOfInputs--;
				timeInput += (3 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_4)) {
				NumberOfInputs--;
				timeInput += (4 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_5)) {
				NumberOfInputs--;
				timeInput += (5 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_6)) {
				NumberOfInputs--;
				timeInput += (6 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_7)) {
				NumberOfInputs--;
				timeInput += (7 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_8)) {
				NumberOfInputs--;
				timeInput += (8 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_9)) {
				NumberOfInputs--;
				timeInput += (9 * (pow(10, NumberOfInputs)));
				break;
			}
			if ((buttonByte == NUM_PAD_0)) {
				NumberOfInputs--;
				timeInput += 0;
				break;
			}
			if (buttonByte == NUM_PAD_SHARP) {
				timeInput /= 10;
				subMenuDisplayObject.printTime(timeInput, NumberOfInputs);
				return timeInput;
			}
		}
		subMenuDisplayObject.printTime(timeInput, NumberOfInputs);
		delayWithoutDelay(175);
	}
	return timeInput;
}

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

int SubMenu::inputDuration() {
	int durationInput = 0;
	int loopCount = 0;
	while (1) {
		while (1) {
			byte buttonByte = buttonPoll();
			if ((buttonByte == NUM_PAD_1)) {
				if (loopCount == 0) {
					durationInput = 1;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 1;
				break;
			}
			if ((buttonByte == NUM_PAD_2)) {
				if (loopCount == 0) {
					durationInput = 2;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 2;
				break;
			}
			if ((buttonByte == NUM_PAD_3)) {
				if (loopCount == 0) {
					durationInput = 3;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 3;
				break;
			}
			if ((buttonByte == NUM_PAD_4)) {
				if (loopCount == 0) {
					durationInput = 4;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 4;
				break;
			}
			if ((buttonByte == NUM_PAD_5)) {
				if (loopCount == 0) {
					durationInput = 5;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 5;
				break;
			}
			if ((buttonByte == NUM_PAD_6)) {
				if (loopCount == 0) {
					durationInput = 6;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 6;
				break;
			}
			if ((buttonByte == NUM_PAD_7)) {
				if (loopCount == 0) {
					durationInput = 7;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 7;
				break;
			}
			if ((buttonByte == NUM_PAD_8)) {
				if (loopCount == 0) {
					durationInput = 8;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 8;
				break;
			}
			if ((buttonByte == NUM_PAD_9)) {
				if (loopCount == 0) {
					durationInput = 9;
					loopCount = 1;
					break;
				}
				durationInput *= 10;
				durationInput += 9;
				break;
			}
			if ((buttonByte == NUM_PAD_0)) {
				if (loopCount == 0) {
					continue;
				}
				durationInput *= 10;
				durationInput += 1;
				break;
			}
			if ((buttonByte == NUM_PAD_D)) {
				return durationInput;
			}
		}
		subMenuDisplayObject.printTime(durationInput);
		delayWithoutDelay(175);
	}
}

int SubMenu::verifyDuration(int dur) {
	if (dur <= 0 || dur > 1440)
		return -1;
	return dur;
}

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

void SubMenu::promptTempOverrideTime(int object) {
	//Get the starting HOUR//
	powerArray[object].schedules.tempOverrideHour = 0;
	powerArray[object].schedules.tempOverrideMinute = 0;
	powerArray[object].schedules.tempOverrideDuration = 0;
	subMenuDisplayObject.enterStartingHour();
	int hours = inputTime();
	hours = verifyHour(hours);
	if (hours == -1) {
		subMenuDisplayObject.invalidTime("hour");
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
		subMenuDisplayObject.invalidTime("minutes");
		delayWithoutDelay(1500);
		return;
	}
	powerArray[object].schedules.tempOverrideMinute = minutes;
	//Get the DURATION of the override//
	subMenuDisplayObject.enterDuration();
	int duration = inputDuration();
	duration = verifyDuration(duration);
	if (duration == -1) {
		subMenuDisplayObject.invalidTime("duration");
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

//END INPUTTING MENUS//


//TEMPORARY OVERRIDE FUNCTIONS CONTINUED (these are longer?)//
//Override currently only goes up to 24 hours in advance, probably should make it better
void SubMenu::chooseRelay() {
	int chosenArray = -1;
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if (powerArray[0].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[0]));
				return;
			}
			promptTempOverrideTime(0);
			return;
		}
		if (buttonByte == NUM_PAD_2) {
			if (powerArray[1].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[1]));
				return;
			}
			promptTempOverrideTime(1);
			return;
		}
		if (buttonByte == NUM_PAD_3) {
			if (powerArray[2].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[2]));
				return;
			}
			promptTempOverrideTime(2);
			return;
		}
		if (buttonByte == NUM_PAD_4) {
			if (powerArray[3].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[3]));
				return;
			}
			promptTempOverrideTime(3);
			return;
		}
		if (buttonByte == NUM_PAD_5) {
			if (powerArray[4].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[4]));
				return;
			}
			promptTempOverrideTime(4);
			return;
		}
		if (buttonByte == NUM_PAD_6) {
			if (powerArray[5].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[5]));
				return;
			}
			promptTempOverrideTime(5);
			return;
		}
		if (buttonByte == NUM_PAD_7) {
			if (powerArray[6].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[6]));
				return;
			}
			promptTempOverrideTime(6);
			return;
		}
		if (buttonByte == NUM_PAD_8) {
			if (powerArray[7].schedules.tempOverrideFlag == true) {
				confirmClear(&(powerArray[7]));
				return;
			}
			promptTempOverrideTime(7);
			return;
		}
		if (buttonByte == NUM_PAD_STAR) {
			return;
		}
	}
}

void SubMenu::displayTempOverrideSubMenu() {
	subMenuDisplayObject.tempOverrideSubMenu();
}