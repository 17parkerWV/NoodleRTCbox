// SubMenus.h

#ifndef _SUBMENUS_h
#define _SUBMENUS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#define COL_LOW_ROW_PULLUP ROW_BITS
#define ROW_LOW_COL_PULLUP COL_BITS
#define ROW_IN_COL_OUT COL_BITS
#define COL_IN_ROW_OUT ROW_BITS
byte buttonPress = 0b00000000;

#endif

