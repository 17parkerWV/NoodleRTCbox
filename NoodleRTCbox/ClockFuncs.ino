//LOOK AT COMMIT NOTES
//Updates clockSecondObj
void updateClock() {
	clockSecondObj = clockObj.now();
	return;
}
//Gets user input to set the RTC's date and time
void setTime() {
	prepDisp(1, 0, 0);
	uint8_t  month, day, hour, minute, second;
	uint16_t year, dayOfWeek;
	printHeader(F("Enter the month MM\nPress D to submit\nPress STAR to cancel"), 1, true);
	month = (uint8_t)inputTime();
	if (month < 1 || month >12) {
		dispError(F("invalid\nmonth"));
		return;
	}
	printHeader(F("Enter the day DD\nPress STAR to cancel"), 1, true);
	day = (uint8_t)inputTime();
	if (!validateDay(month, day)) {
		//dispError(F("Invalid\nday"));
		dispError((String)day);
		return;
	}
	printHeader(F("Enter the last 2\ndigits of the year\nEx: for 2021 enter 21\nPress STAR to cancel"), 1, true);
	year = (uint16_t)inputTime();
	if (year < 20) {
		dispError(F("Invalid\nyear"));
		return;
	}
	printHeader(F("Enter the hour HH\nUse 24hr notation\nPress D to submit\nPress STAR to cancel"), 1, true);
	hour = (uint8_t)inputTime();
	if (hour > 24 || hour < 0) {
		dispError(F("Invalid hour"));
		return;
	}
	printHeader(F("Enter the minute MM\nPress D to submit\nPress STAR to cancel"), 1, true);
	minute = (uint8_t)inputTime();
	if (minute < 0 || minute > 59) {
		dispError(F("Invalid\nminute"));
		return;
	}
	second = 0;
	uint16_t days = day;
	for (uint8_t i = 1; i < month; ++i)
		days += pgm_read_byte(daysInMonth + i - 1);
	if (month > 2 && year % 4 == 0)
		++days;
	days = days + 365 * year + (year + 3) / 4 - 1;
	clockObj.adjust(year, month, day, hour, minute, second, (uint8_t)days);
	dispError(F("Set"));
	return;
}
//Verify day is legal
bool validateDay(uint8_t month, uint8_t day) {
	if (day > 0) {
		switch (month) {
		case 1:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 2:
			if (day > 28) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 3:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 4:
			if (day > 30) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 5:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 6:
			if (day > 30) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 7:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 8:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 9:
			if (day > 30) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 10:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 11:
			if (day > 30) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		case 12:
			if (day > 31) {
				dispError(F("Invalid day"));
				return false;
			}
			return true;
			break;
		}
		return false;
	}
	return false;
}
//Update clockSecondObj and print the time in the top left corner
void dispCurrentTime() {
	updateClock();
	disp.fillRect(0, 0, 79, 16, BLACK);
	disp.display();
	prepDisp(2, 0, 0, false);
	disp.print(clockSecondObj.hour());
	disp.print(":");
	if (clockSecondObj.minute() < 10)
		disp.print(0);
	disp.print(clockSecondObj.minute());
	disp.display();
	return;
}
//Checks the current time and controls the relays depending on configuration
void timeControl() {
	int currentDay = clockSecondObj.day();
	int currentHour = clockSecondObj.hour();
	int currentMinute = clockSecondObj.minute();
	for (int index = 0; index <= 7; ++index) {
		if (relay[index].getOverrideStatus() == false && relay[index].getSchedSetFlag() == false)
			continue;
		if (relay[index].getOverrideStatus() == true) {
			//If inside range of override
			if ((relay[index].overrideHour < currentHour || (relay[index].overrideHour == currentHour && relay[index].overrideMinute <= currentMinute)) && (relay[index].overrideOffHour > currentHour || (relay[index].overrideOffHour == currentHour && relay[index].overrideOffMinute > currentMinute))) {
				relay[index].setOverrideStatus();
				digitalWrite(relay[index].relayPin, relay[index].overrideState);
				continue;
			}
			else {
				//If there isn't a schedule, change the power state to what the manual state was and continue
				//continue prevents it from checking schedule (there isn't a schedule anyway)
				//Also because I don't want extra functions to do similar things, the power will flicker off briefly when the override ends (if it should be on after it)
				relay[index].clearOverrideSetFlag();
				if (relay[index].getSchedSetFlag() == false) {
					digitalWrite(relay[index].relayPin, !relay[index].getManualPoweredStatus());
					continue;
				}
			}
		}
		//If a schedule is set, turn on/off
		//If sched is set overnight, time ON and time OFF were swapped, and the sched state is inverted (happened when sched was set)
		if (relay[index].getSchedSetFlag() == true) {
			if ((relay[index].hourOn < currentHour || (relay[index].hourOn == currentHour && relay[index].minuteOn <= currentMinute)) && (relay[index].hourOff > currentHour || (relay[index].hourOff == currentHour && relay[index].minuteOff > currentMinute)))
				digitalWrite(relay[index].relayPin, relay[index].schedState);
			else
				digitalWrite(relay[index].relayPin, !relay[index].schedState);
		}
	}
	return;
}