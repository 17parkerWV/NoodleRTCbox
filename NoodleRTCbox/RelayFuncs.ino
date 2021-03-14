void initializeObjs() {
	if (!disp.begin(SSD1306_SWITCHCAPVCC, 0x3C))
		while (1);
	if (!clockObj.begin() || clockObj.lostPower()) 
		while (1);
	clockSecondObj = clockObj.now();
	clockObj.writeSqwPinMode(DS3231_SquareWave1Hz);	//Enable the 1Hz squarewave clock
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextColor(WHITE);
	for (int i = 0; i <= 7; ++i) {
		pinMode(relayArrayPins[i], OUTPUT);
		relay[i].relayPin = relayArrayPins[i];
		relay[i].off();
	}
	return;
}

void confirmClear(outlets& obj, void (outlets::* clearFunc)()) {
	dispConfirmClearFlag();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_4) {
			if (buttonByte == NUM_PAD_A) {
				(obj.*clearFunc)();
				dispError(F("Cleared"));
				break;
			}
			if (buttonByte == NUM_PAD_B)
				break;
		}
	}
	return;
}
int chooseRelay(int func) {
	bool (outlets:: * statusFunc)();
	void (outlets:: * clearFunc)();
	void (*promptFunc)(outlets&);
	switch (func) {
	case 1:
		statusFunc = &outlets::getOverrideSetFlag;
		clearFunc = &outlets::clearOverrideSetFlag;
		promptFunc = &promptOverrideTime;
		break;
	case 2:
		statusFunc = &outlets::getSchedSetFlag;
		clearFunc = &outlets::clearSchedSetFlag;
		promptFunc = &promptSchedTime;
		break;
	default:
		return -1;
	}
	int chosenArray = -1;
	while (1) {
		byte buttonByte = buttonPoll();
		if (buttonByte == NUM_PAD_1) {
			if ((relay[0].*statusFunc)() == true) {
				confirmClear(relay[0], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[0]);
			if (func == 2 && !relay[0].getManualPoweredStatus()) {
				promptFunc(relay[0]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_2) {
			if ((relay[1].*statusFunc)() == true) {
				confirmClear(relay[1], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[1]);
			if (func == 2 && !relay[1].getManualPoweredStatus()) {
				promptFunc(relay[1]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_3) {
			if ((relay[2].*statusFunc)() == true) {
				confirmClear(relay[2], clearFunc);
				return 0;
			}
			if (func == 2)
				promptFunc(relay[0]);
			if (func == 2 && !relay[2].getManualPoweredStatus()) {
				promptFunc(relay[2]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_4) {
			if ((relay[3].*statusFunc)() == true) {
				confirmClear(relay[3], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[3]);
			if (func == 2 && !relay[3].getManualPoweredStatus()) {
				promptFunc(relay[3]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_5) {
			if ((relay[4].*statusFunc)() == true) {
				confirmClear(relay[4], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[4]);
			if (func == 2 && !relay[4].getManualPoweredStatus()) {
				promptFunc(relay[4]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_6) {
			if ((relay[5].*statusFunc)() == true) {
				confirmClear(relay[5], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[5]);
			if (func == 2 && !relay[5].getManualPoweredStatus()) {
				promptFunc(relay[5]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_7) {
			if ((relay[6].*statusFunc)() == true) {
				confirmClear(relay[6], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[6]);
			if (func == 2 && !relay[6].getManualPoweredStatus()) {
				promptFunc(relay[6]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_8) {
			if ((relay[7].*statusFunc)() == true) {
				confirmClear(relay[7], clearFunc);
				return 0;
			}
			if (func == 1)
				promptFunc(relay[7]);
			if (func == 2 && !relay[7].getManualPoweredStatus()) {
				promptFunc(relay[7]);
				return 0;
			}
		}
		if (buttonByte == NUM_PAD_STAR)
			return 0;
	}
	return 0;
}

void completeOffLoop() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[0].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_2) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[1].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_3) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[2].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_4) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[3].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_5) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[4].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_6) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[5].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_7) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[6].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_8) {
			bool confirmed = confirmationMenu();
			if (confirmed == false)
				return;
			if (confirmed == true) {
				relay[7].off();
				dispError(F("Cleared"));
				return;
			}
		}
		if (buttonPress == NUM_PAD_STAR) {
			return;
		}
	}
	return;
}

byte confirmationMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4) {
			if (buttonPress == NUM_PAD_A)
				return true;
			if (buttonPress == NUM_PAD_B)
				return false;
		}
	}
	return;
}
void promptSchedTime(outlets& obj) {
	//RESET THE TIME
	obj.hourOn = 0;
	obj.minuteOn = 0;
	obj.hourOff = 0;
	obj.minuteOff = 0;
	dispEnterTime((F("Enter hour to TURN ONusing 24 hour format Press STAR to cancel Press D when done")), (F("24 HOUR FORMAT ONLY")));
	int startHour = inputTime();
	if (startHour == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyHour(startHour) == -1) {
		dispError(F("Invalid\nhour"));
		return;
	}
	dispEnterTime((F("Enter minute to TURN ON   * - back\nPress D when done")), F(""));
	int startMinute = inputTime();
	if (startMinute == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyMinute(startMinute) == -1) {
		dispError(F("Invalid\nminute"));
		return;
	}
	dispEnterTime((F("What hour should it  turn off? Press STAR to cancel Press D    when done")), (F("24 HOUR FORMAT ONLY")));	//I think this is too long but we'll see
	int stopHour = inputTime();
	if (stopHour == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyHour(stopHour) == -1) {
		dispError(F("Invalid\nhour"));
		return;
	}
	dispEnterTime((F("Enter minute to TURN OFF   * - back\nPress D when done")), F(""));
	int stopMinute = inputTime();
	if (stopMinute == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyMinute(stopMinute) == -1) {
		dispError(F("Invalid\nminute"));
		return;
	}
	//FINAL INPUT CHECKS
	if ((startHour == stopHour) && (startMinute == stopMinute)) {
		dispError(F("Invalid Input"));
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
	obj.schedState = powerState;
	obj.minuteOff = stopMinute;
	obj.hourOn = startHour;
	obj.hourOff = stopHour;
	obj.minuteOn = startMinute;
	dispError(F("Set"));
	obj.setSchedFlag();
	return;
}

void promptOverrideTime(outlets& obj) {
	//Get the starting HOUR//
	obj.overrideHour = 0;
	obj.overrideMinute = 0;
	obj.overrideDuration = 0;
	dispEnterTime((F("Enter starting hour  using 24 hour format Press STAR to cancel Press D when done")), (F("24 HOUR FORMAT ONLY")));
	int hour = inputTime();
	if (hour == -1) {
		dispError(F("Canceled"));
		delayWithoutDelay(1500);
		return;
	}
	if (verifyHour(hour) == -1) {
		dispError(F("Invalid\nhour"));
		delayWithoutDelay(1500);
		return;
	}
	//Get the starting MINUTE//
	dispEnterTime((F("Enter starting\nminutes   * - back\nPress D when done")), F(""));
	int minute = inputTime();
	if (minute == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyMinute(minute) == -1) {
		dispError(F("Invalid\nminutes"));
		return;
	}
	//Get the DURATION of the override//
	dispEnterDuration();
	int duration = inputDuration();
	if (duration == -1) {
		dispError(F("Canceled"));
		return;
	}
	if (verifyDuration(duration) == -1) {
		dispError(F("Invalid\nDuration"));
		return;
	}
	int minuteOff = (duration + minute) % 60;	//stores the minute it will turn OFF;
	int tempHourVar = (duration / 60);
	if ((duration % 60 + minute) > 59)
		++tempHourVar;		//Add one hour if the minutes overflow
	int hourOff = (hour + tempHourVar) % 24;
	//Get the POWER STATE//
	dispEnterPowerState();
	byte powerState = inputPowerState();
	if (powerState == -1) {
		dispError(F("Canceled"));
		return;
	}
	//FINAL INPUT CHECKS
	if (hour == hourOff && minute == minuteOff) {
		dispError(F("Invalid Input"));
		return;
	}
	//Set the flag and swap the times
	if ((hour > hourOff) || (hour >= hourOff && minute > minuteOff)) {
		powerState = true;
		int tempMinute = minute;
		minute = minuteOff;
		minuteOff = tempMinute;
		int tempHour = hour;
		hour = hourOff;
		hourOff = tempHour;
	}
	obj.overrideDuration = duration;
	obj.overrideState = powerState;
	obj.overrideOffMinute = minuteOff;
	obj.overrideOffHour = hourOff;
	obj.overrideHour = hour;
	obj.overrideMinute = minute;
	//FINALIZE - set the flag
	dispError(F("Set"));
	obj.setOverrideSetFlag();
	return;
}