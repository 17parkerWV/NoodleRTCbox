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