#include "SubMenus.h"

////------SUB MENU WHILE LOOPS------////
//Main Menu --> B --> 1. Waits for input. Displays status of temporary override flag (SET/OFF). Waits for parameters to new temp override, or clears if an override is active already


int inputTime() {
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
				printTime(timeInput, NumberOfInputs);
				return timeInput;
			}
			if (buttonByte == NUM_PAD_STAR)
				return -1;
		}
		printTime(timeInput, NumberOfInputs);
		delayWithoutDelay(175);
	}
	return timeInput;
}
int inputDuration() {
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
		printTime(durationInput);
		delayWithoutDelay(175);
	}
}
byte inputPowerState() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)
			return 0;
		if (buttonPress == NUM_PAD_0)
			return 1;
		if (buttonPress == NUM_PAD_SHARP)
			return -1;
	}
	return -1;
}
//Main Menu --> A --> 2. Waits for input. Displays current schedule on chosen relay



//Main Menu --> B --> 2. Waits for input. Displays the selected object's temporary Override status and variables, if a temporary override exists







//Menu that gets the answer to "Are you sure?"





////------NON-LOOPING MENU FUNCTIONS/SUB MENU FLOW CONTROL FUNCTIONS------////


//Turns off and sets appropriate flags for ALL relay objects
void allOff() {
	for (int i = 0; i <= 7; i++) {
		outlet[i]->off();
	}
	return;
}

//Initializes the relay objects with the physical pins they control



//Main Menu --> A. Time is displayed in the upper left. This prints the time when the time is right
void displayCurrentTime(int hour, int min) {
	disp.setTextSize(2);
	disp.setCursor(0, 0);
	disp.print(hour);
	disp.print(":");
	if (min < 10)
		disp.print(0);
	disp.print(min);
	disp.display();
}
//Displays the main menu. What else do you want from it?
void displayMainMenu() {

}
//Main Menu --> C --> 2. Waits for input. Flips the power state of the relay (ON/OFF) if (manualOverrideFlag == true && tempOverrideFlag == false)
void manualOnOffSubMenu() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (outlet[0]->getManualOverrideFlagStatus() == true || (outlet[0]->getTempOverrideFlag() == false && outlet[0]->getSchedFlag() == false)) {
					outlet[0]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (outlet[3]->getManualOverrideFlagStatus() == true || (outlet[3]->getTempOverrideFlag() == false && outlet[3]->getSchedFlag() == false)) {
					outlet[3]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (outlet[6]->getManualOverrideFlagStatus() == true || (outlet[6]->getTempOverrideFlag() == false && outlet[6]->getSchedFlag() == false)) {
					outlet[6]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (outlet[1]->getManualOverrideFlagStatus() == true || (outlet[1]->getTempOverrideFlag() == false && outlet[1]->getSchedFlag() == false)) {
					outlet[1]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				//Number 5 on the numpad
				if (outlet[4]->getManualOverrideFlagStatus() == true || (outlet[4]->getTempOverrideFlag() == false && outlet[4]->getSchedFlag() == false)) {
					outlet[4]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (outlet[7]->getManualOverrideFlagStatus() == true || (outlet[7]->getTempOverrideFlag() == false && outlet[7]->getSchedFlag() == false)) {
					outlet[7]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (outlet[2]->getManualOverrideFlagStatus() == true || (outlet[2]->getTempOverrideFlag() == false && outlet[2]->getSchedFlag() == false)) {
					outlet[2]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (outlet[5]->getManualOverrideFlagStatus() == true || (outlet[5]->getTempOverrideFlag() == false && outlet[5]->getSchedFlag() == false)) {
					outlet[5]->flipPowerState();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
	}
}
//FUNCTIONS FOR THE STRUCT
//Turns off and sets appropriate flags for the calling object
void outlets::off() {
	digitalWrite(relayPin, HIGH);
	clearPoweredState();
	clearSchedSetFlag();
	clearTempOverrideFlag();
	clearTempOverrideStarted();
	setManualOverrideFlag();
	return;
}





void outlets::flipPowerState() {
	bool power;
	manualOverrideFlag = true;
	powered = (!powered);		//Flip the state of the flag
	power = !powered;						//flip it back because the relays are active LOW
	digitalWrite(relayPin, power);	//Write power state to the pin, should mae sure this is bullet proof
	return;
}


void outlets::clearManualOverrideFlag() {
	manualOverrideFlag = false;
	powered = false;
	digitalWrite(relayPin, HIGH);
	return;
}


void outlets::clearTempOverrideFlag() {
	clearManualOverrideFlag();
	tempOverrideFlag = false;
	clearTempOverrideStarted();
	if (getSchedFlag() == false)
		digitalWrite(relayPin, HIGH);
	return;
}

//Causes a delay without using delay(), because it uses interrupts

//saves the state of the button as a byte, to be decoded elsewhere

//Menu that pauses sub menu flow until A, B, C, or D is pressed
void waitForAnyLetterPress() {
	delayWithoutDelay(175);
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4)
			return;
	}
	delayWithoutDelay(175);
	return;
}
////------END GENERAL UTILITY FUNCTIONS------////
/*
////------FUNCTIONS FOR THE ISR------////
//It is going to be typed out so that I do not have to put it in a FOR loop, it'll look gross though
//If the current time is WITHIN scheduled time, relay is ON
void SubMenu::timeControl(int currentDay, int currentHour, int currentMinute) {
	int relay;
	for (relay = 0; relay <= 7; relay++) {
		if (outlet[relay]->getTempOverrideStatus() == false && outlet[relay]->getScheduleSetFlagStatus() == false)
			continue;
		if (outlet[relay]->getTempOverrideStatus() == true) {
			byte tempState = outlet[relay]->getTempOverrideState();
			if ((outlet[relay]->tempOverrideHour <= currentHour) && (outlet[relay]->tempOverrideMinute <= currentMinute) && (outlet[relay]->tempOverrideOffHour >= currentHour) && (outlet[relay]->tempOverrideOffMinute > currentMinute)) {
				outlet[relay]->setTempOverrideStarted();
				outlet[relay]->clearPoweredState();
				digitalWrite(outlet[relay]->relayPin, outlet[relay]->getTempOverrideState());
			}
			if ((outlet[relay]->tempOverrideOffHour <= currentHour) && (outlet[relay]->tempOverrideOffMinute <= currentMinute)) {
				outlet[relay]->clearTempOverrideFlag();
				outlet[relay]->clearTempOverrideStarted();
				if (outlet[relay]->getScheduleSetFlagStatus() == false) {
					digitalWrite(outlet[relay]->relayPin, HIGH);
					continue;
				}
			}
		}
		if (outlet[relay]->getScheduleSetFlagStatus() == true) {
			byte schedState = outlet[relay]->getScheduleState();
			if ((outlet[relay]->hourOn <= currentHour) && (outlet[relay]->minuteOn <= currentMinute) && (outlet[relay]->hourOff >= currentHour) && (outlet[relay]->minuteOff > currentMinute)) {
				outlet[relay]->setPoweredState();
				digitalWrite(outlet[relay]->relayPin, schedState);
			}
			if ((outlet[relay]->hourOff <= currentHour) && (outlet[relay]->minuteOff <= currentMinute)) {
				outlet[relay]->clearPoweredState();
				digitalWrite(outlet[relay]->relayPin, !schedState);
			}
		}
	}
	return;
}
*/