/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a slot choice window>
*************************************************************/

#ifndef NANOCONTROLER
#include "DcDccControler.h"
#include "WindowChooseSlot.hpp"

char *WindowChooseSlot::GetChoiceTextValue(byte indexValue, LcdScreen *apScreen) const
{
	if (RollingStock::rollingStock[indexValue] == 255)
	{											 
		strcpy(EngineShed::name, apScreen->GetString(STR_EMPTY));
		sprintf(LcdScreen::buffer, " %d", indexValue);
		strcat(EngineShed::name, LcdScreen::buffer);
	}
	else
		EngineShed::GetName(RollingStock::rollingStock[indexValue]);

	return EngineShed::name;
}

void WindowChooseSlot::Event(byte inEventType, LcdUi *inpLcd)
{
	unsigned int lastSelected = *this->pValue;

	WindowChoiceText::Event(inEventType, inpLcd);

	if ((this->state == STATE_NONE || this->state == STATE_INITIALIZE) && lastSelected != 255)
	{
		bool redo, redone = false;

		this->lockScreen = true;

		if (this->state == STATE_INITIALIZE)
			inEventType = EVENT_MORE;
		do {
			redo = false;
			switch (this->canSelect)
			{
			case AllStock:
				return;
			case FreeRollingStockSlot:
				if (RollingStock::rollingStock[*this->pValue] != 255)
					redo = true;
				break;
			case UsedRollingStockSlot:
				if (RollingStock::rollingStock[*this->pValue] == 255)
					redo = true;
				break;
			}
			if (redo)
			{
				redone = true;
				WindowChoiceText::Event(inEventType, inpLcd);
			}
		} while (redo && lastSelected != *this->pValue);

		this->lockScreen = false;
		if (redone)
		{
			this->state = STATE_INITIALIZE;
			WindowChoiceText::Event(EVENT_NONE, inpLcd);
		}
	}
}


#endif