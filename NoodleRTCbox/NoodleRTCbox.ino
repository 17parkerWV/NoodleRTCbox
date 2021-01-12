/*
delayWithoutDelay - has been caught in loop because the argument is always true?
millis() - This thing is giving me headaches but I don't know if something is wrong with it

Use my spare arduino mega to test hardware interrupts before getting close to this one
*/

#include "RelayObjects.h"
#include "SubMenus.h"
#include "DisplayUpdates.h"
//I can't get this working if these aren't here, even though they aren't needed here. Oh well
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
#define DIGITAL_PIN_2 (1<<4)	//(OC3B/INT4) Port E bit 4
volatile byte counter = 0b00000000;
volatile byte state = 0b00000001;

ISR(INT4_vect) {
	counter++;
	if (counter >= 4) {
		digitalWrite(13, state);
		state ^= (0b00000001);
		counter = 0;
	}
}

void errorQuit(int code);	//Print error code
void printTime(void);		//print current date/time

	//allows manual control of relay (not to be confused with manualOnOff, which actually turns them on/off)
void enableDisableRelay();	
	//menu to manually turn on/off a relay
void manualOnOff();				
	//sets the clock object equal to the other one
void updateClockObj();
	//This brings you to the menu where you can override or not
void manualOverrideSubMenu();		
	//From the main menu - press A to get to the schedules menu
void schedulesSubMenu();
	//Prints the updated time at the top of the display
void updateCurrentTime();
	//Displays the title info for the enable/disable schedule submenu
void enableDisableScheduleSubMenu();
	//Displays and starts temporary override menu option
void temporaryOverride();

void setup() {
	Serial.begin(115200);
	subMenuObj.initializePins();		//Initializes the relay pins and sets them as OUTPUT and turns them off()
	if (subMenuObj.initializeDisplay() == false)		//Initializes the display object and also checks to make sure it began (begin())
		errorQuit(3);
	if (!clockObj.begin())
		errorQuit(1);
	if (clockObj.lostPower())
		errorQuit(2);

	//FOR THE INTERRUPTS
	pinMode(13, OUTPUT);
	digitalWrite(13, LOW);
	clockObj.writeSqwPinMode(DS3231_SquareWave1Hz);	//Enable the 1Hz squarewave clock
	DDRE = 0b00000000;	//All bit in port E are inputs
	EIMSK = 0b00000000;	//All interrupts are masked out (recommended in the datasheet)
	PORTE |= DIGITAL_PIN_2;		//Bit 4 has pullup resistor enabled
	EICRB = 0b00000010;			//Falling edge interrupt enabled on INT4 (pin 2)
	EIMSK |= DIGITAL_PIN_2;		//Bit 4 (INT4) interrupt mask bit to 1
	sei();


	digitalWrite(22, INPUT_PULLUP);
	//Set up the keypad inputs/outputs
	DDRC = ROW_IN_COL_OUT;
	PORTC = COL_LOW_ROW_PULLUP;

	clockSecondObj = clockObj.now();
	//Maybe function to set the clock?
	//Function to notify that all schedules were wiped out
	subMenuObj.displayMainMenu();
}


void loop() {
	byte buttonData = buttonPoll();
	if ((buttonData & COL_BITS) == (COL_4)) {
		if ((buttonData & ROW_BITS) == (ROW_1)) {					//"A" on the num pad
			//This is the adjust schedule menu
			subMenuObj.displaySchedulesSubMenuDisplay();
			schedulesSubMenu();
			subMenuObj.displayMainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_2)) {					//"B" on the num pad
		   //This is the temporary override menu
			temporaryOverride();
			subMenuObj.displayMainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_3)) {					//"C" on the num pad
			//This goes to the enable/disable manual control and turn on/off relays
			subMenuObj.displayManualOverrideSubMenuDisplay();
			manualOverrideSubMenu();
			subMenuObj.displayMainMenu();
		}
		if ((buttonData & ROW_BITS) == (ROW_4)) {					//"D" on the num pad
		   //This is the more options menu
			subMenuObj.displayMainMenu();
		}
		delayWithoutDelay(100);
	}
	delayWithoutDelay(80);
}



