/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Item button from analog array of push buttons with debounce.>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderAnalogPushes.hpp"
#include "ButtonsCommanderAnalogPushesItem.hpp"

bool ButtonsCommanderAnalogPushesItem::IsPushed(int inCurrentValue)
{
	if (inCurrentValue >= this->analogValue - this->readingTolerancy && inCurrentValue <= this->analogValue + this->readingTolerancy)
		return true;

	return false;
}