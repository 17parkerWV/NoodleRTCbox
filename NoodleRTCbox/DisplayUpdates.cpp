#include "DisplayUpdates.h"

//From the main menu, press C to get here
void DisplayClass::overrideSubMenuDisplay() {
	OLED.clearDisplay();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Force an outlet to\nstay ON or OFF"));
	OLED.print(F("Press # to go back"));
	OLED.setCursor(0, 32);
	OLED.print(F("1: Enable/disable\n   manual control"));
	OLED.setCursor(0, 50);
	OLED.print(F("2: Set outlet ON/OFF"));
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
	OLED.setCursor(80, 8);
	OLED.print(F("# - back"));
	OLED.setCursor(0, 16);
	OLED.print(F("1: Set Schedules"));
	OLED.setCursor(0, 25);
	OLED.print(F("2: Show Set Schedule"));
	OLED.setCursor(0, 34);
	OLED.print(F("3: Enable/Disable\n   schedules"));
	OLED.setCursor(0, 52);
	OLED.print(F("4: Complete disable"));
	OLED.display();
}

void DisplayClass::enableDisableScheduleScreen() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("Allow schedule to be set?  * - back"));
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
	OLED.print(F("Main Menu"));
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);
	OLED.print("A: Adjust Schedule");
	OLED.setCursor(0, 27);
	OLED.print(F("B: Temporary Override"));
	OLED.setCursor(0, 38);
	OLED.print(F("C: Manual Override\n   Menu"));
	OLED.setCursor(0, 56);
	OLED.print(F("D: More options"));
	OLED.display();
}

void DisplayClass::eightRelayNumbers() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);		//1
	OLED.print(F("1: "));
	OLED.setCursor(0, 28);		//2
	OLED.print(F("2: "));
	OLED.setCursor(0, 40);		//3
	OLED.print(F("3: "));
	OLED.setCursor(0, 52);		//4
	OLED.print(F("4: "));
	OLED.setCursor(64, 16);		//5
	OLED.print(F("5: "));
	OLED.setCursor(64, 28);		//6
	OLED.print(F("6: "));
	OLED.setCursor(64, 40);		//7
	OLED.print(F("7: "));
	OLED.setCursor(64, 52);		//8
	OLED.print(F("8: "));
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
	OLED.print(F("ON means ON OFF meansOFF, easy   * - back"));
	OLED.display();
}

void DisplayClass::enableDisableRelayScreen() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("ON enables manual\ncontrol     * - back"));
	OLED.display();
}

void DisplayClass::completeOffScreen() {
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.print(F("Select an outlet to  be reset    * - back"));
	OLED.display();
}

void DisplayClass::confirmationScreen() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(2);
	OLED.print(F("Are you   sure?"));
	OLED.setTextSize(1);
	OLED.setCursor(0, 42);
	OLED.print(F("A: Yes        B: No"));
	OLED.display();
}

//TEMPORARY OVERRIDE FUNCTIONS
void DisplayClass::temporaryOverrideDisplay() {
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("Select an outlet\n* - back"));
	OLED.display();
}

void DisplayClass::confirmClearTempOverride() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.print(F("There is already an\noverride set \nWould you like to\nclear it?\n\nPress A to clear\nPress B to cancel "));
	OLED.display();
}

void DisplayClass::displayError(String word) {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(2);
	OLED.print(word);
	OLED.setCursor(0, 17);
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
	OLED.print(F("Enter starting hour  using 24 hour format Press STAR to cancel Press D when done"));
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.setCursor(0, 52);
	OLED.print(F("24 HOUR FORMAT ONLY"));
	OLED.display();
}

void DisplayClass::enterStartingMinute() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("Enter starting\nminutes   * - back\nPress D when done"));
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.display();
}

void DisplayClass::enterDuration() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("Enter duration of\noverride in minutes\npress * to cancel\nPress 'D' when done"));
	OLED.fillRect(0, 40, 128, 48, BLACK);
	OLED.display();
}

void DisplayClass::displayCleared() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(0, 0);
	OLED.print(F("Cleared"));
	OLED.display();
}

void DisplayClass::enterPowerState() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.print(F("Should it be forced\nON or OFF?\n1 means ON\n0 means OFF"));
	OLED.display();
}

void DisplayClass::printTime(int time, int loopCount) {
	OLED.setCursor(0, 0);
	OLED.fillRect(0, 32, 128, 20, BLACK);
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(48, 35);
	OLED.print(time);
	if (loopCount == 1)
		OLED.fillRect(58, 32, 16, 20, BLACK);
	OLED.display();
}

void DisplayClass::printTime(int duration) {
	OLED.setCursor(0, 0);
	OLED.fillRect(0, 32, 128, 48, BLACK);
	OLED.display();
	OLED.setTextSize(2);
	OLED.setCursor(32, 36);
	OLED.print(duration);
	OLED.display();
}

void DisplayClass::tempOverrideSubMenu() {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Temporarily override any set schedule!"));
	OLED.print(F("Press # to go back"));
	OLED.setCursor(0, 32);
	OLED.print(F("1: Set/Clear\n   temp. override"));
	OLED.setCursor(0, 50);
	OLED.print(F("2: Show single status"));
	OLED.display();
}

void DisplayClass::tempOverrideStatus() {
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Choose one to show\nstatus   * - back"));
	OLED.display();
}

void DisplayClass::displaySingleObjectTempOverrideStatus(int hour, int minute, int duration, byte state) {
	OLED.clearDisplay();
	OLED.display();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.print(F("Current Override:"));
	OLED.setTextSize(2);
	OLED.setCursor(0, 16);
	if (hour > 12)
		OLED.print(hour - 12);
	else
		OLED.print(hour);
	OLED.print(F(":"));
	if (minute < 10)
		OLED.print(0);
	OLED.print(minute);
	OLED.setTextSize(1);
	OLED.println();
	if (hour < 12 && hour >= 0) {
		OLED.println(F("          AM"));
	}
	if (hour >= 12) {
		OLED.println(F("          PM"));
	}
	OLED.print(F("Forced state: "));
	OLED.println((state == 0) ? "ON" : "OFF");
	OLED.print(F("For "));
	OLED.print(duration);
	OLED.print(F(" minutes"));
	OLED.setCursor(0, 56);
	OLED.print(F("press A-D to go back"));
	OLED.display();
}
//END TEMPORARY OVERRIDE MENU