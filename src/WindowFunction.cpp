/*************************************************************
project: <LcdUi>
author: <Thierry PARIS>
description: <Class for a basic screen>
*************************************************************/

#include "DcDccControler.h"
#include "WindowFunction.hpp"

void WindowFunction::begin(byte inFirstLine, int *inpValue)
{ 
	this->WindowInt::begin(inFirstLine, inpValue, FUNCTION_MIN, FUNCTION_MAX); 
}

void WindowFunction::Event(byte inEventType, LcdUi *inpLcd)
{
	if (this->state == STATE_START)
	{
		inpLcd->GetScreen()->clear();
		inpLcd->GetScreen()->GetString(STR_FUNCTION);
		inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, 0, 0);
		int len = strlen(LcdScreen::buffer);
		inpLcd->GetScreen()->BuildString(this->GetWindowId() - STR_FUNCTIONID1, LcdScreen::buffer);
		inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, len+1, 0);
		this->state = STATE_INITIALIZE;
		return;
	}

	WindowInt::Event(inEventType, inpLcd);
}

#ifdef LCDUI_PRINT_WINDOWS
void WindowFunction::printWindow()
{
	printWindowHeader(F("Window Function"));
	Serial.print(F(" / F"));
	Serial.println(this->GetWindowId() - STR_FUNCTIONID1);
}
#endif