void manualOverrideSubMenu() {
	while (1) {
		byte buttonPress = buttonPoll();
		if ((buttonPress & COL_BITS) == COL_4) {
			if ((buttonPress & ROW_BITS) == ROW_1) {		//Button A goes to enable disable
				enableDisableRelay();
			}
			if ((buttonPress & ROW_BITS) == ROW_2) {		//Button B goes to manual ON/OFF menu
				manualOnOff();
			}
		}
		if ((buttonPress & COL_BITS) == COL_3) {
			if ((buttonPress & ROW_BITS) == ROW_4) {	//# sign to go back
				break;
			}
		}
	}
}

void enableDisableRelay() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayEnableDisableRelayScreen();
	subMenuObj.displayOverrideScreenStatus();
	subMenuObj.enableDisableRelaySubMenu();
	subMenuObj.displayManualOverrideSubMenuDisplay();
}

void manualOnOff() {						
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayManualOnOffScreen();
	subMenuObj.displayOnOffScreenStatus();
	subMenuObj.manualOnOffSubMenu();
	subMenuObj.displayManualOverrideSubMenuDisplay();
}

void temporaryOverride() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayTemporaryOverrideDisplay();		//Function to display the header info
	subMenuObj.displayTemporaryOverrideStatus();		//function to display status of temporary override flags
	subMenuObj.chooseRelay();
	//function to go into submenu while(1) loop
	//Don't think I need a display function here as long as things return correctly
}

void schedulesSubMenu() {
	updateCurrentTime();
	int currentMin = clockSecondObj.minute();
	unsigned long currentMillis = millis();
	while (1) {
		currentMillis = millis();
		while ((millis()-currentMillis)<=5000L) {
			byte buttonPress = buttonPoll();
			if ((buttonPress & ROW_BITS) == ROW_1) {
				if ((buttonPress & COL_BITS) == COL_1) {		//Row 1 Col 1 ->button 1

				}
				if ((buttonPress & COL_BITS) == COL_2) {		//Row 1 Col 2 ->button 2

				}
				if ((buttonPress & COL_BITS) == COL_3) {		//Row 1 Col 3 ->button 3 //C: Enable/disable schedules
					enableDisableScheduleSubMenu();
				}
			}
			if ((buttonPress & ROW_BITS) == ROW_2) {
				if ((buttonPress & COL_BITS) == COL_1) {		//Row 2 Col 1 ->button 4

				}
			}
			if (((buttonPress & ROW_BITS) == ROW_4) && ((buttonPress & COL_BITS) == COL_3)) {	//Row 4 Col 3 -> # sign to exit
				return;
			}
		}
		updateClockObj();
		if (currentMin != clockSecondObj.minute()) {
			subMenuObj.clearCurrentTime();
			updateCurrentTime();
			currentMin = clockSecondObj.minute();
		}
	}
}

void enableDisableScheduleSubMenu() {
	subMenuObj.displayEightRelayNumbers();
	subMenuObj.displayEnableDisableScheduleScreen(); //function to display the correct header info for that screen
	subMenuObj.displayScheduleSetFlagStatus(); //function to print the status of the schedules
	subMenuObj.enableDisableSchedulesSubMenu();//function to go into the sub menu - the while(1) loop
	subMenuObj.displaySchedulesSubMenuDisplay(); //The function that returns us back to the menu
	updateCurrentTime();		//Updates the clock object and prints it, otherwise the time will be missin upon return
}

void updateClockObj() {
	clockSecondObj = clockObj.now();
}

void updateCurrentTime() {
	updateClockObj();
	subMenuObj.displayCurrentTime(clockSecondObj.hour(), clockSecondObj.minute());
}

void errorQuit(int code) {
	switch (code) {
	case 1:
		Serial.println(F("Clock failed to begin, contact support"));
		break;
	case 2:
		Serial.println(F("RTC lost power, reset time and schedules"));
		break;
	case 3:
		Serial.println(F("Display failed to start"));
		break;
	default:
		Serial.println(F("Unspecified error"));
		break;
	}
	subMenuObj.allOff();
	for (;;) {};
}

void printTime() {
	updateClockObj;
	Serial.println(clockSecondObj.year());
	Serial.println(clockSecondObj.month());
	Serial.println(clockSecondObj.day());
	Serial.println(clockSecondObj.hour());
	Serial.println(clockSecondObj.minute());
	Serial.println(clockSecondObj.second());
}