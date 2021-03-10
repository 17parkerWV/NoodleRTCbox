//excluding these 2 lines, this program is 1,678 lines long (total including spaces + comments)
//There are 56 functions (79 if you include all of the inline ones, too)
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
#include <RTClib.h>

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
//THIS IS FOR CHECKING BUTTON PRESSES
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
#define DIGITAL_PIN_42 (1<<7)	//Pin 42, PL7
#define DIGITAL_PIN_43 (1<<0)	//Pin 43, PD0
#define DIGITAL_PIN_44 (1<<5)	//Pin 44, PL5
#define DIGITAL_PIN_45 (1<<4)	//Pin 45, PL4
#define DIGITAL_PIN_46 (1<<3)	//Pin 46, PL3
#define DIGITAL_PIN_47 (1<<2)	//Pin 47, PL2
#define DIGITAL_PIN_48 (1<<1)	//Pin 48, PL1
#define DIGITAL_PIN_49 (1<<0)	//Pin 49, PL0
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
	//Whether or not a schedule is set
	volatile bool schedSetFlag = false;
	//On and off times for a schedule
	int hourOn = 0;
	int minuteOn = 0;
	int hourOff = 0;
	int minuteOff = 0;
	//Whether or not the user has manual control of the relay
	volatile bool manualOverrideEnabled = true;
	//Whether or not the relay is currently powered on manually
	volatile bool manualPoweredStatus = false;
	//Hours and minutes that the override turns on and off, converted from (start time + duration)
	int overrideHour = 0;
	int overrideMinute = 0;
	int overrideOffHour = 0;
	int overrideOffMinute = 0;
	//how long the temp override should last, this is converted into hours on/off
	int overrideDuration = 0;
	//Whether there is an override scheduled
	volatile bool overrideSetFlag = false;
	//Whether or not the relay is currently being overridden (within range of set override)
	volatile bool overrideStatus = false;
	//The state is the powered status of the relay if it is within the range of an override or schedule
	//Example: schedule is set from 1pm to 7pm, override is at 2pm for 30 min, set to OFF (1 - active LOW)
	//Example 2: sched is set from 1pm to 7pm, override is set at 8pm ON (0 - active LOW)
	//The sched state flag is useful when schedule is set from 9pm to 8am, for example
	//instead of ON from 9pm to 8am, turn it off from 8am to 9pm
	volatile bool overrideState = 0;
	volatile bool schedState = 0;
	//number of the physical pin the corresponding relay is connected to
	int relayPin;

	//For the manual override stuff
	bool getManualOverrideEnabled() { return manualOverrideEnabled; }
	bool getManualPoweredStatus() { return manualPoweredStatus; }
	void setManualOverrideEnabled() { manualOverrideEnabled = true; }
	void setManualPoweredStatus() { manualPoweredStatus = true; }
	void clearManualOverrideEnabled() {
		manualOverrideEnabled = false;
		manualPoweredStatus = false;
		digitalWrite(relayPin, HIGH);
	}
	void clearManualPoweredStatus() { manualPoweredStatus = false; }
	void flipManualOverrideFlag(void) { manualOverrideEnabled = (!manualOverrideEnabled); }
	void flipManualPoweredStatus() {
		digitalWrite(relayPin, manualPoweredStatus); //relay is ON (0) when status is TRUE (1) -> set relay to (1)...
		manualPoweredStatus = !manualPoweredStatus;
	}
	//For the schedule stuff
	//bool withinSchedRange();  <--returns true if current time is within sched range
	bool getSchedSetFlag() { return schedSetFlag; }
	bool getSchedState() { return schedState; }
	void setSchedFlag() { schedSetFlag = true, manualOverrideEnabled = false; } //PRECONDITION: relay is not powered on manually
	void setSchedState() { schedState = true; }	//most likely unnecessary
	void clearSchedSetFlag() { schedSetFlag = false, digitalWrite(relayPin, HIGH); }
	void clearSchedState() { schedState = false; }
	//for the override stuff
	bool getOverrideSetFlag() { return overrideSetFlag; }
	bool getOverrideStatus() { return overrideStatus; }
	void setOverrideStatus() { overrideStatus = true; }
	void setOverrideSetFlag() { overrideSetFlag = true, manualOverrideEnabled = false; } //PRECONDITION: relay is not powered on manually
	void clearOverrideStatus() { overrideStatus = false; }
	void clearOverrideSetFlag() {
		overrideSetFlag = false;
		overrideStatus = false;
		digitalWrite(relayPin, HIGH);
	}
	void setSchedOn(int hr, int min) {
		hourOn = hr;
		minuteOn = min;
	}
	void setSchedOff(int hr, int min) {
		hourOff = hr;
		minuteOff = min;
	}
	void off();
};

