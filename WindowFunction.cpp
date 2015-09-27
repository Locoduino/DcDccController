/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowFunction.hpp"

WindowFunction::WindowFunction(byte inFirstLine, int inNumber) : WindowInt(inFirstLine, FUNCTION_MAX, FUNCTION_MIN, inNumber)
{
}

void WindowFunction::Event(byte inEventType, LcdUi *inpLcd)
{
	bool start = false;
	if (this->state == STATE_START)
		start = true;

	WindowInt::Event(inEventType, inpLcd);
	if (start)
	{
		inpLcd->GetScreen()->GetString(this->firstLine);

		inpLcd->GetScreen()->setCursor(strlen(Screen::buffer), 0);
		inpLcd->GetScreen()->BuildString(this->tag + 1, Screen::buffer);
		inpLcd->GetScreen()->print(Screen::buffer);
	}

}

