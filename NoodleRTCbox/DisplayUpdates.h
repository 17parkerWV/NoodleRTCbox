// DisplayUpdates.h
#ifndef _DISPLAYUPDATES_h
#define _DISPLAYUPDATES_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
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
#else
#include "WProgram.h"
#endif

extern void delayWithoutDelay(int);


#endif