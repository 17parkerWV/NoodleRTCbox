// //Main Menu --> A. Goes into options regarding set schedules
void prepDisp(int fontSize = 1, int x = 0, int y = 0, bool clear = true) {
	if (clear) {
		disp.clearDisplay();
		disp.display();
	}
	disp.setTextSize(fontSize);
	disp.setCursor(x, y);
	return;
}

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
bool dispConfirmation() {
	prepDisp(1, 0, 42, false);
	disp.print(F("A: Yes        B: No"));
	disp.display();
	byte button;
	while (1) {
		button = buttonPoll();
		if (button == NUM_PAD_A)
			return true;
		if (button == NUM_PAD_B)
			return false;
	}
	return false;
}
void printHeader(String message, int fontSize = 1, bool clear = false) {
	prepDisp(fontSize, 0, 0, clear);
	disp.print(message);
	disp.display();
	return;
}
void dispEnterDuration() {
	prepDisp(1, 0, 0);
	disp.print(F("Enter duration of\noverride in minutes\npress * to cancel\nPress 'D' when done"));
	disp.fillRect(0, 40, 128, 48, BLACK);
	disp.display();
	return;
}

void printTime(int time, int loopCount = 0) {
	disp.setTextSize(2);
	int xRect;
	int xPrint = 32;
	switch (loopCount) {
	case 1000:
		xRect = 42;
		break;
	case 100:
		xRect = 53;
		break;
	case 10:
		xRect = 65;
		break;
	case 1:
		xRect = 77;
		break;
	case 0:
		xPrint = 43;
		xRect = 53;
		break;
	default:
		xRect = 52;
		break;
	}
	disp.setCursor(xPrint, 35);
	disp.print(time);
	disp.fillRect(xRect, 32, 108 - xRect + xPrint, 20, BLACK);
	disp.display();
	return;
}
void dispEnterTime(String top, String bottom) {
	prepDisp(1, 0, 0);
	disp.print(top);
	disp.fillRect(0, 32, 128, 48, BLACK);
	disp.setCursor(0, 52);
	disp.print(bottom);
	disp.display();
	return;
}
void dispEnterPowerState() {
	prepDisp(1, 0, 0);
	disp.print(F("Should it be forced\nON or OFF?\n1 means ON\n0 means OFF"));
	disp.display();
	return;
}
void dispConfirmClearFlag() {
	prepDisp(1, 0, 0);
	disp.print(F("Something is already set, would you like  to clear it?\nPress A to clear\nPress B to cancel "));
	disp.display();
	return;
}
void dispError(String error) {
	prepDisp(2, 0, 0);
	disp.print(error);
	prepDisp(1, 32, 38, false);
	disp.print(F("returning..."));
	disp.display();
	delayWithoutDelay(1200);
	return;
}

void dispOverrideMenu() {
	prepDisp(1, 0, 0);
	disp.println(F("Temporarily override any set schedule!"));
	disp.print(F("Press # to go back"));
	disp.setCursor(0, 32);
	disp.print(F("1: Set/Clear\n   temp. override"));
	disp.setCursor(0, 50);
	disp.print(F("2: Show single status"));
	disp.display();
	return;
}
void dispSchedulesMenu() {
	prepDisp(1, 80, 8);
	disp.print(F("# - Back"));
	disp.setCursor(0, 16);
	disp.print(F("1: Set/Clear Schedule"));
	disp.setCursor(0, 25);
	disp.print(F("2: View schedule"));
	disp.setCursor(0, 34);
	disp.print(F("3: Nothing here..."));
	disp.setCursor(0, 52);
	disp.print(F("4: Complete disable"));
	disp.display();
	return;
}
void dispMainMenu() {
	prepDisp(2, 10, 0);
	disp.print(F("Main Menu"));
	prepDisp(1, 0, 16, false);
	disp.print("A: Adjust Schedule");
	disp.setCursor(0, 27);
	disp.print(F("B: Temporary Override"));
	disp.setCursor(0, 38);
	disp.print(F("C: Manual Override\n   Menu"));
	disp.setCursor(0, 56);
	disp.print(F("D: Set date and time"));
	disp.display();
	return;
}
//Main Menu --> A. Time is displayed in the upper left. When it comes time to update it, this erases just the time, so the whole screen does not flash
void clearCurrentTime() {
	disp.fillRect(0, 0, 79, 16, BLACK);
	disp.display();
	return;
}

void clearRelayUpdate() {
	int x = 22;
	int y = 16;
	for (int rect = 0; rect <= 3; ++rect)
		disp.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	x = 86;
	y = 16;
	for (int rect = 0; rect <= 3; ++rect)
		disp.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	disp.display();
	return;
}

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