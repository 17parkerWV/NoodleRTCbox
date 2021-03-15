//LOOK AT COMMIT NOTES
//delay without using the arduino interrupts
void delayWithoutDelay(unsigned int time) {
	unsigned long delayTime = millis();
	while ((millis() - time) < delayTime) {}
	return;
}
//waits for button press and returns it - does not display anything
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
//Pauses until A, B, C or D is pressed
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
//Choose a relay to toggle on/off, if a schedule does not exist for the relay
void manualOnOffLoop() {
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if (buttonByte == NUM_PAD_1) {
				if (relay[0].getSchedSetFlag() == false) {
					relay[0].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_4) {
				if (relay[3].getSchedSetFlag() == false) {
					relay[3].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_7) {
				if (relay[6].getSchedSetFlag() == false) {
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
				if (relay[1].getSchedSetFlag() == false) {
					relay[1].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_5) {
				//Number 5 on the numpad
				if (relay[4].getSchedSetFlag() == false) {
					relay[4].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_8) {
				if (relay[7].getSchedSetFlag() == false) {
					relay[7].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if (buttonByte == NUM_PAD_3) {
				if (relay[2].getSchedSetFlag() == false) {
					relay[2].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
			if (buttonByte == NUM_PAD_6) {
				if (relay[5].getSchedSetFlag() == false) {
					relay[5].flipManualPoweredStatus();
					clearRelayUpdate();
					dispEightRelays(2);
				}
			}
		}
	}
	return;
}
//Choose relay - displays schedule status (if set)
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
//Choose relay - displays override status (if set)
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
//Takes input for time (and other 2 digit inputs) - displays the numbers pressed as they are pressed
int inputTime() {
	int NumberOfInputs = 100;
	int timeInput = 0;
	byte leadingZero = 0;
	while (NumberOfInputs > 0) {
		while (1) {
			byte buttonByte = buttonPoll();
			if (buttonByte == NUM_PAD_1) {
				timeInput += (1 * NumberOfInputs/10);
				break;
			}
			if (buttonByte == NUM_PAD_2) {
				timeInput += (2 * NumberOfInputs/10);
				break;
			}
			if (buttonByte == NUM_PAD_3) {
				timeInput += (3 * NumberOfInputs/10);
				break;
			}
			if (buttonByte == NUM_PAD_4) {
				timeInput += (4 * NumberOfInputs / 10);
				break;
			}
			if (buttonByte == NUM_PAD_5) {
				timeInput += (5 * NumberOfInputs / 10);
				break;
			}
			if (buttonByte == NUM_PAD_6) {
				timeInput += (6 * NumberOfInputs / 10);
				break;
			}
			if (buttonByte == NUM_PAD_7) {
				timeInput += (7 * NumberOfInputs / 10);
				NumberOfInputs -= 9;
				break;
			}
			if (buttonByte == NUM_PAD_8) {
				timeInput += (8 * NumberOfInputs / 10);
				break;
			}
			if (buttonByte == NUM_PAD_9) {
				timeInput += (9 * NumberOfInputs / 10);
				break;
			}
			if (buttonByte == NUM_PAD_0) {
				++leadingZero;
				break;
			}
			if (buttonByte == NUM_PAD_D) {
				//This can only be chosen on the first loop; must divide by 10
				timeInput /= 10;
				delayWithoutDelay(225);
				return timeInput;
			}
			if (buttonByte == NUM_PAD_STAR)
				return -1;
		}
		printTime(timeInput, (leadingZero == 2 && NumberOfInputs == 100) ? 0 : NumberOfInputs * 10);
		NumberOfInputs -= 90;
		++leadingZero;
		delayWithoutDelay(225);
	}
	return timeInput;
}
//Takes input for duration - prints number as numbers are pressed
int inputDuration() {
	int durationInput = 0;
	int loopCount = 1000;
	while (loopCount >= 10) {
		//When loopCount = 1000 to start, the condiiton was (loopCount >= 1) and relied on integer math to set it to zero, which I didn't like
		while (1) {
			byte buttonByte = buttonPoll();
			if (buttonByte == NUM_PAD_1) {
				durationInput += 1 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_2) {
				durationInput += 2 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_3) {
				durationInput += 3 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_4) {
				durationInput += 4 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_5) {
				durationInput += 5 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_6) {
				durationInput += 6 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_7) {
				durationInput += 7 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_8) {
				durationInput += 8 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_9) {
				durationInput += 9 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_0) {
				durationInput += 0 * loopCount / 10;
				break;
			}
			if (buttonByte == NUM_PAD_D)
				//Be careful, integer math is all around us...
				return ((durationInput * 10) / loopCount);
			if (buttonByte == NUM_PAD_STAR)
				return -1;
		}
		loopCount /= 10;
		printTime(durationInput, loopCount*10);
		delayWithoutDelay(175);
	}
	return durationInput;
}
//Waits for 1 (on), 0 (off), or # (cancel)
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
//Verify hour is legal
int verifyHour(int& hour) {
	if (hour == 24)
		hour = 0;
	if (hour < 0 || hour > 24)
		return -1;
	return hour;
}
//Verify minute is legal
int verifyMinute(int& min) {
	if (min > 59 || min < 0)
		return -1;
	return min;
}
//Verify duration is legal
int verifyDuration(int& dur) {
	if (dur <= 0 || dur > 1000)
		return -1;
	return dur;
}