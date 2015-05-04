/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco choice window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"

byte *WindowChooseLoco::pLocoSlots;
byte WindowChooseLoco::selectedSlot;

void WindowChooseLoco::RebuildChoices()
{
	if (WindowChooseLoco::pLocoSlots != 0)
	{
		delete[] WindowChooseLoco::pLocoSlots;
		WindowChooseLoco::pLocoSlots = 0;
	}

	byte count = DCCItemList.CountItems(LOCOMOTIVE_TYPE);

	WindowChooseLoco::pLocoSlots = new byte[count + 1];

	byte slot = 0;
	int add = 0;

	while (slot != 255)
	{
		slot = DCCItemList.FindItem(LOCOMOTIVE_TYPE, slot);
		if (slot == 255)
			break;
		WindowChooseLoco::pLocoSlots[add++] = slot;

		if (WindowChooseLoco::selectedSlot == 255)
			WindowChooseLoco::selectedSlot = slot;

		slot++;
	}

	for (int i = add; i < count + 1; i++)
		WindowChooseLoco::pLocoSlots[i] = 255;

#ifdef DEBUG_MODE
	Serial.print(count);
	Serial.println(F(" Locomotives found"));
#endif
}

byte WindowChooseLoco::GetNextLocoSlot(byte slot)
{
	for (int count = 0;  WindowChooseLoco::pLocoSlots[count] != 255; count++)
		if (WindowChooseLoco::pLocoSlots[count] == slot)
		{
			count++;
			if (WindowChooseLoco::pLocoSlots[count] != 255)
				return WindowChooseLoco::pLocoSlots[count];
			return WindowChooseLoco::pLocoSlots[0];
		}

	return 255;
}

byte WindowChooseLoco::GetPrevLocoSlot(byte slot)
{
	for (int count = 0; WindowChooseLoco::pLocoSlots[count] != 255; count++)
		if (WindowChooseLoco::pLocoSlots[count] == slot)
		{
			if (count == 0)
			{
				// if start of the list, go to the end
				while (WindowChooseLoco::pLocoSlots[count] != 255)
					count++;

				if (count == 0)
					return 255;
			}
			return WindowChooseLoco::pLocoSlots[count - 1];
		}

	return 255;
}

WindowChooseLoco::WindowChooseLoco(int inFirstLine, Handle *inpHandle) : Window(inFirstLine)
{
	this->pHandle = inpHandle;
}

void WindowChooseLoco::Event(byte inEventType, LcdUi *inpLcd)
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
		pScreen->DisplayHeader(this->firstLine);
		this->state = STATE_INITIALIZE;
		if (WindowChooseLoco::pLocoSlots[0] != 255 && WindowChooseLoco::pLocoSlots[1] == 255)
		{
			WindowChooseLoco::selectedSlot = WindowChooseLoco::pLocoSlots[0];
			this->SetState(STATE_CONFIRMED);
		}
	}

	switch (inEventType)
	{
	case EVENT_MORE:
		WindowChooseLoco::selectedSlot = WindowChooseLoco::GetNextLocoSlot(WindowChooseLoco::selectedSlot);
		showValue = true;
		break;
	case EVENT_LESS:
		WindowChooseLoco::selectedSlot = WindowChooseLoco::GetPrevLocoSlot(WindowChooseLoco::selectedSlot);
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
		if (WindowChooseLoco::selectedSlot == 255)
			return;
		char name[12];
		Locomotive::LoadName(DCCItemList.GetItemPos(WindowChooseLoco::selectedSlot), name);

		pScreen->DisplayChoice(name);
	}
}

