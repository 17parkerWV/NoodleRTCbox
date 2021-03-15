//I HAD TO MODIFY THE RTC LIBRARY FOR THIS PROGRAM
//BE CAREFUL USING IT SOMEWHERE ELSE
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"

//BUTTONS ON THE NUMBERPAD IN BINARY
#define NUM_PAD_1 0b11101110
#define NUM_PAD_2 0b11011110
#define NUM_PAD_3 0b10111110
#define NUM_PAD_4 0b11101101
#define NUM_PAD_5 0b11011101
#define NUM_PAD_6 0b10111101
#define NUM_PAD_7 0b11101011
#define NUM_PAD_8 0b11011011
#define NUM_PAD_9 0b10111011
#define NUM_PAD_0 0b11010111
#define NUM_PAD_A 0b01111110
#define NUM_PAD_B 0b01111101
#define NUM_PAD_C 0b01111011
#define NUM_PAD_D 0b01110111
#define NUM_PAD_STAR 0b11100111
#define NUM_PAD_SHARP 0b10110111
//BUTTON PRESSES IN BINARY
#define ROW_BITS 0b00001111
#define COL_BITS 0b11110000
#define COL_1 0b11100000
#define COL_2 0b11010000
#define COL_3 0b10110000
#define COL_4 0b01110000
#define ROW_1 0b00001110
#define ROW_2 0b00001101
#define ROW_3 0b00001011
#define ROW_4 0b00000111
#define COL_LOW_ROW_PULLUP ROW_BITS
#define ROW_LOW_COL_PULLUP COL_BITS
#define ROW_IN_COL_OUT COL_BITS
#define COL_IN_ROW_OUT ROW_BITS
//BITSHIFT AMOUNTS for port manipulation - used with number pad
#define DIGITAL_PIN_42 (1<<7)	//Pin 42, PL7
#define DIGITAL_PIN_43 (1<<0)	//Pin 43, PD0
#define DIGITAL_PIN_44 (1<<5)	//Pin 44, PL5
#define DIGITAL_PIN_45 (1<<4)	//Pin 45, PL4
#define DIGITAL_PIN_46 (1<<3)	//Pin 46, PL3
#define DIGITAL_PIN_47 (1<<2)	//Pin 47, PL2
#define DIGITAL_PIN_48 (1<<1)	//Pin 48, PL1
#define DIGITAL_PIN_49 (1<<0)	//Pin 49, PL0

//Arrays for setting and checking the set date & time
const uint8_t daysInMonth[] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 };
const String daysOfWeek[] = { "Sun","Mon","Tue","Wed","Thurs","Fri","Sat" };
//Arrays used when printing menus
const String abcd[] = { "A: ","B: ","C: ","D: " };
const String num[] = { "1: ","2: ","3: ","4: " };
String messArr[4];

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

class outlets {
public:
	int relayPin;
	//Whether or not a schedule or an override is set
	volatile bool overrideSetFlag = false;
	volatile bool schedSetFlag = false;
	//On and off times for schedules and overrides, and duration of override
	int hourOn = 0;
	int minuteOn = 0;
	int hourOff = 0;
	int minuteOff = 0;
	int overrideHour = 0;
	int overrideMinute = 0;
	int overrideOffHour = 0;
	int overrideOffMinute = 0;
	int overrideDuration = 0;
	//TRUE if relay is within range of override, FALSE otherwise
	volatile bool overrideStatus = false;
	//1 if override forces relay ON, 0 if override forces relay OFF
	volatile bool overrideState = 1;
	//If FALSE, schedule is not set to be on overnight. If TRUE, schedule was set to be on overnight
	volatile bool schedState = false;
	//TRUE if relay is ON manually, FALSE otherwise
	volatile bool manualPoweredStatus = false;

	//GET functions
	bool getManualPoweredStatus() { return manualPoweredStatus; }
	bool getSchedSetFlag() { return schedSetFlag; }
	bool getSchedState() { return schedState; }
	bool getOverrideSetFlag() { return overrideSetFlag; }
	bool getOverrideStatus() { return overrideStatus; }

	//SET functions
	void setSchedFlag() { schedSetFlag = true; }
	void setOverrideStatus() { overrideStatus = true; }
	void setOverrideSetFlag() { overrideSetFlag = true; }
	void setManualPoweredStatus() { manualPoweredStatus = true; }
	void clearManualPoweredStatus() { manualPoweredStatus = false, digitalWrite(relayPin, HIGH); }
	void setSchedOn(int hr, int min) {
		hourOn = hr;
		minuteOn = min;
	}
	void setSchedOff(int hr, int min) {
		hourOff = hr;
		minuteOff = min;
	}

	//CLEAR functions
	void off();
	void clearSchedSetFlag() { schedSetFlag = false, digitalWrite(relayPin, HIGH); }
	void clearSchedState() { schedState = false; }
	void clearOverrideStatus() { overrideStatus = false; }
	void clearOverrideSetFlag() {
		overrideSetFlag = false;
		overrideStatus = false;
		digitalWrite(relayPin, HIGH);
	}

