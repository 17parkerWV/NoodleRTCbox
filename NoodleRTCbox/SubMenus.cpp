#include "SubMenus.h"

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
				if (powerArray[0].schedules.powered == false && powerArray[0].schedules.scheduleSetFlag == false) {
					powerArray[0].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.powered == false && powerArray[3].schedules.scheduleSetFlag == false) {
					powerArray[3].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.powered == false && powerArray[6].schedules.scheduleSetFlag == false) {
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
				if (powerArray[1].schedules.powered == false && powerArray[1].schedules.scheduleSetFlag == false) {
					powerArray[1].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.powered == false && powerArray[4].schedules.scheduleSetFlag == false) {
					powerArray[4].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.powered == false && powerArray[7].schedules.scheduleSetFlag == false) {
					powerArray[7].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.powered == false && powerArray[2].schedules.scheduleSetFlag == false) {
					powerArray[2].flipManualOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.powered == false && powerArray[5].schedules.scheduleSetFlag == false) {
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
				if (powerArray[0].schedules.manualOverrideFlag == true) {
					powerArray[0].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.manualOverrideFlag == true) {
					powerArray[3].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.manualOverrideFlag == true) {
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
				if (powerArray[1].schedules.manualOverrideFlag == true) {
					powerArray[1].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.manualOverrideFlag == true) {
					powerArray[4].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.manualOverrideFlag == true) {
					powerArray[7].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.manualOverrideFlag == true) {
					powerArray[2].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.manualOverrideFlag == true) {
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
				if (powerArray[0].schedules.powered == false) {
					powerArray[0].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.powered == false) {
					powerArray[3].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.powered == false) {
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
				if (powerArray[1].schedules.powered == false) {
					powerArray[1].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.powered == false) {
					powerArray[4].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.powered == false) {
					powerArray[7].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.powered == false) {
					powerArray[2].flipScheduleSetFlag();
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.powered == false) {
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


//TEMPORARY OVERRIDE FUNCTIONS
void SubMenu::displayTemporaryOverrideDisplay() {
	subMenuDisplayObject.temporaryOverrideDisplay();
}

void SubMenu::displayTemporaryOverrideStatus() {
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

void SubMenu::confirmClear(Relay* arrObj) {
	subMenuDisplayObject.confirmClearDisplay();
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


//INPUTTING MENUS//
int SubMenu::inputTime() {
	subMenuDisplayObject.startingHour();
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
			if ((buttonByte == NUM_PAD_SHARP)) {
				return -1;
			}
		}
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

int SubMenu::durationInput() {

}

//END INPUTTING MENUS//

void SubMenu::chooseRelay() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[0]));
					return;	//returns to chooseArray(), back to mainMenu
				}

				//Get the starting HOUR
				subMenuDisplayObject.enterStartingHour();
				int hours = inputTime();
				hours = verifyHour(hours);
				if (hours == -1) {
					subMenuDisplayObject.invalidTime("hour");
					return;
				}
				//Get the starting MINUTE
				subMenuDisplayObject.enterStartigMinute();
				powerArray[0].schedules.tempOverrideHour = hours;
				int minutes = inputTime();
				minutes = verifyMinute(minutes);
				if (minutes == -1) {
					subMenuDisplayObject.invalidTime("minutes");
					return;
				}
				powerArray[0].schedules.tempOverrideMinute = minutes;
				//Get the DURATION of the override
				subMenuDisplayObject.enterDuration();
				
				//Get the POWER STATE
				subMenuDisplayObject.enterPowerState();

				//FINALIZE - set the flag

				return;
			}

			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[3]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}

			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[6]));
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
				if (powerArray[1].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[1]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[4]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[7]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[2]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.tempOverrideFlag == true) {
					confirmClear(&(powerArray[5]));
					subMenuDisplayObject.clearRelayUpdate();
					displayScheduleSetFlagStatus();
				}
			}
		}
	}
}