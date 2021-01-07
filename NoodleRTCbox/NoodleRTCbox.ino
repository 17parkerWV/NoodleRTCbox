#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <splash.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include "RelayObjects.h"
#include <Wire.h>
#include <RTClib.h>

/*
PIN LAYOUT FOR THE 4x4 NUMBER PAD:
pins 1-4: ROW
pins 5-8: COLUMN (5 is column 1, 6 is column 2, ...)
ex: pin 2 + pin 7 = row 2 column 3 -> button [6]
Set ROW as INPUT_PULLUP and COLUMN as OUTPUT -> LOW, then switch when ROW is detected
PC0 (pin 37) - Pin 1 on number pad
PC1 (pin 36) - Pin 2 on number pad
PC2 (pin 35) - Pin 3 on number pad
PC3 (pin 34) - Pin 4 on number pad
PC4 (pin 33) - Pin 5 on number pad
PC5 (pin 32) - Pin 6 on number pad
PC6 (pin 31) - Pin 7 on number pad
PC7 (pin 30) - Pin 8 on number pad
*/
#define COL_LOW_ROW_PULLUP ROW_BITS
#define ROW_LOW_COL_PULLUP COL_BITS
#define ROW_IN_COL_OUT COL_BITS
#define COL_IN_ROW_OUT ROW_BITS
byte buttonPress = 0b00000000;

int relayArrayPins[8] = { 42,43,44,45,46,47,48,49 };	//pins of the relays

RTC_DS3231 clockObj;
DateTime clockSecondObj;
Adafruit_SSD1306 OLEDdisplay(128, 64);
Relay powerArray[8];
Relay dummyObj;


void mainMenu(void);			//menu that displays all relays
void moreOptionsMenu();			//More options menu because they didn't fit on one screen
void enableDisableRelay();		//allows manual control of relay
void manualOnOff();				//menu to enable/disable a plug
void adjustScheduleMenu(int);		//menu when you select a relay
void overrideScreenUpdate(int*);		//I will need a function that updates displayed info, separate from the menu items that will not change, need to change the name
void printNumber();		//This will print the binary result of pressing a button on the keypad
void updateClockObj();
void errorQuit(int code);	//Print error code
void printTime(void);		//print current date/time
byte buttonPoll(void);		//this will get byte data from the number pad to reduce duplicated code
void eightRelayNumbers();	//This will print the numbers of the relays on the screen
void clearRelayUpdate();	//this prints black rectangles over data so the whole screen isn't cleared
void overrideSubMenuDisplay();		//this prints the override sub menu stuff
void overrideSubMenu();		//This brings you to the menu where you can override or not

void setup() {
	Serial.begin(115200);
	for (int i = 0; i <= 7; i++) {
		pinMode(relayArrayPins[i], OUTPUT);
		digitalWrite(relayArrayPins[i], HIGH);
	}
	dummyObj.initializePins(powerArray, relayArrayPins);
	if (!OLEDdisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
		errorQuit(3);
	}
	if (clockObj.lostPower()) {
		errorQuit(2);
	}
	if (!clockObj.begin()) {
		errorQuit(1);
	}
	OLEDdisplay.display();
	OLEDdisplay.clearDisplay();
	OLEDdisplay.display();
	OLEDdisplay.setCursor(0, 0);
	OLEDdisplay.setTextColor(WHITE);
	//Set up the keypad inputs/outputs
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;

	//Maybe function to set the clock?
	//Function to notify that all schedules were wiped out
	mainMenu();
}


void loop() {
	byte buttonData = buttonPoll();
	if ((buttonData & COL_BITS) == (COL_4)) {
		if ((buttonData & ROW_BITS) == (ROW_1)) {		//"A" on the num pad
			//This is the adjust schedule menu
			mainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_2)) {		//"B" on the num pad
		   //This is the temporary override menu
			mainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_3)) {	//"C" on the num pad
			overrideSubMenu();
			mainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_4)) {	//"D" on the num pad
		   //This is the more options menu
			mainMenu();
		}
		delay(100);
	}
	delay(175);
}


