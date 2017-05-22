/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Tiny Dc/Dcc controler sample>
*************************************************************/

#include "French16.hpp"
#include "LcdUi.h"

//#include <NewLiquidCrystal_I2C.h>
#include <NewLiquidCrystal.h>
#include "ScreenLiquidNew.hpp"

//#include <LiquidCrystal.h>
//#include "ScreenLiquid.hpp"

#include "EEPROMextent.h"
#include "DcDccControler.h"

//NewLiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
NewLiquidCrystal lcd(8, 9, 4, 5, 6, 7);
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

ScreenLiquidNew screen;
//ScreenLiquid screen;

Handle handle;

ButtonsCommanderAnalogPushes pushes;

void setup()
{
	int v;
extern int __heap_start, *__brkval;
	Serial.begin(115200);

	// Buttons on a 'LCD Keypad shield', handled in analog way.
	int values[] = { 0, 145, 329, 505, 741 };
	// Each button as an EVENT number as ID ! Easier for the loop !
	// One button has no meaning with 1004 (why not ?) as ID.
	unsigned long ids[] = { EVENT_EMERGENCY, LCD1_EVENT_MORE, LCD1_EVENT_LESS, LCD1_EVENT_CANCEL, LCD1_EVENT_SELECT };

	pushes.begin(A0, 5, ids, values, 50);
	
	DcDccControler::begin(true, 11, 10);    // Dc: Pwm, Dir / Dcc : Pwm, unused

	handle.begin();

	//screen.begin(20, 4, string_table, &lcd);
	screen.begin(16, 2, string_table, &lcd);
	handle.GetUI()->begin(&screen);

	DcDccControler::AddHandle(&handle);
Serial.println("7");
Serial.println((int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval));
delay(1000);
}

void loop()
{
	unsigned long event = Commanders::loop();

	// For LcdUi, UNDEFINED_ID of Commanders has no meaning. And because it is necessary 
	// to execute lcdui.event() at each call of the main loop, 
	// do it with an empty event : EVENT_NONE.

	if (event == UNDEFINED_ID)
		event = EVENT_NONE;

	DcDccControler::loop(event);
}

