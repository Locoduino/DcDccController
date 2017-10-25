/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for the Rolling Stock>
*************************************************************/

#ifndef NANOCONTROLER
#include "DcDccControler.h"
#include "RollingStock.hpp"

byte RollingStock::rollingStock[ROLLINGSTOCK_NUMBER];
Locomotive RollingStock::Current;

void RollingStock::begin()
{
	Clear();
}

void RollingStock::SetCurrent(byte inPosition)
{
}

byte RollingStock::FindFreeSlot()
{
	for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++)
		if (rollingStock[i] == 255)
			return i;

	return 255;
}

bool RollingStock::IsInStock(byte inLocoNumber)
{
	for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++)
		if (rollingStock[i] == inLocoNumber)
			return true;

	return false;
}

void RollingStock::AddLoco(byte inSlotPosition, byte inLocoNumber)
{
	rollingStock[inSlotPosition] = inLocoNumber;
}

void RollingStock::Remove(byte inPosition)
{
	rollingStock[inPosition] = 255;
}

int RollingStock::ConfigSave(int inPosition)
{
	for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++)
		EEPROM.write(inPosition + i, rollingStock[i]);

	return inPosition + ROLLINGSTOCK_NUMBER;
}

int RollingStock::ConfigLoad(int inPosition)
{
	for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++)
		rollingStock[i] = EEPROM.read(inPosition + i);

#ifdef DDC_DEBUG_MODE
	Serial.println(F("Rolling Stock: "));
	for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++)
	{
		Serial.print(rollingStock[i]);
		Serial.print(F(" "));
	}
	Serial.println(F(""));
#endif
	return inPosition + ROLLINGSTOCK_NUMBER;
}
#endif