void mainMenu(void) {
	OLEDdisplay.clearDisplay();
	OLEDdisplay.display();
	OLEDdisplay.setTextSize(2);
	OLEDdisplay.setCursor(10, 0);
	OLEDdisplay.println(F("Main Menu"));
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 16);
	OLEDdisplay.println("A: Adjust Schedule");
	OLEDdisplay.println(F("B: Temporary Override"));
	OLEDdisplay.println(F("C: Manual Override      Menu"));
	OLEDdisplay.println(F("D: More options"));
	OLEDdisplay.display();
}

void moreOptionsMenu() {

}

void adjustScheduleMenu(int relayNumber) {

}

void manualOnOff() {						//DOES NOT CHECK TIME IN THIS LOOP///
	eightRelayNumbers();
	overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
	OLEDdisplay.fillRect(0, 0, 128, 16, BLACK);
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 0);
	OLEDdisplay.println(F("Number turns on/off"));
	OLEDdisplay.println(F("press * to go back"));
	OLEDdisplay.display();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].getOverrideStatus() == true) {
					powerArray[0].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].getOverrideStatus() == true) {
					powerArray[3].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].getOverrideStatus() == true) {
					powerArray[6].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_4) {
				break;
			}
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 2 on the numpad
				if (powerArray[1].getOverrideStatus() == true) {
					powerArray[1].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].getOverrideStatus() == true) {
					powerArray[4].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[8].getOverrideStatus() == true) {
					powerArray[7].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].getOverrideStatus() == true) {
					powerArray[2].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].getOverrideStatus() == true) {
					powerArray[5].flipPowerState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getPowerStates(powerArray));
				}
			}
		}
	}
	overrideSubMenuDisplay();
}


void eightRelayNumbers() {
	OLEDdisplay.clearDisplay();
	OLEDdisplay.display();
	OLEDdisplay.setTextSize(2);
	OLEDdisplay.setCursor(0, 0);
	OLEDdisplay.println(F("Wait a sec"));
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 16);		//1
	OLEDdisplay.println(F("1: "));
	OLEDdisplay.setCursor(0, 28);		//2
	OLEDdisplay.println(F("2: "));
	OLEDdisplay.setCursor(0, 40);		//3
	OLEDdisplay.println(F("3: "));
	OLEDdisplay.setCursor(0, 52);		//4
	OLEDdisplay.println(F("4: "));
	OLEDdisplay.setCursor(64, 16);		//5
	OLEDdisplay.println(F("5: "));
	OLEDdisplay.setCursor(64, 28);		//6
	OLEDdisplay.println(F("6: "));
	OLEDdisplay.setCursor(64, 40);		//7
	OLEDdisplay.println(F("7: "));
	OLEDdisplay.setCursor(64, 52);		//8
	OLEDdisplay.println(F("8: "));
	OLEDdisplay.display();
}

void clearRelayUpdate() {
	int x = 22;
	int y = 16;
	for (int rect = 0; rect <= 3; rect++) {
		OLEDdisplay.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	}
	x = 86;
	y = 16;
	for (int rect = 0; rect <= 3; rect++) {
		OLEDdisplay.fillRect(x, y + (rect * 12), 24, 16, BLACK);
	}
	OLEDdisplay.display();
}

void overrideScreenUpdate(int* states) {
	for (int spot = 0; spot <= 3; spot++) {
		OLEDdisplay.setCursor(22, 16 + (spot * 12));
		OLEDdisplay.println(states[spot] ? "ON" : "OFF");
	}
	for (int spot = 0; spot <= 3; spot++) {
		OLEDdisplay.setCursor(86, 16 + (spot * 12));
		OLEDdisplay.println(states[spot + 4] ? "ON" : "OFF");
	}
	OLEDdisplay.display();
}

