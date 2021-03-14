// //Main Menu --> A. Goes into options regarding set schedules
void dispEightRelays(int mode, String message = "", bool header = false) {
	if (header == true) {
		disp.clearDisplay();
		disp.display();
		disp.setTextSize(1);
		disp.setCursor(0, 16);		//1
		disp.print(F("1: "));
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
void dispConfirmation() {
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(2);
	disp.print(F("Are you   sure?"));
	disp.setTextSize(1);
	disp.setCursor(0, 42);
	disp.print(F("A: Yes        B: No"));
	disp.display();
	return;
}
void printHeader(String message) {
	disp.setTextSize(1);
	disp.setCursor(0, 0);
	disp.print(message);
	disp.display();
	return;
}
void dispEnterDuration() {
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(1);
	disp.setCursor(0, 0);
	disp.print(F("Enter duration of\noverride in minutes\npress * to cancel\nPress 'D' when done"));
	disp.fillRect(0, 40, 128, 48, BLACK);
	disp.display();
	return;
}


void printTime(int time, int loopCount = 0) {
	disp.setTextSize(2);
	int x;
	switch (loopCount) {
	case 1000:
		x = 42;
		break;
	case 100:
		x = 53;
		break;
	case 10:
		x = 65;
		break;
	case 1:
		x = 77;
		break;
	default:
		x = 52;
		break;
	}
	disp.setCursor(32, 35);
	disp.print(time);
	disp.fillRect(x, 32, 140 - x, 20, BLACK);
	disp.display();
	return;
}
void dispEnterTime(String top, String bottom) {
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(1);
	disp.setCursor(0, 0);
	disp.print(top);
	disp.fillRect(0, 32, 128, 48, BLACK);
	disp.setCursor(0, 52);
	disp.print(bottom);
	disp.display();
	return;
}
void dispEnterPowerState() {
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(1);
	disp.setCursor(0, 0);
	disp.print(F("Should it be forced\nON or OFF?\n1 means ON\n0 means OFF"));
	disp.display();
	return;
}
void dispConfirmClearFlag() {
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(1);
	disp.print(F("Something is already set, would you like  to clear it?\nPress A to clear\nPress B to cancel "));
	disp.display();
	return;
}
void dispError(String error) {
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(2);
	disp.print(error);
	disp.setCursor(0, 17);
	disp.setTextSize(1);
	disp.setCursor(32, 38);
	disp.print(F("returning..."));
	disp.display();
	delayWithoutDelay(1200);
	return;
}

void dispOverrideMenu() {
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(1);
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
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(1);
	disp.setCursor(80, 8);
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
	disp.clearDisplay();
	disp.display();
	disp.setTextSize(2);
	disp.setCursor(10, 0);
	disp.print(F("Main Menu"));
	disp.setTextSize(1);
	disp.setCursor(0, 16);
	disp.print("A: Adjust Schedule");
	disp.setCursor(0, 27);
	disp.print(F("B: Temporary Override"));
	disp.setCursor(0, 38);
	disp.print(F("C: Manual Override\n   Menu"));
	disp.setCursor(0, 56);
	disp.print(F("D: More options"));
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
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(1);
	disp.print(F("Current Override"));
	disp.setTextSize(2);
	disp.setCursor(0, 16);
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
	disp.clearDisplay();
	disp.display();
	disp.setCursor(0, 0);
	disp.setTextSize(1);
	disp.print(F("Selected outlet: "));
	disp.setTextSize(2);
	disp.print(relayNum + 1);
	disp.setTextSize(1);
	disp.setCursor(0, 21);
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