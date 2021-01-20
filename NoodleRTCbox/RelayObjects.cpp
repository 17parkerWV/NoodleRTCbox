#include "RelayObjects.h"

//These functions control power and time
void Relay::setTimeOn(int hr, int min) {
	schedules.hourOn = hr;
	schedules.minuteOn = min;
}

void Relay::setTimeOff(int hr, int min) {
	schedules.hourOff = hr;
	schedules.minuteOff = min;
}

//scheduleSetFlag should ALWAYS BE FALSE when the manualOverrideFlag is TRUE, so this does not check scheduleSetFlag
//As a precaution, this is only meant to be called
//powered CAN ONLY BE CHANGED IF THE OVERRIDE FLAG IS TRUE (which should only be true if scheduleSetFlag is FALSE)
void Relay::flipPowerState(void) {
	bool power;
	schedules.manualOverrideFlag = true;
	schedules.powered = (!schedules.powered);		//Flip the state of the flag
	power = !schedules.powered;						//flip it back because the relays are active LOW
	digitalWrite(schedules.relayPin, power);	//Write power state to the pin, should mae sure this is bullet proof
}

bool Relay::getPowerStatus() {
	return schedules.powered;
}

void Relay::setPoweredState() {
	schedules.powered = true;
	schedules.manualOverrideFlag = true;
}

void Relay::clearPoweredState() {
	schedules.powered = false;
}

//schedules.scheduleSetFlag MUST BE FALSE GOING INTO THIS TO PREVENT SOME OBSCURE BUG FROM GIBING ME YEARS OF PAIN
void Relay::flipManualOverrideFlag(void) {
	schedules.manualOverrideFlag = (!schedules.manualOverrideFlag);
}

void Relay::setManualOverrideFlag() {
	schedules.manualOverrideFlag = true;
}

//clears manualoverride flag, powered flag, and writes the relay HIGH (turns it off)
void Relay::clearManualOverrideFlag() {
	schedules.manualOverrideFlag = false;
	schedules.powered = false;
	digitalWrite(schedules.relayPin, HIGH);
}

bool Relay::getManualOverrideFlagStatus() {
	return schedules.manualOverrideFlag;
}

void Relay::off(void) {
	digitalWrite(schedules.relayPin, HIGH);
	clearPoweredState();
	clearScheduleSetFlag();
	clearTempOverrideFlag();
	setManualOverrideFlag();
	clearTempOverrideStarted();
}

bool Relay::getScheduleSetFlagStatus() {
	return schedules.scheduleSetFlag;
}

void Relay::setScheduleSetFlag() {
	schedules.scheduleSetFlag = true;
	schedules.manualOverrideFlag = false;
}

void Relay::clearScheduleSetFlag() {
	schedules.scheduleSetFlag = false;
	schedules.manualOverrideFlag = false; //Redundant, it should be false already to be able to clear scheduleSetFlag
}

////------TEMPORARY OVERRIDE FUNCTIONS------

//Clears the temp override flag, AND the manual override flag, and the tempOverrideStartedFlag AND turns relay off if there is no schedule set at the moment
void Relay::clearTempOverrideFlag() {
	clearManualOverrideFlag();
	schedules.tempOverrideFlag = false;
	clearTempOverrideStarted();
	if (getScheduleSetFlagStatus() == false)
		digitalWrite(schedules.relayPin, HIGH);
}

void Relay::setTempOverrideFlag() {
	clearManualOverrideFlag();
	schedules.tempOverrideFlag = true;
}

byte Relay::getTempOverrideState() {
	return schedules.tempOverrideState;
}

bool Relay::getTempOverrideStatus() {
	return schedules.tempOverrideFlag;
}

void Relay::setTempOverrideStarted() {
	schedules.tempOverrideStarted = true;
}

void Relay::clearTempOverrideStarted() {
	schedules.tempOverrideStarted = false;
}

bool Relay::getTempOverrideStartedStatus() {
	return schedules.tempOverrideStarted;
}
////------TEMPORARY OVERRIDE FUNCTIONS------//