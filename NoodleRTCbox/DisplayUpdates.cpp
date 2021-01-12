#include "DisplayUpdates.h"

//From the main menu, press C to get here
void DisplayClass::overrideSubMenuDisplay() {
	OLED.clearDisplay();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Manual control"));
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);
	OLED.println(F("1: enable/disable       manual control"));
	OLED.setCursor(0, 32);
	OLED.println(F("2: manual ON/OFF"));
	OLED.setCursor(0, 48);
	OLED.println(F("Press # to go back"));
	OLED.display();
}

void DisplayClass::currentTime(int hour, int min) {
	OLED.setTextSize(2);
	OLED.setCursor(0, 0);
	OLED.print(hour);
	OLED.print(":");
	if (min < 10)
		OLED.print(0);
	OLED.print(min);
	OLED.display();
}

void DisplayClass::schedulesSubMenu() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(80, 0);
	OLED.print(F("# - back"));
	OLED.setCursor(10, 16);
	OLED.println(F("1: Set Schedules"));
	OLED.setCursor(10, 25);
	OLED.println(F("2: Show Set Schedule"));
	OLED.setCursor(10, 34);
	OLED.println(F("3: Enable/Disable       schedules"));
	OLED.setCursor(10, 52);
	OLED.println(F("4: TEST 4"));
	OLED.display();
}

void DisplayClass::enableDisableScheduleScreen() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Allow schedule to be set?  * -> go back"));
	OLED.display();
}

void DisplayClass::clearCurrentTime() {
	OLED.fillRect(0, 0, 79, 16, BLACK);
	OLED.display();
}

void DisplayClass::mainMenu(void) {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(10, 0);
	OLED.println(F("Main Menu"));
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);
	OLED.println("A: Adjust Schedule");
	OLED.println(F("B: Temporary Override"));
	OLED.println(F("C: Manual Override      Menu"));
	OLED.println(F("D: More options"));
	OLED.display();
}

void DisplayClass::eightRelayNumbers() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);		//1
	OLED.println(F("1: "));
	OLED.setCursor(0, 28);		//2
	OLED.println(F("2: "));
	OLED.setCursor(0, 40);		//3
	OLED.println(F("3: "));
	OLED.setCursor(0, 52);		//4
	OLED.println(F("4: "));
	OLED.setCursor(64, 16);		//5
	OLED.println(F("5: "));
	OLED.setCursor(64, 28);		//6
	OLED.println(F("6: "));
	OLED.setCursor(64, 40);		//7
	OLED.println(F("7: "));
	OLED.setCursor(64, 52);		//8
	OLED.println(F("8: "));
	OLED.display();
}

void DisplayClass::clearRelayUpdate() {
	int x = 22;
	int y = 16;
	for (int rect = 0; rect <= 3; rect++) {
		OLED.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	}
	x = 86;
	y = 16;
	for (int rect = 0; rect <= 3; rect++) {
		OLED.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	}
	OLED.display();
}

void DisplayClass::manualOnOff() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Number turns on/off"));
	OLED.println(F("press * to go back"));
	OLED.display();
}

void DisplayClass::enableDisableRelayScreen() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("ON = manual control"));
	OLED.println(F("press * to go back"));
	OLED.display();
}

//TEMPORARY OVERRIDE FUNCTIONS
void DisplayClass::temporaryOverrideDisplay() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Select an outlet"));
	OLED.println(F("* - go back"));
	OLED.display();
}

void DisplayClass::confirmClearTempOverride() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.print(F("There is already an override set"));
	OLED.setCursor(0, 24);
	OLED.println(F("Would you like to    clear it?"));
	OLED.println(F("Press A to clear"));
	OLED.print(F("Press B to cancel"));
	OLED.display();
}

void DisplayClass::invalidTime(String word) {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(2);
	OLED.print(F("Invalid "));
	OLED.setCursor(0, 17);
	OLED.println(word);
	OLED.setTextSize(1);
	OLED.setCursor(32, 38);
	OLED.print(F("returning..."));
	OLED.display();
}

void DisplayClass::enterStartingHour() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("enter starting hours"));
	OLED.println(F("use 24 hour format"));
	OLED.println(F("Press A-D to go back"));
	OLED.print(F("# when done"));
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.display();
}

void DisplayClass::enterStartingMinute() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("enter starting       minutes   * - back"));
	OLED.print(F("# when done"));
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.display();
}

void DisplayClass::enterDuration() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Enter duration"));
	OLED.println(F("use minutes only"));
	OLED.println(F("Press 'D' when done"));
	OLED.print(F("press * to cancel"));
	OLED.fillRect(0, 40, 128, 48, BLACK);
	OLED.display();
}

void DisplayClass::enterPowerState() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Should it be forced  ON or OFF?"));
	OLED.println(F("1 means ON"));
	OLED.print(F("0 means OFF"));
	OLED.display();
}

void DisplayClass::printTime(int time, int loopCount) {
	OLED.setCursor(0, 0);
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(48, 42);
	OLED.print(time);
	if (loopCount == 1)
		OLED.fillRect(58, 24, 10, 16, BLACK);
	OLED.display();
}

void DisplayClass::printTime(int duration) {
	OLED.setCursor(0, 0);
	OLED.fillRect(0, 24, 128, 48, BLACK);
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(50, 26);
	OLED.print(duration);
	OLED.display();
}

void DisplayClass::tempOverrideSubMenu() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("pick one"));
	OLED.print(F("# - go back"));
	OLED.setCursor(0, 18);
	OLED.println(F("1: Set/Clear            temp. override"));
	OLED.print(F("2: Show single status"));
	OLED.display();
}

void DisplayClass::tempOverrideStatus() {
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Choose one to show     status   * - back"));
	OLED.display();
}

void DisplayClass::displaySingleObjectTempOverrideStatus(int hour, int minute, byte state) {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.print(F("Current Override:"));
	OLED.setTextSize(2);
	OLED.setCursor(0, 16);
	OLED.print(hour);
	OLED.print(F(":"));
	OLED.println(minute);
	OLED.setTextSize(1);
	OLED.print(F("Forced state: "));
	OLED.print((state == 1) ? "ON" : "OFF");
	OLED.setCursor(0, 48);
	OLED.print(F("press A-D to go back"));
	OLED.display();
}
//END TEMPORARY OVERRIDE MENU