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

class Relay {
public:
	friend class SubMenu;
		//pins of the relays
	const int relayArrayPins[8] = { 42,43,44,45,46,47,48,49 };	
		//Turns off specified relay
	void off(void);								
		//Sets the time it will turn on
	void setTimeOn(int, int, int, int);			
		//sets the time it wil turn off
	void setTimeOff(int, int, int, int);		
		//flip the state of power (on or off) and update powered flag
	void flipPowerState(void);		
		//flip the state of the overrideFlag
	void flipOverrideState(void);		

private:
		//This struct is going to hold the scheduled times, mostly initialized because none of this needs to be initailized differently
	struct schedules {
		int monthOn = 0;
		int dayOn = 0;
		int hourOn = 0;
		int minuteOn = 0;
		int monthOff = 0;
		int dayOff = 0;
		int hourOff = 0;
		int minuteOff = 0;
			//Whether or not it is being overridden, flag for other parts of the program
		bool overrideFlag = true;	
			//Whether or not it is on or off (for manual override) power state of the relay must be in sync with this flag!!
		bool powered = false;		
			//Override flag and powered are more or less local to the override menu, so this is the flag that will that menu to the schedule menus without mixing too much
			//Specifically, if the relay is overriden, it is tripped and can only be reset by setting a schedule again
		bool scheduleSetFlag = false;
		int overrideHour = 0;				
		int overrideMinute = 0;		
			//number of the physical pin the corresponding relay is connected to
		int relayPin;						
	} schedules;
};
#endif 