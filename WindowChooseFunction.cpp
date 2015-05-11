/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a function choice window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseFunction.hpp"

byte WindowChooseFunction::GetSelectedSlotIndex()
{
	for (int count = 0; this->pHandle->GetEditedLocomotive().GetFunctionNumber(); count++)
		if (this->pHandle->GetEditedLocomotive().GetFunctionFromIndex(count) == this->pSelected)
			return count;

	return 0;
}

Function *WindowChooseFunction::GetNextSlot()
{
	byte slot = GetSelectedSlotIndex();
	const Locomotive &loco = this->pHandle->GetEditedLocomotive();

	if (slot < loco.GetFunctionNumber() - 1)
		return loco.GetFunctionFromIndex(slot + 1);
	return loco.GetFunctionFromIndex(0);
}

Function *WindowChooseFunction::GetPrevSlot()
{
	byte slot = GetSelectedSlotIndex();
	const Locomotive &loco = this->pHandle->GetEditedLocomotive();

	if (slot == 0)
		return loco.GetFunctionFromIndex(loco.GetFunctionNumber() - 1);
	return loco.GetFunctionFromIndex(slot - 1);
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

