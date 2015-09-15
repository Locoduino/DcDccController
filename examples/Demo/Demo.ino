/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Tiny Dc/Dcc controler sample>
*************************************************************/

#include "French16.hpp"

#include <LcdUi.h>
#include "EEPROMextent.h"
#include "DcDccControler.h"

ButtonsCommander buttons;
ScreenTwoLines Screen;    
Handle handle;

#define BUTTON_DIR              0
#define BUTTON_SPEED            1
#define BUTTON_MODE        	2
#define BUTTON_PANIC        	3
#define BUTTON_DCDCC        	4
#define BUTTON_F1        	5	// Also for slow mode in Dc .
#define BUTTON_F2        	6

void setup()
{
    Serial.begin(115200);

	DDC.StartSetup(11, 9);    // Dc: Pwm, Dir / Dcc : Pwm, unused

	buttons.Setup(7,
		new ButtonsCommanderPush(),
		new ButtonsCommanderEncoder(),
		new ButtonsCommanderPush(),
		new ButtonsCommanderPush(),
		new ButtonsCommanderSwitch(),
		new ButtonsCommanderSwitch(),
		new ButtonsCommanderSwitch()
		);

        PUSH(buttons, BUTTON_DIR)->Setup(A0);
        ENCODER(buttons, BUTTON_SPEED)->Setup(8, 12);
        PUSH(buttons, BUTTON_MODE)->Setup(A3);
        PUSH(buttons, BUTTON_PANIC)->Setup(A4);
        SWITCH(buttons, BUTTON_DCDCC)->Setup(A5);
        SWITCH(buttons, BUTTON_F1)->Setup(A1);
        SWITCH(buttons, BUTTON_F2)->Setup(A2);
        
	handle.pSpeedEncoder = ENCODER(buttons, BUTTON_SPEED);
	handle.pDirectionPush = PUSH(buttons, BUTTON_DIR);
	handle.pCancelButton = PUSH(buttons, BUTTON_MODE);

    handle.Setup(2);	// Two function buttons
    handle.AddFunction(new FunctionHandle(1, PUSH(buttons, BUTTON_F1)));
    handle.AddFunction(new FunctionHandle(2, PUSH(buttons, BUTTON_F2)));
      
	Screen.Setup(16, 2, string_table, 7, -1, 6, 5, 4, 3, 2);
	handle.GetUI()->Setup(&Screen);

	DDC.AddHandle(&handle);
	DDC.SetDcDccButton(SWITCH(buttons, BUTTON_DCDCC));
	DDC.SetPanicButton(PUSH(buttons, BUTTON_PANIC));

	DDC.EndSetup();
}

void loop()
{
    buttons.Loop();
    DDC.Loop();
}