//From the main menu, press C to get here
void overrideSubMenuDisplay() {
	OLEDdisplay.clearDisplay();
	OLEDdisplay.setCursor(0, 0);
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.println(F("Manual control"));
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 16);
	OLEDdisplay.println(F("A: enable/disable       manual control"));
	OLEDdisplay.setCursor(0, 32);
	OLEDdisplay.println(F("B: manual ON/OFF"));
	OLEDdisplay.setCursor(0, 48);
	OLEDdisplay.println(F("Press # to go back"));
	OLEDdisplay.display();
}
void overrideSubMenu() {
	overrideSubMenuDisplay();
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_3) {
			if ((buttonPress & ROW_BITS) == ROW_4) {	//# sign to go back
				break;
			}
		}
		if ((buttonPress & COL_BITS) == COL_4) {
			if ((buttonPress & ROW_BITS) == ROW_1) {		//Button A goes to enable disable
				enableDisableRelay();
			}
			if ((buttonPress & ROW_BITS) == ROW_2) {		//Button B goes to manual ON/OFF menu
				manualOnOff();
			}
		}
	}
}

void enableDisableRelay() {
	eightRelayNumbers();
	overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
	OLEDdisplay.fillRect(0, 0, 128, 16, BLACK);
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 0);
	OLEDdisplay.println(F("ON = manual control"));
	OLEDdisplay.println(F("press * to go back"));
	OLEDdisplay.display();
	while (1) {
		byte buttonByte = buttonPoll();
		if ((buttonByte & COL_BITS) == COL_1) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 1 on the numpad
				if (powerArray[0].getPoweredStatus() == false) {
					powerArray[0].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 4 on the numpad
				if (powerArray[3].getPoweredStatus() == false) {
					powerArray[3].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 7 on the numpad
				if (powerArray[6].getPoweredStatus() == false) {
					powerArray[6].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_4) {		//Exit with the * sign
				break;
			}
		}
		if ((buttonByte & COL_BITS) == COL_2) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 2 on the numpad
				if (powerArray[1].getPoweredStatus() == false) {
					powerArray[1].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 5 on the numpad
				if (powerArray[4].getPoweredStatus() == false) {
					powerArray[4].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_3) {
				//Number 8 on the numpad
				if (powerArray[7].getPoweredStatus() == false) {
					powerArray[7].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
		}
		if ((buttonByte & COL_BITS) == COL_3) {
			if ((buttonByte & ROW_BITS) == ROW_1) {
				//Number 3 on the numpad
				if (powerArray[2].getPoweredStatus() == false) {
					powerArray[2].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
			if ((buttonByte & ROW_BITS) == ROW_2) {
				//Number 6 on the numpad
				if (powerArray[5].getPoweredStatus() == false) {
					powerArray[5].flipOverrideState();
					clearRelayUpdate();
					overrideScreenUpdate(dummyObj.getEnableStates(powerArray));
				}
			}
		}
	}
	overrideSubMenuDisplay();
}

void updateClockObj() {
	clockSecondObj = clockObj.now();
}

byte buttonPoll() {
	byte buttonPress = 0b00000000;
	byte rowBits = (PINC & ROW_BITS);
	DDRC = COL_IN_ROW_OUT;
	PORTC = ROW_LOW_COL_PULLUP;
	delay(50);
	byte colBits = (PINC & COL_BITS);
	buttonPress = rowBits | colBits;
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;
	delay(50);
	return buttonPress;
}

void errorQuit(int code) {
	switch (code) {
	case 1:
		Serial.println(F("Clock failed to begin, contact support"));
		break;
	case 2:
		Serial.println(F("RTC lost power, reset time and schedules"));
		break;
	case 3:
		Serial.println(F("Display failed to start"));
		break;
	default:
		Serial.println(F("Unspecified error"));
		break;
	}
	dummyObj.allOff(powerArray);
	for (;;) {};
}

void printTime() {
	updateClockObj;
	Serial.println(clockSecondObj.year());
	Serial.println(clockSecondObj.month());
	Serial.println(clockSecondObj.day());
	Serial.println(clockSecondObj.hour());
	Serial.println(clockSecondObj.minute());
	Serial.println(clockSecondObj.second());
}

void printNumber() {
	Serial.println(buttonPress, BIN);
	buttonPress = 0b00000000;
	delay(200);
}