//These are for the interrupt
#define BUILT_IN_LED (1<<7)		//digital pin 13, PB7
#define DIGITAL_PIN_2 (1<<4)	//(OC3B/INT4) Port E bit 4
volatile byte counter = 0;
const int relayArrayPins[8] = { 42,43,44,45,46,47,48,49 };
outlets relay[8];
Adafruit_SSD1306 disp(128, 64);
RTC_DS3231 clockObj;
DateTime clockSecondObj;

//*****FUNCTION PROTOTYPES*****//
//***OLED Display Prototypes
void dispEnterDuration();
void printTime(int duration);
void printTime(int time, int loopCount);
void dispEnterTime(String top, String bottom);
void dispEnterPowerState();
void dispConfirmClearFlag();
void dispError(String error);
void dispOverrideMenu();
void dispSchedulesMenu();
void dispMainMenu();
void dispCurrentTime(int hour, int min);
void clearCurrentTime();
void dispEightRelays(int mode, String message = "", bool header = false);
void printHeader(String message);
void dispManualOverrideMenu();
void clearRelayUpdate();
void dispConfirmation();
void dispSingleSchedStatus(int relayNum);
void dispSingleOverrideStatus(int relayNum); //I have prototypes for functions up to here

//***Relay Function Prototypes
void allOff();
void initializeObjs();
int chooseRelay(int func);
void confirmClear(outlets& obj, void (outlets::* clearFunc)());
void completeOffSubMenu();
byte confirmationMenu();
void completeOffLoop();
void promptSchedTime(outlets& obj);
void promptOverrideTime(outlets& obj);

//***Clock Object functions
void updateClock();
void updateCurrentTime();
void dispCurrentTime(int hour, int min);

//***Sub Menu functions
void delayWithoutDelay(unsigned int time);
byte buttonPoll();
void waitForAnyLetterPress();
int inputTime();
int inputDuration();
byte inputPowerState();
int verifyHour(int hour);
int verifyMinute(int min);
int verifyDuration(int dur);

void schedulesMenu();
void setScheduleSubMenu();
void scheduleSetStatusSubMenu();
void scheduleSetStatusLoop();
void completeOffSubMenu();

void overrideMenu();
void overrideSubMenu();
void overrideStatusSubMenu();
void overrideStatusLoop();

void manualOverrideMenu();
void manualOnOffSubMenu();
void manualOnOffLoop();
void enableDisableRelaySubMenu();
void enableDisableRelayLoop();