	//FLIP functions
	void flipManualPoweredStatus() {
		manualPoweredStatus = !manualPoweredStatus;				//Flip the status of the flag
		digitalWrite(relayPin, !manualPoweredStatus);			//status of flag is opposite of actual powered status
	}
}; //END class

//Interrupt setup
#define BUILT_IN_LED (1<<7)		//digital pin 13, PB7
#define DIGITAL_PIN_2 (1<<4)	//(OC3B/INT4) Port E bit 4
volatile byte counter = 0;

//Object setup
outlets relay[8];
const int relayArrayPins[8] = { 42,43,44,45,46,47,48,49 };
Adafruit_SSD1306 disp(128, 64);
RTC_DS3231 clockObj;
DateTime clockSecondObj;

//*****FUNCTION PROTOTYPES*****//
//General functions
void initializeObjs();
void POWERLOSS();
void allOff();
void delayWithoutDelay(unsigned int time);
byte buttonPoll();
void waitForAnyLetterPress();

//START OLED Functions
//Shared functions
void printMenu(int argc, const String label[], String m1, String m2 = "", String m3 = "", String m4 = "", bool header = false, String headerm = "", bool footer = false, String footerm = "", int fontSize = 2);
void dispEightRelays(int mode, String message = "", bool header = false);
void printHeader(String message, int fontSize = 1, bool clear = false);
void prepDisp(int fontSize = 1, int x = 0, int y = 0, bool clear = true);
void printTime(int time, int loopCount = 0);
void dispEnterTime(String top, String bottom = "");
void dispError(String error);
void clearRelayUpdate();
//Main menu
void dispMainMenu() {
	printMenu(4, abcd, F("Adjust Schedule"), F("Temporary Override"), F("Manual Override\n   Menu"), F("Set date and time"), true, F("Main Menu"));
}
//Schedule display functions
void dispSchedulesMenu() {
	printMenu(3, num, F("Set/Clear Schedule"), F("View Schedule"), F("Complete disable"), F(""), false, F(""), true, F("Press # to go back"));
}
void dispSingleSchedStatus(int relayNum);
void dispCurrentTime();
//Override display functions
void dispOverrideMenu() {
	printMenu(2, num, F("Set/Clear\n   temp. override"), F("Show single status"), F(""), F(""), true, F("Overrides"), true, F("Press # to go back"));
}
void dispSingleOverrideStatus(int relayNum);
void dispEnterDuration();
//Clock display functions
void dispClockMenu() {
	printMenu(2, num, F("Set Date/Time"), F("Show current time"), F(""), F(""), true, F("Clock Menu"), true, F("Press # to go back"), 1);
}
void dispTime();
//END OLED  functions

//***Relay Functions
void promptSchedTime(outlets& obj);
void promptOverrideTime(outlets& obj);
void confirmClear(outlets& obj, void (outlets::* clearFunc)());
int chooseRelay(int func);

//***Clock Object functions
void timeControl();
void updateClock();
void setTime();
bool validateDay(uint8_t month, uint8_t day);


//***Sub Menu functions
int inputTime();
bool confirmationMenu();
int inputDuration();
int verifyHour(int& hour);
int verifyMinute(int& min);
int verifyDuration(int& dur);
byte inputPowerState();
//A -> Schedules
void schedulesMenu();				//A
void setScheduleSubMenu();			//A -> 1
void scheduleSetStatusSubMenu();	//A -> 2
void scheduleSetStatusLoop();
void completeOffSubMenu();			//A -> 3
void completeOffLoop();
void completeOffSubMenu();			//A -> 4
//B -> Overrides
void overrideMenu();				//B
void overrideSubMenu();				//B -> 1
void overrideStatusSubMenu();		//B -> 2
void overrideStatusLoop();
//C -> Manual control
void manualOnOffMenu();				//C
void manualOnOffLoop();
//D -> Clock menu
void clockMenu();
void setTimeMenu();

void outlets::off() {
	clearSchedSetFlag();
	clearSchedState();
	clearOverrideSetFlag();
	clearOverrideStatus();
	digitalWrite(relayPin, HIGH);
	return;
}
void allOff() {
	for (int i = 0; i <= 7; ++i)
		relay[i].off();
	return;
}
void POWERLOSS() {
	printHeader(F("POWER HAS BEEN LOST\n\nSchedules have been\nreset\n\n\nPress A-D to continue"), 1, true);
	waitForAnyLetterPress();
	return;
}

ISR(INT4_vect) {
	sei();
	EIMSK = 0b00000000;
	EIFR |= DIGITAL_PIN_2;
	if (counter >= 2) {
		PORTB ^= BUILT_IN_LED;
		updateClock();
		timeControl();
		counter = 0;
	}
	else
		++counter;
	EIMSK |= DIGITAL_PIN_2;
}

