/*************************************************************
project: <Dc Dcc Controler>
author: <Thierry PARIS>
description: <DDC UI demo>
*************************************************************/

#include "English16.hpp"			// Must be BEFORE define DcDccControler.h

#include "DcDccControler.h"
#include "ButtonsCommanderKeyboard.hpp"

ButtonsCommander buttons;

ScreenTwoLines Screen;
Locomotive *pLocoDummy, *pLocoDummy2;

#define BUTTON_SPEED_MORE	0
#define BUTTON_SPEED_LESS	1
#define BUTTON_DCDCC		2
#define BUTTON_DIR			3
#define BUTTON_PANIC		4
#define BUTTON_F1			5
#define BUTTON_F2			6
#define BUTTON_MODE			7

Handle handle(1);

void setup()
{								
	DDC.StartSetup();

	pLocoDummy = new Locomotive(3, DCC_SHORT_ADDRESS, "Picasso2732", 128);
	pLocoDummy->Setup(2);
	pLocoDummy->AddFunction(new Function(1, "Front", FrontLight));
	pLocoDummy->AddFunction(new Function(2, "Rear", RearLight));

	buttons.Setup(8,
		new ButtonsCommanderKeyboard(),	 // speed more
		new ButtonsCommanderKeyboard(),	 // speed less
		new ButtonsCommanderKeyboard(),  // dc-c
		new ButtonsCommanderKeyboard(),  // select / dir
		new ButtonsCommanderKeyboard(),	 // PANIC
		new ButtonsCommanderKeyboard(),	 // F1
		new ButtonsCommanderKeyboard(),	 // F2
		new ButtonsCommanderKeyboard()	 // MODE
		);

	KEYBOARD(buttons, BUTTON_SPEED_MORE)->Setup('+');
	KEYBOARD(buttons, BUTTON_SPEED_LESS)->Setup('-');
	KEYBOARD(buttons, BUTTON_DCDCC)->Setup('d');
	KEYBOARD(buttons, BUTTON_DIR)->Setup('*');
	KEYBOARD(buttons, BUTTON_PANIC)->Setup('p');
	KEYBOARD(buttons, BUTTON_F1)->Setup('1');
	KEYBOARD(buttons, BUTTON_F2)->Setup('2');
	KEYBOARD(buttons, BUTTON_MODE)->Setup('/');

	Screen.Setup(16, 2, string_table, 8, 10, 9, 4, 5, 6, 7);

	handle.pSpeedPushMore = KEYBOARD(buttons, BUTTON_SPEED_MORE);
	handle.pSpeedPushLess = KEYBOARD(buttons, BUTTON_SPEED_LESS);
	handle.pDirectionPush = KEYBOARD(buttons, BUTTON_DIR);
	handle.pModeButton = KEYBOARD(buttons, BUTTON_MODE);
	handle.Setup(2,
		new FunctionHandle(1, KEYBOARD(buttons, BUTTON_F1)),
		new FunctionHandle(2, KEYBOARD(buttons, BUTTON_F2))
		);
	handle.SetLocomotive(*pLocoDummy);
	handle.MoreLessIncrement = 10;
	handle.GetUI()->Setup(&Screen);

	DDC.AddHandle(&handle);
	DDC.SetDcDccButton(KEYBOARD(buttons, BUTTON_DCDCC));
	DDC.SetPanicButton(KEYBOARD(buttons, BUTTON_PANIC));

	DDC.EndSetup();

	// Must be done AFTER endsetup() !
/*
	pLocoDummy2 = new Locomotive(9990, DCC_LONG_ADDRESS, "SNCF030", 14);
	pLocoDummy2->Setup(2);
	pLocoDummy2->AddFunction(new Function(1, "Sound 5", Sound5));

	DCCItemList.AddLoco(pLocoDummy);
	DCCItemList.AddLoco(pLocoDummy2);

	DCCItemList.FreeLoco(pLocoDummy);
	DCCItemList.FreeLoco(pLocoDummy2);

	DCCItemList.AddLoco(pLocoDummy2);
	DCCItemList.AddLoco(pLocoDummy);

	pLocoDummy2->SetName("SCNF040");
	pLocoDummy2->AddFunction(new Function(2, "Sound 10", FunctionType::Sound10));

	DCCItemList.UpdateLoco(pLocoDummy2);

	DCCItemList.GetLoco(pLocoDummy2->GetSlotNumber(), pLocoDummy);

	DCCItemList.printList(5);
*/
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
