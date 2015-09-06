/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowFunction.hpp"

WindowFunction::WindowFunction(byte inFirstLine, int inNumber) : WindowInt(inFirstLine, inNumber)
{
	this->intValue = 0;
	this->minIntValue = 0;
	this->maxIntValue = 0;
}

void WindowFunction::Event(byte inEventType, LcdUi *inpLcd)
{
	if (this->state == STATE_START)
	{
		inpLcd->GetScreen()->clear();

		inpLcd->GetScreen()->setCursor(0, 0);
		inpLcd->GetScreen()->print(inpLcd->GetScreen()->GetString(this->firstLine));

		inpLcd->GetScreen()->setCursor(strlen(Screen::buffer) + 1, 0);
		inpLcd->GetScreen()->BuildString(this->tag + 1, Screen::buffer);
		inpLcd->GetScreen()->print(Screen::buffer);

		if (this->intValue > FUNCTION_MAX)
			this->intValue = FUNCTION_MAX;
		if (this->intValue < FUNCTION_MIN)
			this->intValue = FUNCTION_MIN;

		this->state = STATE_INITIALIZE;
	}

	WindowInt::Event(inEventType, inpLcd);
}

