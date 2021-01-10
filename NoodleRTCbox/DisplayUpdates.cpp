#include "DisplayUpdates.h"

//From the main menu, press C to get here
void DisplayClass::overrideSubMenuDisplay() {
	OLED.clearDisplay();
	OLED.setCursor(0, 0);
	OLED.setTextSize(1);
	OLED.println(F("Manual control"));
	OLED.setTextSize(1);
	OLED.setCursor(0, 16);
	OLED.println(F("A: enable/disable       manual control"));
	OLED.setCursor(0, 32);
	OLED.println(F("B: manual ON/OFF"));
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
	OLED.println(F("A: TEST 1"));
	OLED.setCursor(10, 28);
	OLED.println(F("B: TEST 2"));
	OLED.setCursor(10, 40);
	OLED.println(F("C: TEST 3"));
	OLED.setCursor(10, 52);
	OLED.println(F("D: TEST 4"));
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
	OLED.setTextSize(2);
	OLED.setCursor(0, 0);
	OLED.println(F("Wait a sec"));
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
	OLED.fillRect(0, 0, 128, 16, BLACK);
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("Number turns on/off"));
	OLED.println(F("press * to go back"));
	OLED.display();
}

void DisplayClass::overrideScreenUpdate(int* states) {
	for (int spot = 0; spot <= 3; spot++) {
		OLED.setCursor(22, 16 + (spot * 12));
		OLED.println(states[spot] ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		OLED.setCursor(86, 16 + (spot * 12));
		OLED.println(states[spot + 4] ? "ON" : "OFF");
	}
	OLED.display();
}

void DisplayClass::enableDisableRelayScreen() {
	OLED.fillRect(0, 0, 128, 16, BLACK);
	OLED.setTextSize(1);
	OLED.setCursor(0, 0);
	OLED.println(F("ON = manual control"));
	OLED.println(F("press * to go back"));
	OLED.display();
}