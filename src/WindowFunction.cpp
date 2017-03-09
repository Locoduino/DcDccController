/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowFunction.hpp"

void WindowFunction::Event(byte inEventType, LcdUi *inpLcd)
{
	bool start = false;
	if (this->state == STATE_START)
		start = true;

	WindowInt::Event(inEventType, inpLcd);
	if (start)
	{
		inpLcd->GetScreen()->GetString(this->firstLine);
		int len = strlen(LcdScreen::buffer);
		inpLcd->GetScreen()->BuildString(this->tag + 1, LcdScreen::buffer);
		inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, len, 0);
	}

}

