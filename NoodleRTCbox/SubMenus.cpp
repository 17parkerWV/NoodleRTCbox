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
		subMenuDisplayObject.OLED.println(powerArray[spot].schedules.overrideFlag ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		subMenuDisplayObject.OLED.setCursor(86, 16 + (spot * 12));
		subMenuDisplayObject.OLED.println(powerArray[spot + 4].schedules.overrideFlag ? "ON" : "OFF");
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
					powerArray[0].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.powered == false && powerArray[3].schedules.scheduleSetFlag == false) {
					powerArray[3].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.powered == false && powerArray[6].schedules.scheduleSetFlag == false) {
					powerArray[6].flipOverrideState();
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
					powerArray[1].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.powered == false && powerArray[4].schedules.scheduleSetFlag == false) {
					powerArray[4].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].schedules.powered == false && powerArray[7].schedules.scheduleSetFlag == false) {
					powerArray[7].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.powered == false && powerArray[2].schedules.scheduleSetFlag == false) {
					powerArray[2].flipOverrideState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOverrideScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.powered == false && powerArray[5].schedules.scheduleSetFlag == false) {
					powerArray[5].flipOverrideState();
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
				if (powerArray[0].schedules.overrideFlag == true) {
					powerArray[0].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].schedules.overrideFlag == true) {
					powerArray[3].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].schedules.overrideFlag == true) {
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
				if (powerArray[1].schedules.overrideFlag == true) {
					powerArray[1].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].schedules.overrideFlag == true) {
					powerArray[4].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[8].schedules.overrideFlag == true) {
					powerArray[7].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].schedules.overrideFlag == true) {
					powerArray[2].flipPowerState();
					subMenuDisplayObject.clearRelayUpdate();
					displayOnOffScreenStatus();
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].schedules.overrideFlag == true) {
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
				if (powerArray[8].schedules.powered == false) {
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

void SubMenu::allOff() {
	for (int i = 0; i <= 7; i++) {
		powerArray[i].off();
	}
}