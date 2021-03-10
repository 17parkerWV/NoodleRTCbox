void delayWithoutDelay(unsigned int time) {
	unsigned long delayTime = millis();
	while ((millis() - time) < delayTime) {
	}
	return;
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
	return;
}
//Main Menu -> C -> 1. Waits for input. Selecting a relay will flip the manualOverrideFlag, if (powered == scheduleSetFlag == tempOverrideFlag == false)
void enableDisableRelayLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (relay[0].getManualPoweredStatus() == false && relay[0].getSchedSetFlag() == false && relay[0].getOverrideSetFlag() == false) {
					relay[0].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (relay[3].getManualPoweredStatus() == false && relay[3].getSchedSetFlag() == false && relay[3].getOverrideSetFlag() == false) {
					relay[3].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (relay[6].getManualPoweredStatus() == false && relay[6].getSchedSetFlag() == false && relay[6].getOverrideSetFlag() == false) {
					relay[6].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (relay[1].getManualPoweredStatus() == false && relay[1].getSchedSetFlag() == false && relay[1].getOverrideSetFlag() == false) {
					relay[1].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (relay[4].getManualPoweredStatus() == false && relay[4].getSchedSetFlag() == false && relay[4].getOverrideSetFlag() == false) {
					relay[4].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (relay[7].getManualPoweredStatus() == false && relay[7].getSchedSetFlag() == false && relay[7].getOverrideSetFlag() == false) {
					relay[7].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (relay[2].getManualPoweredStatus() == false && relay[2].getSchedSetFlag() == false && relay[2].getOverrideSetFlag() == false) {
					relay[2].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (relay[5].getManualPoweredStatus() == false && relay[5].getSchedSetFlag() == false && relay[5].getOverrideSetFlag() == false) {
					relay[5].flipManualOverrideFlag();
					clearRelayUpdate();
					dispEightRelays(1);
				}
			}
		}
	}
	return;
}

void manualOnOffLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (relay[0].getManualOverrideEnabled() == true || (relay[0].getOverrideSetFlag() == false && relay[0].getSchedSetFlag() == false)) {
					relay[0].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (relay[3].getManualOverrideEnabled() == true || (relay[3].getOverrideSetFlag() == false && relay[3].getSchedSetFlag() == false)) {
					relay[3].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (relay[6].getManualOverrideEnabled() == true || (relay[6].getOverrideSetFlag() == false && relay[6].getSchedSetFlag() == false)) {
					relay[6].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				break;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (relay[1].getManualOverrideEnabled() == true || (relay[1].getOverrideSetFlag() == false && relay[1].getSchedSetFlag() == false)) {
					relay[1].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				//Number 5 on the numpad
				if (relay[4].getManualOverrideEnabled() == true || (relay[4].getOverrideSetFlag() == false && relay[4].getSchedSetFlag() == false)) {
					relay[4].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (relay[7].getManualOverrideEnabled() == true || (relay[7].getOverrideSetFlag() == false && relay[7].getSchedSetFlag() == false)) {
					relay[7].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (relay[2].getManualOverrideEnabled() == true || (relay[2].getOverrideSetFlag() == false && relay[2].getSchedSetFlag() == false)) {
					relay[2].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (relay[5].getManualOverrideEnabled() == true || (relay[5].getOverrideSetFlag() == false && relay[5].getSchedSetFlag() == false)) {
					relay[5].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
	}
	return;
}

void scheduleSetStatusLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (relay[0].getSchedSetFlag() == true) {
					dispSingleSchedStatus(0);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (relay[3].getSchedSetFlag() == true) {
					dispSingleSchedStatus(1);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (relay[6].getSchedSetFlag() == true) {
					dispSingleSchedStatus(2);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_STAR)
				return;
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if (buttonByte == NUM_PAD_2) {
				if (relay[1].getSchedSetFlag() == true) {
					dispSingleSchedStatus(3);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_5) {
				if (relay[4].getSchedSetFlag() == true) {
					dispSingleSchedStatus(4);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (relay[7].getSchedSetFlag() == true) {
					dispSingleSchedStatus(5);
					waitForAnyLetterPress();
					return;
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (relay[2].getSchedSetFlag() == true) {
					dispSingleSchedStatus(6);
					waitForAnyLetterPress();
					return;
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (relay[5].getSchedSetFlag() == true) {
					dispSingleSchedStatus(7);
					waitForAnyLetterPress();
					return;
				}
			}
		}
	}
	return;
}

void overrideStatusLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if (relay[0].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(0);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_2) {
			if (relay[1].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(1);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_3) {
			if (relay[2].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(2);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_4) {
			if (relay[3].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(3);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_5) {
			if (relay[4].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(4);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_6) {
			if (relay[5].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(5);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_7) {
			if (relay[6].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(6);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_8) {
			if (relay[7].getOverrideSetFlag() == true) {
				dispSingleOverrideStatus(7);
				waitForAnyLetterPress();
				return;
			}
		}
		if (buttonByte == NUM_PAD_STAR)
			return;
	}
	return;
}
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
	return;
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

int verifyHour(int hour) {
	if (hour == 24)
		hour = 0;
	if (hour < 0 || hour > 24)
		return -1;
	return hour;
}

int verifyMinute(int min) {
	if (min > 59 || min < 0)
		return -1;
	return min;
}

int verifyDuration(int dur) {
	if (dur <= 0 || dur > 1440)
		return -1;
	return dur;
}