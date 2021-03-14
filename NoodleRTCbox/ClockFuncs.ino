void updateClock() {
	clockSecondObj = clockObj.now();
	return;
}
void updateCurrentTime() {
	updateClock();
	dispCurrentTime(clockSecondObj.hour(), clockSecondObj.minute());
	return;
}
void dispCurrentTime(int hour, int min) {
	disp.setTextSize(2);
	disp.setCursor(0, 0);
	disp.print(hour);
	disp.print(":");
	if (min < 10)
		disp.print(0);
	disp.print(min);
	disp.display();
	return;
}
void timeControl() {
	int currentDay = clockSecondObj.day();
	int currentHour = clockSecondObj.hour();
	int currentMinute = clockSecondObj.minute();
	for (int index = 0; index <= 7; ++index) {
		if (relay[index].getOverrideStatus() == false && relay[index].getSchedSetFlag() == false)
			continue;
		if (relay[index].getOverrideStatus() == true) {
			//If inside range of override
			if ((relay[index].overrideHour <= currentHour) && (relay[index].overrideMinute <= currentMinute) && (relay[index].overrideOffHour >= currentHour) && (relay[index].overrideOffMinute > currentMinute)) {
				relay[index].setOverrideStatus();
				digitalWrite(relay[index].relayPin, relay[index].overrideState);
			}
			//If inside before but now outside, set powered status to what it was before
			if ((relay[index].overrideOffHour <= currentHour) && (relay[index].overrideOffMinute <= currentMinute)) {
				relay[index].clearOverrideSetFlag();
				if (relay[index].getSchedSetFlag() == false) 
					digitalWrite(relay[index].relayPin, !relay[index].getManualPoweredStatus());
			}
			continue;
		}
		//If a schedule is set, turn on/off
		//If sched is set overnight, time ON and time OFF were swapped, and the sched state is inverted (happened when sched was set)
		if (relay[index].getSchedSetFlag() == true) {
			if ((relay[index].hourOn <= currentHour) && (relay[index].minuteOn <= currentMinute) && (relay[index].hourOff >= currentHour) && (relay[index].minuteOff > currentMinute)) 
				digitalWrite(relay[index].relayPin, relay[index].schedState);
			if ((relay[index].hourOff <= currentHour) && (relay[index].minuteOff <= currentMinute))
				digitalWrite(relay[index].relayPin, !relay[index].schedState);
		}
	}
	return;
}