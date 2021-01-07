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

class Relay {
public:
	void allOff(Relay*);							//turns off ALL relays
	void off(void);									//Turns off specified relay
	void initializePins(Relay pwrArr[], int pinArr[]);			//This is called once at the beginning, to set the relays to their controlling pin
	void setTimeOn(int, int, int, int);			//Sets the time it will turn on
	void setTimeOff(int, int, int, int);		//sets the time it wil turn off
	void tempOverride(int, int);				//Temporarily override schedule (on or off), delay is in hours and minutes
	void manualOverride(int state);					//enables/disables the relay indefinitely, overriding the schedule
	void checkSchedule(void);						//After a relay comes out of being overridden, it needs to check if it is scheduled to be on
	void flipPowerState(void);							//flip the state of power (on or off) and updating the powerState flag 
	void flipOverrideState(void);
	void setPowerState(bool level);				//manually change the state to given level
	int* getPowerStates(Relay pwrArr[]);						//To return states (enable/disable) of the outlets
	int* getEnableStates(Relay pwrArr[]);
	bool getOverrideStatus();							//returns the current override status
	bool getPoweredStatus();
	void setOverrideFlag(bool);							//sets the flag to state that is passed as argument

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
		bool overrideFlag = true;		//Whether or not it is being overridden, flag for other parts of the program
		bool powered = false;			//Whether or not it is on or off (for manual override)
		int overrideHour = 0;				//the hour the override ends
		int overrideMinute = 0;				//the minute the override is over
		int relayPin;						//needs to get set by the setPins() function
	} schedules;
};
#endif 