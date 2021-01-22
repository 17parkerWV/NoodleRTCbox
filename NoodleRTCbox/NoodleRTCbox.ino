/*
delayWithoutDelay - has been caught in loop because the argument is always true?
millis() - This thing is giving me headaches but I don't know if something is wrong with it

Use my spare arduino mega to test hardware interrupts before getting close to this one
*/

#include "RelayObjects.h"
#include "SubMenus.h"
#include "DisplayUpdates.h"
//I can't get this working if these aren't here, even though they aren't needed here (I think). Oh well
#include <Adafruit_SPIDevice.h>
#include <Adafruit_I2CRegister.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_BusIO_Register.h>
#include <gfxfont.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>

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

RTC_DS3231 clockObj;
DateTime clockSecondObj;
SubMenu subMenuObj;

//These are for the interrupt
#define BUILT_IN_LED (1<<7)		//digital pin 13, PB7
#define DIGITAL_PIN_2 (1<<4)	//(OC3B/INT4) Port E bit 4
volatile int counter = 0;


//allows manual control of relay (not to be confused with manualOnOff, which actually turns them on/off)
void enableDisableRelay();
//menu to manually turn on/off a relay
void manualOnOff();

////------CLOCK FUNCTIONS AND OBJECTS------////
//sets the clock object equal to the other one
void updateClockObj();
//Prints the updated time at the top of the display in the schedule set sub menu
void updateCurrentTime();
////------END CLOCK FUNCTIONS AND OBJECTS------////

////------SUBMENU OPTIONS FUNCTIONS------////
//By options, I am talking about the options available AFTER selecting an option from the Main Menu
//These are bridges to functions in the SubMenuClass
//This brings you to the menu where you can override or not
void manualOverrideSubMenu();
//From the main menu - press A to get to the schedules menu
void schedulesSubMenu();
//Displays the title info for the schedules that are set
void scheduleSetStatus();
//Where you go to set a schedule
void setScheduleSubMenu();
//Shows options in the temporaryOverrideSubMenu
void temporaryOverrideSubMenu();
//completely turns off everything about a relay
void completeOffSubMenu();
////------END SUBMENU OPTIONS FUNCTIONS------////

//Displays and starts temporary override menu option
void temporaryOverride();
//Show the time and duration of the current overrides
void temporaryOverrideStatus();


ISR(INT4_vect) {
	//flips the BUILT IN LED HIGH when it starts and turns it off when it leaves
	counter++;
	if (counter >= 2) {
		updateClockObj();
		//subMenuObj.timeControl(clockSecondObj.day(), clockSecondObj.hour(), clockSecondObj.minute());
		counter = 0;
	}
}

void setup() {
	subMenuObj.initializePins();		//Initializes the relay pins and sets them as OUTPUT and turns them off()
	if (subMenuObj.initializeDisplay() == false) {		//Initializes the display object and also checks to make sure it began (begin())
		//Error message, maybe a colored LED?
	}
	if (!clockObj.begin()) {
		//Error message, maybe a colored LED?
	}
	if (clockObj.lostPower()) {
		//Error message, maybe a colored LED?
	}

	//FOR THE INTERRUPTS
	DDRB |= BUILT_IN_LED;		//set PB7 (digital pin 13) as output
	PORTB = 0b00000000;			//write digital pin 13 LOW and the other pins are INPUTS without the PULLUP
	clockObj.writeSqwPinMode(DS3231_SquareWave1Hz);	//Enable the 1Hz squarewave clock
	DDRE = 0b00000000;	//All bit in port E are inputs
	EIMSK = 0b00000000;	//All interrupts are masked out (recommended in the datasheet)
	MCUCR |= (0 << 4);		//Set the pullup disable bit LOW
	PORTE |= DIGITAL_PIN_2;		//Bit 4 has pullup resistor enabled
	EICRB = 0b00000010;			//Falling edge interrupt enabled on INT4 (pin 2)
	EIMSK |= DIGITAL_PIN_2;		//Bit 4 (INT4) interrupt mask bit to 1
	sei();

	//This is the interrupt pin, I think, it was set to pin 22, not 2, but I'm pretty sure that 22 was a mistake
	digitalWrite(2, INPUT_PULLUP);
	//Set up the keypad inputs/outputs
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;

	clockSecondObj = clockObj.now();
	//Maybe function to set the clock?
	//Function to notify that all schedules were wiped out
	subMenuObj.displayMainMenu();
}
//MAIN MENU***
void loop() {
	byte buttonData = buttonPoll();
	if ((buttonData & COL_BITS) == (COL_4)) {
		if (buttonData == NUM_PAD_A) {					//"A" on the num pad
			//This is the adjust schedule menu
			subMenuObj.displaySchedulesSubMenuDisplay();
			schedulesSubMenu();
			subMenuObj.displayMainMenu();
		}
		if (buttonData == NUM_PAD_B) {					//"B" on the num pad
		   //This is the temporary override menu
			subMenuObj.displayTempOverrideSubMenu();
			temporaryOverrideSubMenu();
			subMenuObj.displayMainMenu();
		}
		if (buttonData == NUM_PAD_C) {					//D: on the num pad
			subMenuObj.displayManualOverrideSubMenuDisplay();
			manualOverrideSubMenu();
			subMenuObj.displayMainMenu();
		}
		if (buttonData == NUM_PAD_D) {			//D: More Options menu, incomplete
			subMenuObj.displayMainMenu();
		}
		delayWithoutDelay(100);
	}
	delayWithoutDelay(80);
}

