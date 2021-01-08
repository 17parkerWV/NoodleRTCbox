#include "DisplayUpdates.h"

DisplayClass::DisplayClass(int w, int h) {
	Adafruit_SSD1306 OLED(w, h);

}


void mainMenu(void) {
	OLEDdisplay.clearDisplay();
	OLEDdisplay.display();
	OLEDdisplay.setTextSize(2);
	OLEDdisplay.setCursor(10, 0);
	OLEDdisplay.println(F("Main Menu"));
	OLEDdisplay.setTextSize(1);
	OLEDdisplay.setCursor(0, 16);
	OLEDdisplay.println("A: Adjust Schedule");
	OLEDdisplay.println(F("B: Temporary Override"));
	OLEDdisplay.println(F("C: Manual Override      Menu"));
	OLEDdisplay.println(F("D: More options"));
	OLEDdisplay.display();
}
