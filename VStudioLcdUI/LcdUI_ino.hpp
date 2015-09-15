/*************************************************************
project: <Dc Dcc Controler>
author: <Thierry PARIS>
description: <DDC UI demo>
*************************************************************/

#include "French16.hpp"			// Must be BEFORE define DcDccControler.h

#include <LcdUi.h>
#include <EEPROMextent.h>
#include "DcDccControler.h"
#include "ButtonsCommanderKeyboard.hpp"

ButtonsCommander buttons;
ScreenTwoLines Screen;

#define BUTTON_SPEED_MORE	0
#define BUTTON_SPEED_LESS	1
#define BUTTON_DCDCC		2
#define BUTTON_DIR			3
#define BUTTON_PANIC		4
#define BUTTON_F1			5
#define BUTTON_F2			6
#define BUTTON_MODE			7

Handle handle;

void setup()
{								
	DDC.StartSetup(10, 9);

	buttons.Setup(8,
		new ButtonsCommanderKeyboard(),	 // BUTTON_SPEED_MORE	
		new ButtonsCommanderKeyboard(),	 // BUTTON_SPEED_LESS	
		new ButtonsCommanderKeyboard(),  // BUTTON_DCDCC		
		new ButtonsCommanderKeyboard(),  // BUTTON_DIR			
		new ButtonsCommanderKeyboard(),	 // BUTTON_PANIC		
		new ButtonsCommanderKeyboard(),	 // BUTTON_F1			
		new ButtonsCommanderKeyboard(),	 // BUTTON_F2			
		new ButtonsCommanderKeyboard()	 // BUTTON_MODE			
		);

	KEYBOARD(buttons, BUTTON_SPEED_MORE)->Setup('+');
	KEYBOARD(buttons, BUTTON_SPEED_LESS)->Setup('-');
	KEYBOARD(buttons, BUTTON_DCDCC)->Setup('d');
	KEYBOARD(buttons, BUTTON_DIR)->Setup('*');
	KEYBOARD(buttons, BUTTON_PANIC)->Setup('p');
	KEYBOARD(buttons, BUTTON_F1)->Setup('1');
	KEYBOARD(buttons, BUTTON_F2)->Setup('2');
	KEYBOARD(buttons, BUTTON_MODE)->Setup('/');

	handle.pSpeedPushMore = KEYBOARD(buttons, BUTTON_SPEED_MORE);
	handle.pSpeedPushLess = KEYBOARD(buttons, BUTTON_SPEED_LESS);
	handle.pDirectionPush = KEYBOARD(buttons, BUTTON_DIR);
	handle.pCancelButton = KEYBOARD(buttons, BUTTON_MODE);
	handle.Setup(2);
	handle.AddFunction(new FunctionHandle(1, PUSH(buttons, BUTTON_F1)));
	handle.AddFunction(new FunctionHandle(2, PUSH(buttons, BUTTON_F2)));
	
	Screen.Setup(16, 2, string_table, 8, 10, 9, 4, 5, 6, 7);
	handle.GetUI()->Setup(&Screen);

	handle.MoreLessIncrement = 10;

	DDC.AddHandle(&handle);
	DDC.SetDcDccButton(KEYBOARD(buttons, BUTTON_DCDCC));
	DDC.SetPanicButton(KEYBOARD(buttons, BUTTON_PANIC));

	DDC.EndSetup();
}

void loop()
{
#ifdef VISUALSTUDIO
	if (theApp.lastKeyPressed != 0)
		ButtonsCommanderKeyboard::lastLoopKey = theApp.lastKeyPressed;
	theApp.lastKeyPressed = 0;
#endif
	buttons.Loop();
	DDC.Loop();
}