////------SUB MENUS (A, B, C, D FROM THE MAIN MENU------////
//Main Menu --> C. Enable/disable manual control of relays and manually control their power states
void manualOverrideSubMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)
			enableDisableRelay();
		if (buttonPress == NUM_PAD_2)
			manualOnOff();
		if (buttonPress == NUM_PAD_SHARP)
			return;
	}
	return;
}
//Main Manu --> B. Set/clear/view temporary override configs
void temporaryOverrideSubMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if (buttonPress == NUM_PAD_1)		//Button 1 goes to set/clear
			temporaryOverride();
		if (buttonPress == NUM_PAD_2) 		//Button 2 goes to show current status
			temporaryOverrideStatus();
		if (buttonPress == NUM_PAD_SHARP) 	//# sign to go back
			break;
	}
}
//Main Menu --> A. Set/clear/view schedule configs
void schedulesSubMenu() {
	updateCurrentTime();
	int currentMin = clockSecondObj.minute();
	unsigned long currentMillis = millis();
	while (1) {
		currentMillis = millis();
		while ((millis() - currentMillis) <= 5000L) {
			byte buttonPress = buttonPoll();
			if (buttonPress == NUM_PAD_1) 		//Set new schedule
				setScheduleSubMenu();
			if (buttonPress == NUM_PAD_2) 		//View schedules
				scheduleSetStatus();
			//if (buttonPress == NUM_PAD_3) 	//Nothing here....	
			if (buttonPress == NUM_PAD_4) 		//Disable everything
				completeOffSubMenu();
			if (buttonPress == NUM_PAD_SHARP)	//Quit back
				return;
		}
		updateClockObj();
		if (currentMin != clockSecondObj.minute()) {
			subMenuObj.clearCurrentTime();
			updateCurrentTime();
			currentMin = clockSecondObj.minute();
		}
	}
}
////------END SUB MENUS (A, B, C, D FROM THE MAIN MENU------////

////------SUB MENU SELECTED OPTION SUB MENU------////
/*
*
*
///---These are called when a selection is made in their parents' sub menu in the .ino---///
//--"Parent sub menu" refers to the submenu from which these functions (made only with functions) were called from
//-Or the previous while(1) menu where these functions exist
/*
These all have the exact same layout:
void [SubMenuOption]{
	*Display 8 relay numbers (most likely)*
	subMenuObj.[displaySubmenuOptionScreen]()
	*subMenuObj.[SubMenuOptionScreenStatus]()	//(most likely called, to print the statuses)
	subMenuObj.[SubMenuClassMemberFunction])()	//This goes to the while(1) loop, waiting to receive an input
	subMenuObj.[displayParentSubMenu]();
}


*/
//Main Menu -> C -> 1. Menu where manualOverrideFlag is flipped for each relay
void enableDisableRelay() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("ON enables manual\ncontrol     * - back"));
	subMenuObj.displayStatuses(subMenuObj.getManualFlag);
	subMenuObj.enableDisableRelaySubMenu();
	subMenuObj.displayManualOverrideSubMenuDisplay();
}
//MainMenu -> C -> 2. Menu where power state of relay is flipped, if manualOverrideFlag == false
void manualOnOff() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("ON means ON OFF meansOFF, easy   * - back"));
	subMenuObj.displayStatuses(subMenuObj.getPower);
	subMenuObj.manualOnOffSubMenu();
	subMenuObj.displayManualOverrideSubMenuDisplay();
}
//Main Menu -> B -> 1. Menu where tempOverrideFlag is flipped
void temporaryOverride() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("Select an outlet\n* - back"));
	subMenuObj.displayStatuses(subMenuObj.getTempFlag);
	subMenuObj.chooseRelay(subMenuObj.getTempFlag, subMenuObj.clearTempOverrideFlag, subMenuObj.promptOverrideFunc);
	subMenuObj.displayTempOverrideSubMenu();
}
//Main Menu -> B -> 2. Menu where a relay is selected and its tempOverride status is shown
void temporaryOverrideStatus() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("Choose one to show\nstatus   * - back"));
	subMenuObj.displayStatuses(subMenuObj.getTempFlag);
	subMenuObj.tempOverrideStatusWhileLoop();
	subMenuObj.displayTempOverrideSubMenu();
}
//Main Manu -> A -> 1. Menu where schedule is set (or cleared if one exists)
void setScheduleSubMenu() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("Pick one to set \nschedule     * - back"));
	subMenuObj.displayStatuses(subMenuObj.getSchedFlag);
	subMenuObj.chooseRelay(subMenuObj.getSchedFlag, subMenuObj.clearScheduleSetFlag, subMenuObj.promptSchedFunc);
	subMenuObj.displaySchedulesSubMenuDisplay();
	updateCurrentTime();
}
//Main Menu -> A -> 2. Menu where set schedule is viewed
void scheduleSetStatus() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("Choose one to show\nstatus * -back"));
	subMenuObj.displayStatuses(subMenuObj.getSchedFlag);
	subMenuObj.scheduleSetStatusWhileLoop();
	subMenuObj.displaySchedulesSubMenuDisplay();
	updateCurrentTime();
}
//Main Menu -> A -> 4.
void completeOffSubMenu() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayHeader(F("Select an outlet to  be reset    * - back"));
	subMenuObj.completeOffSubMenu();
	delayWithoutDelay(1200);
	subMenuObj.displaySchedulesSubMenuDisplay();
	updateCurrentTime();
}
////------END SUB MENU SELECTED OPTION SUB MENU------////

////------UPDATE OBJECT FUNCITONS------////
//The RTC's data are mainly accessed through an object from a different class. This updates the object
void updateClockObj() {
	clockSecondObj = clockObj.now();
}
//Main Menu -> A. The time is shown in the upper left, this updates it, checking it every couple seconds
void updateCurrentTime() {
	updateClockObj();
	subMenuObj.displayCurrentTime(clockSecondObj.hour(), clockSecondObj.minute());
}
////------END UPDATE OBJECT FUNCITONS------////