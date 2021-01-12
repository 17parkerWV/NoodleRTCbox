#include "RelayObjects.h"

//These functions control power and time
void Relay::setTimeOn(int mon, int day, int hr, int min) {
	schedules.monthOn = mon;
	schedules.dayOn = day;
	schedules.hourOn = hr;
	schedules.minuteOn = min;
}

void Relay::setTimeOff(int mon, int day, int hr, int min) {
	schedules.monthOff = mon;
	schedules.dayOff = day;
	schedules.hourOff = hr;
	schedules.minuteOff = min;
}

//scheduleSetFlag should ALWAYS BE FALSE when the manualOverrideFlag is TRUE, so this does not check scheduleSetFlag
//As a precaution, this is only meant to be called
//powered CAN ONLY BE CHANGED IF THE OVERRIDE FLAG IS TRUE (which should only be true if scheduleSetFlag is FALSE)
void Relay::flipPowerState(void) {
	bool power;
	schedules.powered = (!schedules.powered);		//Flip the state of the flag
	power = !schedules.powered;						//flip it back because the relays are active LOW
	digitalWrite(schedules.relayPin, power);	//Write power state to the pin, should mae sure this is bullet proof
}

//schedules.scheduleSetFlag MUST BE FALSE GOING INTO THIS TO PREVENT SOME OBSCURE BUG FROM GIBING ME YEARS OF PAIN
void Relay::flipManualOverrideState(void) {
	schedules.manualOverrideFlag = (!schedules.manualOverrideFlag);
}

void Relay::off(void) {
	digitalWrite(schedules.relayPin, HIGH);		//I'm pretty sure the relays turn ON when dragged low, need to double check
	schedules.powered = false;					//The one I want to use for changing settings but probably ditch the other one because it's annoying
	schedules.scheduleSetFlag = false;
}

//schedules.powered MUST BE FALSE GOING INTO THIS, TO PREVENT COLLISION BETWEEN SCHEDULED POWER and this one
//This sets manualOverrideFlag to FALSE and can only set it to FALSE
//Can only be called if powered is FALSE, at that point this can override the manualOverrideFlag (irony?) to prevent the schedule from being overwritten
void Relay::flipScheduleSetFlag() {
	schedules.manualOverrideFlag = false;
	schedules.scheduleSetFlag = (!schedules.scheduleSetFlag);
}


//TEMPORARY OVERRIDE FUNCTIONS
void Relay::clearTempOverrideFlag() {
	schedules.tempOverrideFlag = false;
}

void Relay::setTempOverrideFlag() {
	schedules.tempOverrideFlag = true;
}