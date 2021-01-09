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
	digitalWrite(this->schedules.relayPin, (!state));
}

void Relay::checkSchedule(void) {

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

void Relay::off(void) {
	digitalWrite(schedules.relayPin, HIGH);		//I'm pretty sure the relays turn ON when dragged low, need to double check
	schedules.powered = false;					//The one I want to use for changing settings but probably ditch the other one because it's annoying
}