void outlets::off() {
	clearManualOverrideEnabled();
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

ISR(INT4_vect) {
	sei();
	EIMSK = 0b00000000;
	EIFR |= DIGITAL_PIN_2;
	if (counter >= 2) {
		PORTB ^= BUILT_IN_LED;
		updateClock();
		//timeControl(clockSecondObj.day(), clockSecondObj.hour(), clockSecondObj.minute());
		counter = 0;
	}
	else {
		counter++;
	}
	EIMSK |= DIGITAL_PIN_2;
}

void setup() {
	initializeObjs();
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
//MAIN MENU***
void loop() {
	byte buttonData = buttonPoll();
	if ((buttonData & COL_BITS) == (COL_4)) {
		if (buttonData == NUM_PAD_A) {					//"A" on the num pad
			//This is the adjust schedule menu
			dispSchedulesMenu();
			schedulesMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_B) {					//"B" on the num pad
		   //This is the temporary override menu
			dispOverrideMenu();
			overrideMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_C) {					//C: on the num pad
			dispManualOverrideMenu();
			manualOverrideMenu();
			dispMainMenu();
		}
		if (buttonData == NUM_PAD_D) {			//D: More Options menu, incomplete
			dispMainMenu();
		}
		delayWithoutDelay(100);
	}
	delayWithoutDelay(80);
}

////------SUB MENUS (A, B, C, D FROM THE MAIN MENU------////
//Main Menu --> C. Enable/disable manual control of relays and manually control their power states
void manualOverrideMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)
			enableDisableRelaySubMenu();
		if (buttonPress == NUM_PAD_2)
			manualOnOffSubMenu();
		if (buttonPress == NUM_PAD_SHARP)
			return;
	}
	return;
}
//Main Manu --> B. Set/clear/view temporary override configs
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
//Main Menu --> A. Set/clear/view schedule configs
void schedulesMenu() {
	updateCurrentTime();
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
			//if (buttonPress == NUM_PAD_3) 	//Nothing here....	
			if (buttonPress == NUM_PAD_4) 		//Disable everything
				completeOffSubMenu();
			if (buttonPress == NUM_PAD_SHARP)	//Quit back
				return;
		}
		updateClock();
		if (currentMin != clockSecondObj.minute()) {
			clearCurrentTime();
			updateCurrentTime();
			currentMin = clockSecondObj.minute();
		}
	}
	return;
}
////------END SUB MENUS (A, B, C, D FROM THE MAIN MENU------////

////------SUB MENU SELECTED OPTION SUB MENU------////
//Main Menu -> C -> 1. Menu where manualOverrideFlag is flipped for each relay
void enableDisableRelaySubMenu() {
	dispEightRelays(1, "ON enables manual\ncontrol     * - back", true);
	enableDisableRelayLoop();
	dispManualOverrideMenu();
	return;
}
//MainMenu -> C -> 2. Menu where power state of relay is flipped, if manualOverrideFlag == false
void manualOnOffSubMenu() {
	dispEightRelays(2, "ON means ON OFF meansOFF, easy   * - back", true);
	manualOnOffLoop();
	dispManualOverrideMenu();
	return;
}
//Main Menu -> B -> 1. Menu where tempOverrideFlag is flipped
void overrideSubMenu() {
	dispEightRelays(3, "Select an outlet\n* - back", true);
	if (chooseRelay(1) == -1)
		dispError("failed to select relay");
	dispOverrideMenu();
	return;
}
//Main Menu -> B -> 2. Menu where a relay is selected and its tempOverride status is shown
void overrideStatusSubMenu() {
	dispEightRelays(3, "Choose one to show\nstatus   * - back", true);
	overrideStatusLoop();
	dispOverrideMenu();
	return;
}
//Main Manu -> A -> 1. Menu where schedule is set (or cleared if one exists)
void setScheduleSubMenu() {
	dispEightRelays(4, "Pick one to set \nschedule     * - back", true);
	if (chooseRelay(2) == -1)
		dispError("failed to select relay");
	dispSchedulesMenu();
	updateCurrentTime();
	return;
}
//Main Menu -> A -> 2. Menu where set schedule is viewed
void scheduleSetStatusSubMenu() {
	dispEightRelays(4, "Choose one to show\nstatus * -back", true);
	scheduleSetStatusLoop();
	dispSchedulesMenu();
	updateCurrentTime();	//this needs updating
	return;
}
//Main Menu -> A -> 4.
void completeOffSubMenu() {
	dispEightRelays(9, "Select an outlet to  be reset    * - back", true);
	completeOffLoop();
	delayWithoutDelay(1200);
	dispSchedulesMenu();
	updateCurrentTime();
	return;
}