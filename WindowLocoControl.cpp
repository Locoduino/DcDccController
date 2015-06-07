/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco control window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl(int inFirstLine, Handle *inpHandle) : Window(inFirstLine)
{
	this->pHandle = inpHandle;
	this->dcMsg = STR_DC;
	this->dccMsg = STR_DCC;
}

#define DEBUG_MODE

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		if (DDC.dcType == Dc)
			inpLcd->GetScreen()->DisplayHeader(this->dcMsg);
		else
		{
			inpLcd->GetScreen()->DisplayHeader(this->dccMsg);
			inpLcd->GetScreen()->setCursor(3, 0);
			inpLcd->GetScreen()->write(32);
			inpLcd->GetScreen()->setCursor(4, 0);
			Screen::BuildString(this->pHandle->GetLocomotive().GetDccId(), Screen::buffer, this->pHandle->DccIdNbDigits);
			inpLcd->GetScreen()->print(Screen::buffer);
			inpLcd->GetScreen()->setCursor(4 + this->pHandle->DccIdNbDigits, 0);
			inpLcd->GetScreen()->write(32);
			byte len = Screen::BuildStringLeft(this->pHandle->GetLocomotive().GetName(), inpLcd->GetScreen()->GetSizeX() - (4 + this->pHandle->DccIdNbDigits + 1), Screen::buffer);
			inpLcd->GetScreen()->setCursor(inpLcd->GetScreen()->GetSizeX() - len, 0);
			inpLcd->GetScreen()->print(Screen::buffer);
		}

		this->state = STATE_INITIALIZE;
	}

	byte steps = this->pHandle->GetLocomotive().GetSteps();
	byte inc;
#ifdef DEBUG_MODE
	Serial.println(steps);
#endif
	switch (steps)
	{
		case 128: inc = this->pHandle->MoreLessIncrement; break;
		default: inc = 1; break;
	}

	switch (inEventType)
	{
		case EVENT_MORE:
			{
#ifdef DEBUG_MODE
			Serial.print(F("MORE "));
#endif
			unsigned int newValue = this->pHandle->Speed + inc;
			if (newValue > steps)
				newValue = steps;
			this->pHandle->SetSpeed(newValue);
#ifdef DEBUG_MODE
			Serial.println(newValue);
#endif
		}
			showValue = true;
			break;

		case EVENT_LESS:
			{
#ifdef DEBUG_MODE
			Serial.print(F("LESS "));
#endif
			int newValue = this->pHandle->Speed - inc;
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
#ifdef DEBUG_MODE
			Serial.println(newValue);
#endif
		}
			showValue = true;
			break;

		case EVENT_MOVE:
			break;
		case EVENT_SELECT:
#ifdef DEBUG_MODE
			Serial.println(F("SELECT"));
#endif
			this->pHandle->SetDirection(!this->pHandle->DirectionToLeft);
			showValue = true;
			break;
		case EVENT_CANCEL:
#ifdef DEBUG_MODE
			Serial.println(F("CANCEL"));
#endif
			this->state = STATE_ABORTED;
			break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 003 nomloco11
		// 1 +>>>>>			 -
		//   01234567879012345
		Screen::BuildProgress(this->pHandle->Speed, this->pHandle->GetLocomotive().GetSteps(), this->pHandle->DirectionToLeft, inpLcd->GetScreen()->GetSizeX(), Screen::buffer);
		inpLcd->GetScreen()->setCursor(0, 1);
		inpLcd->GetScreen()->print(Screen::buffer);
	}
}

