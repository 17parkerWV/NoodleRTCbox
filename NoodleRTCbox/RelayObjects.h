// RelayObjects.h
#ifndef _RELAYOBJECTS_h
#define _RELAYOBJECTS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

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

class Relay {
public:
	friend class SubMenu;

	//pins of the relays
	const int relayArrayPins[8] = { 42,43,44,45,46,47,48,49 };
	//Turns off specified relay
	void off(void);
	//Sets the time it will turn on
	void setTimeOn(int, int);
	//sets the time it wil turn off
	void setTimeOff(int, int);

	//***POWERED FUNCTIONS***//
		//flip the state of power (on or off) and update powered flag
	void flipPowerState(void);
	//Return the powered status of the object
	bool getPowerStatus();
	//clear the powered status
	void clearPoweredState();
	//set powered state
	void setPoweredState();

	//***SCHEDULE FUNCTIONS***//
		//Set the schedule set flag
	void setScheduleSetFlag();
	//Clear the schedule set flag
	void clearScheduleSetFlag();
	//return the status of the schedule ste flag
	bool getScheduleSetFlagStatus();

	//***MANUAL OVERRIDE FUNCTIONS***//
		//set the manualoverride Flag
	void setManualOverrideFlag();
	//clear the manual override flag
	void clearManualOverrideFlag();
	//return the status of the manual override flag
	bool getManualOverrideFlagStatus();
	//flip the state of the manualOverrideFlag. schedule set flag must be FALSE
	void flipManualOverrideFlag(void);

	//***TEMPORARY OVERRIDE FUNCTIONS***//
	//The only place where anything temp override is accessed without functions is in displaySingleObjectTempOverrideStatus is in SubMenus.cpp, just for displaying purposes
	//Clears the temp override flag
	void clearTempOverrideFlag();
	//Set the tempoverride flag
	void setTempOverrideFlag();
	//return the status of the tempoverride flag
	bool getTempOverrideStatus();
	//return the state the override is in
	byte getTempOverrideState();

	//set the status of the tempOverrideStarted flag
	void setTempOverrideStarted();
	//clear the status of the tempOverrideStarted flag
	void clearTempOverrideStarted();
	//return the status of the tempOverrideStarted flag
	bool getTempOverrideStartedStatus();

private:
	//This struct is going to hold the scheduled times, mostly initialized because none of this needs to be initailized differently
	struct schedules {
		int hourOn = 0;
		int minuteOn = 0;
		int hourOff = 0;
		int minuteOff = 0;
		//Whether or not it is being overridden, flag for other parts of the program
		volatile bool manualOverrideFlag = true;
		//Whether or not it is on or off (for manual override) power state of the relay must be in sync with this flag!!
		volatile bool powered = false;
		//Override flag and powered are more or less local to the override menu, this flag will communicate if a schedule is set
		//Specifically, if the relay is overriden, it is tripped and can only be reset by setting a schedule again
		//Rule 1: If the schedule flag is set, it cannot be manually controlled
		//Rule 2: To set the flag, a schedule must be set
		//Rule 3: The schedule flag can only be cleared in the schedules menu
		volatile bool scheduleSetFlag = false;
		//These three store the hour and minute of the start of the temporary override, and the end time
		int tempOverrideHour = 0;
		int tempOverrideMinute = 0;
		int tempOverrideOffDayOffset = 0;	//NOT SURE if I'll need this one, it would be used to help with hour/day overflow
		int tempOverrideOffHour = 0;
		int tempOverrideOffMinute = 0;
		//Whether there is a temporary override in place
		volatile bool tempOverrideFlag = false;
		//There are a lot of little issues that can come up and these flags will keep track if override has started/in progress
		volatile bool tempOverrideStarted = false;
		//how long the temp override should last
		int tempOverrideDuration = 0;
		//Whether the temporary override is ON (0) or OFF (1)	IT IS LIKE THIS BECAUSE THE RELAYS ARE "ACTIVE LOW"
		volatile byte tempOverrideState = 0b00000000;
		//number of the physical pin the corresponding relay is connected to
		int relayPin;
	} schedules;
};
#endif 