/*************************************************************
project: <LcdUI>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "LcdUi.h"
#include "WindowFunction.hpp"

WindowFunction::WindowFunction(byte inFirstLine, int inNumber) : WindowInt(inFirstLine, 10026, 1, inNumber)
{
	this->intValue = 0;
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

		if (this->intValue > this->GetMaxIntValue())
			this->intValue = this->GetMaxIntValue();
		if (this->intValue < this->GetMinIntValue())
			this->intValue = this->GetMinIntValue();

		this->state = STATE_INITIALIZE;
	}

	WindowInt::Event(inEventType, inpLcd);
}

