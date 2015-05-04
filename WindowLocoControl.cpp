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

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		if (DDC.dcType == Dc)
			pScreen->DisplayHeader(this->dcMsg);
		else
		{
			pScreen->DisplayHeader(this->dccMsg);
			pScreen->setCursor(3, 0);
			pScreen->write(32);
			pScreen->setCursor(4, 0);
			Screen::BuildString(this->pHandle->GetLocomotive().GetDccId(), Screen::buffer, this->pHandle->DccIdNbDigits);
			pScreen->print(Screen::buffer);
			pScreen->setCursor(4 + this->pHandle->DccIdNbDigits, 0);
			pScreen->write(32);
			byte len = Screen::BuildStringLeft(this->pHandle->GetLocomotive().GetName(), pScreen->GetSizeX() - (4 + this->pHandle->DccIdNbDigits + 1), Screen::buffer);
			pScreen->setCursor(pScreen->GetSizeX() - len, 0);
			pScreen->print(Screen::buffer);
		}

		this->state = STATE_INITIALIZE;
	}

	switch (inEventType)
	{
		case EVENT_MORE:
			{
			unsigned int newValue = this->pHandle->Speed + this->pHandle->MoreLessIncrement;
			if (newValue > this->pHandle->GetLocomotive().GetSteps())
				newValue = this->pHandle->GetLocomotive().GetSteps();
			this->pHandle->SetSpeed(newValue);
			}
			showValue = true;
			break;

		case EVENT_LESS:
			{
			int newValue = this->pHandle->Speed - this->pHandle->MoreLessIncrement;
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
			}
			showValue = true;
			break;

		case EVENT_MOVE:
			break;
		case EVENT_SELECT:
			this->pHandle->SetDirection(!this->pHandle->DirectionToLeft);
			showValue = true;
			break;
		case EVENT_CANCEL:
			this->state = STATE_ABORTED;
			break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 003 nomloco11
		// 1 +>>>>>			 -
		//   01234567879012345
		Screen::BuildProgress(this->pHandle->Speed, this->pHandle->GetLocomotive().GetSteps(), this->pHandle->DirectionToLeft, pScreen->GetSizeX(), Screen::buffer);
		pScreen->setCursor(0, 1);
		pScreen->print(Screen::buffer);
	}
}