void setup() {
	initializeObjs();
	POWERLOSS();
	delayWithoutDelay(500);
	//FOR THE INTERRUPTS
	DDRB |= BUILT_IN_LED;		//set PB7 (digital pin 13) as output
	PORTB = 0b00000000;			//write digital pin 13 LOW and the other pins are INPUTS without the PULLUP
	DDRE = 0b00000000;	//All bit in port E are inputs
	EIMSK = 0b00000000;	//All interrupts are masked out (recommended in the datasheet)
	MCUCR |= (0 << 4);		//Set the pullup disable bit LOW
	PORTE |= DIGITAL_PIN_2;		//Bit 4 has pullup resistor enabled
	EICRB = 0b00000011;			//Rising edge interrupt enabled on INT4 (pin 2)
	//This is the interrupt pin, I think
	digitalWrite(2, INPUT_PULLUP);
	//Set up the keypad inputs/outputs
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;
	EIMSK |= DIGITAL_PIN_2;		//Bit 4 (INT4) interrupt mask bit to 1, enable the interrupt
	dispMainMenu();
}
//***MAIN MENU
void loop() {
	byte buttonData = buttonPoll();
	if ((buttonData & COL_BITS) == (COL_4)) {
		if (buttonData == NUM_PAD_A) {				//A: Schedule menu
			//This is the adjust schedule menu
			dispSchedulesMenu();
			schedulesMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_B) {				//B: Override menu
		   //This is the temporary override menu
			dispOverrideMenu();
			overrideMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_C) {				//C: Manual on/off
			manualOnOffMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_D) {			//D: More Options menu, incomplete
			dispClockMenu();
			clockMenu();
			dispMainMenu();
		}
		delayWithoutDelay(100);
	}
	delayWithoutDelay(80);
}
//***START MENUS
//Main Menu -> A
void schedulesMenu() {
	dispCurrentTime();
	int currentMin = clockSecondObj.minute();
	unsigned long currentMillis = millis();
	while (1) {
		currentMillis = millis();
		while ((millis() - currentMillis) <= 5000) {
			byte buttonPress = buttonPoll();
			if (buttonPress == NUM_PAD_1) 		//Set new schedule
				setScheduleSubMenu();
			if (buttonPress == NUM_PAD_2) 		//View schedules
				scheduleSetStatusSubMenu();
			if (buttonPress == NUM_PAD_3) 		//Disable everything
				completeOffSubMenu();
			if (buttonPress == NUM_PAD_SHARP)	//Quit back
				return;
		}
		updateClock();
		if (currentMin != clockSecondObj.minute()) {
			dispCurrentTime();
			currentMin = clockSecondObj.minute();
		}
	}
	return;
}
//Main Manu -> B
void overrideMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)		//Button 1 goes to set/clear
			overrideSubMenu();
		if (buttonPress == NUM_PAD_2) 		//Button 2 goes to show current status
			overrideStatusSubMenu();
		if (buttonPress == NUM_PAD_SHARP) 	//# sign to go back
			break;
	}
	return;
}
//MainMenu -> C - Displays the 8 relays and if they are powered manually
void manualOnOffMenu() {
	dispEightRelays(2, F("ON means ON OFF meansOFF, easy   * - back"), true);
	manualOnOffLoop();
	return;
}
//Main Menu -> D
void clockMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)		//Button 1 goes to set/clear
			setTimeMenu();
		if (buttonPress == NUM_PAD_2) { 		//Button 2 goes to show current status
			updateClock();
			dispTime();
			waitForAnyLetterPress();
			dispClockMenu();
		}
		if (buttonPress == NUM_PAD_SHARP) 	//# sign to go back
			break;
	}
	return;
}
//END MENUS
//***START SUB-MENUS
//Main Manu -> A -> 1
void setScheduleSubMenu() {
	dispEightRelays(4, F("Pick one to set \nschedule     * - back"), true);
	if (chooseRelay(2) == -1)
		dispError(F("failed to select relay"));
	dispSchedulesMenu();
	dispCurrentTime();
	return;
}
//Main Menu -> A -> 2
void scheduleSetStatusSubMenu() {
	dispEightRelays(4, F("Choose one to show\nstatus * -back"), true);
	scheduleSetStatusLoop();
	dispSchedulesMenu();
	dispCurrentTime();
	return;
}
//Main Menu -> A -> 3
void completeOffSubMenu() {
	dispEightRelays(9, F("Select an outlet to  be reset    * - back"), true);
	completeOffLoop();
	delayWithoutDelay(400);
	dispSchedulesMenu();
	dispCurrentTime();
	return;
}
//Main Menu -> B -> 1
void overrideSubMenu() {
	dispEightRelays(3, F("Select an outlet\n* - back"), true);
	if (chooseRelay(1) == -1)
		dispError(F("failed to select relay"));
	dispOverrideMenu();
	return;
}
//Main Menu -> B -> 2
void overrideStatusSubMenu() {
	dispEightRelays(3, F("Choose one to show\nstatus   * - back"), true);
	overrideStatusLoop();
	dispOverrideMenu();
	return;
}
//Main Menu -> D -> 1
void setTimeMenu() {
	printHeader(F("Do you want to set\nthe time?\n\nA: Yes        B : No"), 1, true);
	if (confirmationMenu())
		setTime();
	dispClockMenu();
	return;
}
//END SUB-MENUS