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

void Relay::tempOverride(int hours, int minutes) {

}

void Relay::manualOverride(int state) {
	digitalWrite(schedules.relayPin, (!state));
}

void Relay::checkSchedule(void) {

}

bool Relay::getOverrideStatus() {
	return schedules.overrideFlag;
}

bool Relay::getPoweredStatus(void) {
	return schedules.powered;
}

void Relay::flipPowerState(void) {
	bool power;
	schedules.powered = (!schedules.powered);		//Flip the state of the flag
	power = !schedules.powered;						//flip it back because the relays are active LOW
	digitalWrite(schedules.relayPin, power);	//Write power state to the pin, should mae sure this is bullet proof
}

void Relay::flipOverrideState(void) {
	schedules.overrideFlag = (!schedules.overrideFlag);
}

void Relay::setPowerState(bool level) {
	schedules.powered = level;
	digitalWrite(schedules.relayPin, level);
}

void Relay::setOverrideFlag(bool state) {
	schedules.overrideFlag = state;
}

int* Relay::getPowerStates(Relay pwrArr[]) {
	static int powerStateArray[8];
	for (int start = 0; start <= 7; start++) {
		powerStateArray[start] = pwrArr[start].schedules.powered;
	}
	delay(250);
	return powerStateArray;
}

int* Relay::getEnableStates(Relay pwrArr[]) {
	static int enableStates[8];
	for (int start = 0; start <= 7; start++) {
		enableStates[start] = pwrArr[start].schedules.overrideFlag;
	}
	delay(250);
	return enableStates;
}

void Relay::off(void) {
	digitalWrite(schedules.relayPin, HIGH);		//I'm pretty sure the relays turn ON when dragged low, need to double check
	schedules.powered = false;					//The one I want to use for changing settings but probably ditch the other one because it's annoying
}


void Relay::allOff(Relay* pwrArr) {
	for (int index = 0; index <= 7; index++) {
		pwrArr[index].off();
	}
}

void Relay::initializePins(Relay pwrArr[], int pinArr[]) {						//only to be ran once, when it is powered on
	for (int i = 0; i <= 7; i++) {
		pwrArr[i].schedules.relayPin = pinArr[i];
		pwrArr[i].off();
	}
}