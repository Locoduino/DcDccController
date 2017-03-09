/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Tiny Dc/Dcc controler sample>
*************************************************************/

#include "French16.hpp"

//#include <NewLiquidCrystal_I2C.h>
//#include <NewLiquidCrystal.h>
#include <LiquidCrystal.h>
//#include "ScreenLiquidNew.hpp"
#include "ScreenLiquid.hpp"
#include "EEPROMextent.h"
#include "DcDccControler.h"

//NewLiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
//NewLiquidCrystal lcd(8, 9, 4, 5, 6, 7);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

//ScreenLiquidNew screen;
ScreenLiquid screen;

Handle handle;

#ifdef VISUALSTUDIO
ButtonsCommanderKeyboard push0;
ButtonsCommanderKeyboard push1;
ButtonsCommanderKeyboard push2;
ButtonsCommanderKeyboard push3;
ButtonsCommanderKeyboard pushDcDcc;
ButtonsCommanderKeyboard pushEmergency;
ButtonsCommanderKeyboard pushFunction1;
ButtonsCommanderKeyboard pushFunction2;
#else
ButtonsCommanderPush buttonDir;
ButtonsCommanderEncoder buttonEncoder;
ButtonsCommanderPush buttonMode;
ButtonsCommanderPush buttonPanic;
ButtonsCommanderSwitch buttonDcDcc;
ButtonsCommanderSwitch buttonF1;
ButtonsCommanderSwitch buttonF2;
#endif

#define BUTTON_DIR          0
#define BUTTON_SPEED        1
#define BUTTON_MODE        	2
#define BUTTON_PANIC        3
#define BUTTON_DCDCC        4
#define BUTTON_F1        	5	// Also for slow mode in Dc .
#define BUTTON_F2        	6

void setup()
{
	Serial.begin(115200);

	DcDccControler::begin(11, 9);    // Dc: Pwm, Dir / Dcc : Pwm, unused

#ifdef VISUALSTUDIO
	push0.begin(LCD1_EVENT_SELECT, '*');
	push1.begin(LCD1_EVENT_MORE, '+');
	push2.begin(LCD1_EVENT_LESS, '-');
	push3.begin(LCD1_EVENT_CANCEL, '/');
	pushDcDcc.begin(EVENT_DCDCC, '.');
	pushEmergency.begin(EVENT_EMERGENCY, '0');
	pushFunction1.begin(LCD1_EVENT_FUNCTION1, '1');
	pushFunction2.begin(LCD1_EVENT_FUNCTION2, '2');
#else
	DcDccControler::AddHandle(&handle);
	buttonDir.begin(BUTTON_DIR, A0);
	buttonEncoder.begin(BUTTON_SPEED, 8, 12);
	buttonMode.begin(BUTTON_MODE, A3);
	buttonPanic.begin(BUTTON_PANIC, A4);
	buttonDcDcc.begin();
	buttonF1.begin();
	buttonF2.begin();

	buttonDcDcc.AddEvent(BUTTON_DCDCC, A5);
	buttonF1.AddEvent(BUTTON_F1, A1);
	buttonF2.AddEvent(BUTTON_F2, A2);
#endif
	handle.begin();

	screen.begin(20, 4, string_table, &lcd);
	//screen.begin(16, 2, string_table, &lcd);
	handle.GetUI()->begin(&screen);

	DcDccControler::AddHandle(&handle);       
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

