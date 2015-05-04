/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a function choice window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseFunction.hpp"

Function *WindowChooseFunction::GetNextSlot()
{
	for (int count = 0; this->pHandle->GetEditedLocomotive().GetFunctionNumber(); count++)
		if (this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(count) == this->pSelected)
		{
			if (count < this->pHandle->GetEditedLocomotive().GetFunctionNumber() - 1)
				return this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(count + 1);
			return this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(0);
		}

	return 0;
}

Function *WindowChooseFunction::GetPrevSlot()
{
	for (int count = 0; this->pHandle->GetEditedLocomotive().GetFunctionNumber(); count++)
		if (this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(count) == this->pSelected)
		{
			if (count == 0)
				return this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(this->pHandle->GetEditedLocomotive().GetFunctionNumber() - 1);
			return this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(count - 1);
		}

	return 0;
}

WindowChooseFunction::WindowChooseFunction(int inFirstLine, Handle *inpHandle) : Window(inFirstLine)
{
	this->pHandle = inpHandle;
}

void WindowChooseFunction::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;
	Screen *pScreen = inpLcd->GetScreen();

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		this->pSelected = this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(0);
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		pScreen->DisplayHeader(this->firstLine);
		this->state = STATE_INITIALIZE;
		if (this->pHandle->GetEditedLocomotive().GetFunctionNumber() == 1)
		{
			this->pSelected = this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(0);
			this->SetState(STATE_CONFIRMED);
		}
	}

	switch (inEventType)
	{
	case EVENT_MORE:
		this->pSelected = GetNextSlot();
		showValue = true;
		break;
	case EVENT_LESS:
		this->pSelected = GetPrevSlot();
		showValue = true;
		break;
	case EVENT_MOVE:
		break;
	case EVENT_SELECT:
		this->SetState(STATE_CONFIRMED);
		break;
	case EVENT_CANCEL:
		this->SetState(STATE_ABORTED);
		break;
	}

	if (showValue)
	{
		if (this->pSelected == 0)
			return;

		pScreen->DisplayChoice(this->pSelected->GetName());
	}
}

