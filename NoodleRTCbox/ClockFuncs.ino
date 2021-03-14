void updateClock() {
	clockSecondObj = clockObj.now();
	return;
}

void setTime() {
	prepDisp(1, 0, 0);
	uint8_t  month, day, hour, minute, second;
	uint16_t year, dayOfWeek;
	printHeader(F("Enter the month\nPress D to submit"), 1, true);
	month = (uint8_t)inputTime();
	if (month < 1 || month >12) {
		dispError(F("invalid\nmonth"));
		return;
	}
	printHeader(F("Enter the day"), 1, true);
	day = (uint8_t)inputTime();
	if (!validateDay(month, day)) {
		//dispError(F("Invalid\nday"));
		dispError((String)day);
		return;
	}
	printHeader(F("Enter the last 2\ndigits of the year\nEx: for 2021 enter 21"), 1, true);
	year = (uint16_t)inputTime();
	if (year < 20) {
		dispError(F("Invalid\nyear"));
		return;
	}
	printHeader(F("Enter the hour\nUse 24hr notation\nPress D to submit"), 1, true);
	hour = (uint8_t)inputTime();
	if (hour > 24 || hour < 0) {
		dispError(F("Invalid hour"));
		return;
	}
	printHeader(F("Enter the minute\nPress D to submit"), 1, true);
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
	return;
}

bool validateDay(uint8_t month, uint8_t day) {
	if (day > 0) {
		switch (month) {
		case 1:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 2:
			if (day > 28) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 3:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 4:
			if (day > 30) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 5:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 6:
			if (day > 30) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 7:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 8:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 9:
			if (day > 30) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 10:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 11:
			if (day > 30) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		case 12:
			if (day > 31) {
				dispError("Invalid day");
				return false;
			}
			return true;
			break;
		}
		return false;
	}
	return false;
}

void updateCurrentTime() {
	updateClock();
	dispCurrentTime(clockSecondObj.hour(), clockSecondObj.minute());
	return;
}

void dispCurrentTime(int hour, int min) {
	prepDisp(2, 0, 0, false);
	disp.print(hour);
	disp.print(":");
	if (min < 10)
		disp.print(0);
	disp.print(min);
	disp.display();
	return;
}

void timeControl() {
	int currentDay = clockSecondObj.day();
	int currentHour = clockSecondObj.hour();
	int currentMinute = clockSecondObj.minute();
	for (int index = 0; index <= 7; ++index) {
		if (relay[index].getOverrideStatus() == false && relay[index].getSchedSetFlag() == false)
			continue;
		if (relay[index].getOverrideStatus() == true) {
			//If inside range of override
			if ((relay[index].overrideHour <= currentHour) && (relay[index].overrideMinute <= currentMinute) && (relay[index].overrideOffHour >= currentHour) && (relay[index].overrideOffMinute > currentMinute)) {
				relay[index].setOverrideStatus();
				digitalWrite(relay[index].relayPin, relay[index].overrideState);
			}
			//If inside before but now outside, set powered status to what it was before
			if ((relay[index].overrideOffHour <= currentHour) && (relay[index].overrideOffMinute <= currentMinute)) {
				relay[index].clearOverrideSetFlag();
				if (relay[index].getSchedSetFlag() == false)
					digitalWrite(relay[index].relayPin, !relay[index].getManualPoweredStatus());
			}
			continue;
		}
		//If a schedule is set, turn on/off
		//If sched is set overnight, time ON and time OFF were swapped, and the sched state is inverted (happened when sched was set)
		if (relay[index].getSchedSetFlag() == true) {
			if ((relay[index].hourOn <= currentHour) && (relay[index].minuteOn <= currentMinute) && (relay[index].hourOff >= currentHour) && (relay[index].minuteOff > currentMinute))
				digitalWrite(relay[index].relayPin, relay[index].schedState);
			if ((relay[index].hourOff <= currentHour) && (relay[index].minuteOff <= currentMinute))
				digitalWrite(relay[index].relayPin, !relay[index].schedState);
		}
	}
	return;
}