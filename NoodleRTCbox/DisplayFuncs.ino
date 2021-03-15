//LOOK AT COMMIT NOTES
//Moves the curor, sets font size (and clears if flag is true)
void prepDisp(int fontSize = 1, int x = 0, int y = 0, bool clear = true) {
	if (clear) {
		disp.clearDisplay();
		disp.display();
	}
	disp.setTextSize(fontSize);
	disp.setCursor(x, y);
	return;
}
//Prints menu options (and/or header/footer) - only enough space for one option to take up more than one line
void printMenu(int argc, const String label[], String m1, String m2 = "", String m3 = "", String m4 = "", bool header = false, String headerm = "", bool footer = false, String footerm = "", int fontSize = 2) {
	disp.clearDisplay();
	disp.display();
	messArr[0] = m1;
	messArr[1] = m2;
	messArr[2] = m3;
	messArr[3] = m4;
	if (header == true) {
		prepDisp(fontSize, 0, 0);
		disp.print(headerm);
	}
	if (footer == true) {
		prepDisp(1, 0, 56, false);
		disp.print(footerm);
	}
	prepDisp(1, 0, 16, false);
	for (int i = 0; i < argc; ++i) {
		disp.print(label[i]);
		disp.println(messArr[i]);
	}
	disp.display();
}
//Displays the RTC's date and time
void dispTime() {
	prepDisp(1, 0, 0);
	disp.print(F("month:\nday:\nyear:\nDotW:\nTime:"));
	disp.setCursor(50, 0);
	disp.print(clockSecondObj.month());
	disp.setCursor(50, 8);
	disp.print(clockSecondObj.day());
	disp.setCursor(50, 16);
	disp.print(clockSecondObj.year());
	disp.setCursor(50, 24);
	disp.print(daysOfWeek[clockSecondObj.dayOfTheWeek()]);
	disp.setCursor(50, 32);
	disp.print(clockSecondObj.hour());
	disp.print(F(":"));
	disp.print(clockSecondObj.minute());
	disp.setCursor(0, 52);
	disp.print(F("Press A-D to go back"));
	disp.display();
	return;
}
//displays the 8 relays and the status of any of their settings
void dispEightRelays(int mode, String message = "", bool header = false) {
	if (header == true) {
		prepDisp(1, 0, 16);
		disp.print(F("1: "));		//1
		disp.setCursor(0, 28);		//2
		disp.print(F("2: "));
		disp.setCursor(0, 40);		//3
		disp.print(F("3: "));
		disp.setCursor(0, 52);		//4
		disp.print(F("4: "));
		disp.setCursor(64, 16);		//5
		disp.print(F("5: "));
		disp.setCursor(64, 28);		//6
		disp.print(F("6: "));
		disp.setCursor(64, 40);		//7
		disp.print(F("7: "));
		disp.setCursor(64, 52);		//8
		disp.print(F("8: "));
		disp.display();
		printHeader(message);
		disp.display();
	}
	switch (mode) {
	case 2:
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(22, 16 + (spot * 12));
			disp.println(relay[spot].getManualPoweredStatus() ? "ON" : "OFF");
		}
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(86, 16 + (spot * 12));
			disp.println(relay[spot + 4].getManualPoweredStatus() ? "ON" : "OFF");
		}
		break;
	case 3:
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(22, 16 + (spot * 12));
			disp.println(relay[spot].getOverrideSetFlag() ? "ON" : "OFF");
		}
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(86, 16 + (spot * 12));
			disp.println(relay[spot + 4].getOverrideSetFlag() ? "ON" : "OFF");
		}
		break;
	case 4:
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(22, 16 + (spot * 12));
			disp.println(relay[spot].getSchedSetFlag() ? "ON" : "OFF");
		}
		for (int spot = 0; spot <= 3; ++spot) {
			disp.setCursor(86, 16 + (spot * 12));
			disp.println(relay[spot + 4].getSchedSetFlag() ? "ON" : "OFF");
		}
		break;
	case 9:
		break;
	default:
		return;
	}
	disp.display();
	return;
}
//displays a message starting at 0,0 - can change font size and can clear display before printing
void printHeader(String message, int fontSize = 1, bool clear = false) {
	prepDisp(fontSize, 0, 0, clear);
	disp.print(message);
	disp.display();
	return;
}
//Specialized display - display for entering duration of override
void dispEnterDuration() {
	prepDisp(1, 0, 0);
	disp.print(F("Enter duration of\noverride in minutes\npress * to cancel\nPress 'D' when done"));
	disp.fillRect(0, 40, 128, 48, BLACK);
	disp.display();
	return;
}
//Displays digits on the screen as they are being pressed. Used with inputTime and inputDuration
void printTime(int time, int loopCount = 0) {
	disp.setTextSize(2);
	int xRect;
	int xPrint = 32;
	switch (loopCount) {
	case 1000:
		xRect = 42;
		break;
	case 100:
		xRect = 54;
		break;
	case 10:
		xRect = 67;
		break;
	case 1:
		xRect = 80;
		break;
	case 0:
		xPrint = 43;
		xRect = 53;
		break;
	default:
		xRect = 54;
		break;
	}
	disp.setCursor(xPrint, 35);
	disp.print(time);
	disp.fillRect(xRect, 32, 108 - xRect + xPrint, 20, BLACK);
	disp.display();
	return;
}
//inaccurate function name. prints message on the top and/or the bottom, and draws a black box in the middle
void dispEnterTime(String top, String bottom = "") {
	prepDisp(1, 0, 0);
	disp.print(top);
	disp.fillRect(0, 32, 128, 48, BLACK);
	disp.setCursor(0, 52);
	disp.print(bottom);
	disp.display();
	return;
}
//Displays a message with "returning"
void dispError(String error) {
	prepDisp(2, 0, 0);
	disp.print(error);
	prepDisp(1, 32, 38, false);
	disp.print(F("returning..."));
	disp.display();
	delayWithoutDelay(1200);
	return;
}
//When 8 relays are displayed, this clears ONLY the ON/SET/OFF or whatever the status is
void clearRelayUpdate() {
	disp.fillRect(22, 16, 24, 48, BLACK);
	disp.fillRect(86, 16, 24, 48, BLACK);
	disp.display();
	return;
}
//Displays chosen relay's override specs
void dispSingleOverrideStatus(int relayNum) {
	prepDisp(1, 0, 0);
	disp.print(F("Current Override"));
	prepDisp(2, 0, 16, false);
	if (relay[relayNum].overrideHour > 12)
		disp.print(relay[relayNum].overrideHour - 12);
	else
		disp.print(relay[relayNum].overrideHour);
	disp.print(F(":"));
	if (relay[relayNum].overrideMinute < 10)
		disp.print(0);
	disp.print(relay[relayNum].overrideMinute);
	disp.setTextSize(1);
	disp.println();
	if (relay[relayNum].overrideHour < 12 && relay[relayNum].overrideHour >= 0)
		disp.println(F("          AM"));
	if (relay[relayNum].overrideHour >= 12)
		disp.println(F("          PM"));
	disp.print(F("Forced: "));
	disp.println((relay[relayNum].overrideState == 0) ? "ON" : "OFF");
	disp.print(F("for "));
	disp.print(relay[relayNum].overrideDuration);
	disp.print(F(" minutes"));
	disp.setCursor(0, 56);
	disp.print(F("press A-D to go back"));
	disp.display();
	return;
}
//Displays chosen relay's schedule specs
void dispSingleSchedStatus(int relayNum) {
	prepDisp(1, 0, 0);
	disp.print(F("Selected outlet: "));
	disp.setTextSize(2);
	disp.print(relayNum + 1);
	prepDisp(1, 0, 21, false);
	disp.print(F("Turns ON:  "));
	if (relay[relayNum].hourOn > 12)
		disp.print(relay[relayNum].hourOn - 12);
	else
		disp.print(relay[relayNum].hourOn);
	disp.print(F(":"));
	if (relay[relayNum].minuteOn < 10)
		disp.print(0);
	disp.print(relay[relayNum].minuteOn);
	if (relay[relayNum].hourOn < 12 && relay[relayNum].hourOn >= 0) {
		disp.print(F(" AM"));
	}
	if (relay[relayNum].hourOn >= 12) {
		disp.print(F(" PM"));
	}
	disp.setCursor(0, 39);
	disp.print(F("Turns OFF:  "));
	if (relay[relayNum].hourOff > 12)
		disp.print(relay[relayNum].hourOff - 12);
	else
		disp.print(relay[relayNum].hourOff);
	disp.print(F(":"));
	if (relay[relayNum].minuteOff < 10)
		disp.print(0);
	disp.print(relay[relayNum].minuteOff);
	if (relay[relayNum].hourOff < 12 && relay[relayNum].hourOff >= 0) {
		disp.print(F(" AM"));
	}
	if (relay[relayNum].hourOff >= 12) {
		disp.print(F(" PM"));
	}
	disp.setCursor(0, 56);
	disp.print(F("press A-D to go back"));
	disp.display();
	return;
}