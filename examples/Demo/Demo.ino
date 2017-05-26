/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Tiny Dc/Dcc controler sample>
*************************************************************/

#include "French16.hpp"
#include "LcdUi.h"

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
ButtonsCommanderKeyboard pushEmergency;
ButtonsCommanderKeyboard pushFunction1;
ButtonsCommanderKeyboard pushFunction2;
#endif
ButtonsCommanderPush buttonDir;
ButtonsCommanderEncoder buttonEncoder;
ButtonsCommanderPush buttonMode;
ButtonsCommanderPush buttonPanic;
ButtonsCommanderSwitchOnePin buttonF0;
ButtonsCommanderSwitchOnePin buttonF1;

void setup()
{
	Serial.begin(115200);

#ifdef VISUALSTUDIO
	push0.begin(LCD1_EVENT_SELECT, '*');
	push1.begin(LCD1_EVENT_MORE, '+');
	push2.begin(LCD1_EVENT_LESS, '-');
	push3.begin(LCD1_EVENT_CANCEL, '/');
	pushEmergency.begin(EVENT_EMERGENCY, '0');
	pushFunction1.begin(LCD1_EVENT_FUNCTION0, '1', COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	pushFunction1.AddEvent(LCD1_EVENT_FUNCTION0, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
	pushFunction2.begin(LCD1_EVENT_FUNCTION1, '2', COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_ON);
	pushFunction2.AddEvent(LCD1_EVENT_FUNCTION1, COMMANDERS_EVENT_MOVE, COMMANDERS_MOVE_OFF);
	DcDccControler::begin(255);
	DcDccControler::beginMain(255, 255, 11, 255);
#endif
	buttonDir.begin(LCD1_EVENT_SELECT, A0);
	buttonEncoder.begin(LCD1_EVENT_ENCODER, 12, 8, 2);
	buttonMode.begin(LCD1_EVENT_CANCEL, A3);
	buttonPanic.begin(EVENT_EMERGENCY, A4);
	buttonF0.begin(LCD1_EVENT_FUNCTION0, A1);
	buttonF1.begin(LCD1_EVENT_FUNCTION1, A2);

#ifdef VISUALSTUDIO
	pinName(A0, "OK");
	pinName(A1, "F0");
	pinName(A2, "F1");
	pinName(A3, "CANC");
	pinName(A4, "STOP");
#endif

	// if dcdcc pin equals to 255, dcc mode is forced.
	// if dcdcc pin equals to 0, dc mode is forced.
	// otherwise, pin state give dc or dcc.
	DcDccControler::begin(255);
	DcDccControler::beginMain(255, 255, 11, A6);    // Dc: Dir, Pwm, current sensor

	handle.begin();
	//screen.begin(20, 4, string_table, &lcd);
	screen.begin(16, 2, string_table, &lcd);
	handle.GetUI()->begin(&screen);
	DcDccControler::AddHandle(&handle);
}

void loop()
{
	unsigned long eventId = Commanders::loop();

	// For LcdUi, UNDEFINED_ID of Commanders has no meaning. And because it is necessary 
	// to execute lcdui.event() at each call of the main loop, 
	// do it with an empty event : EVENT_NONE.

	if (eventId == UNDEFINED_ID)
		eventId = EVENT_NONE;

	DcDccControler::loop(eventId, Commanders::GetLastEventData());
}

