/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for the Engine Shed>
*************************************************************/

#ifndef NANOCONTROLER
#include "DcDccControler.h"
#include "EngineShed.hpp"

// Build the list of slots for the choice :
// 0 1 3 5 10 128 255 255 255...
// There is no hole in the list. From the first255 to the end of the list, there is only 255.
// This list must be rebuilt after each modification of the engine shed listing.

byte EngineShed::LocoSlots[LOCO_MAXNUMBER];
Locomotive EngineShed::Current;
char EngineShed::name[DCC_LOCONAME_LENGTH];

void EngineShed::begin()
{
	Clear();

	DCCItemList.begin(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * DcDccControler::handleAddcounter), 30, EEPROM.length() - (EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * DcDccControler::handleAddcounter)));
	EngineShed::RebuildLocos();

//	DCCItemList.printList(50);
}

void EngineShed::RebuildLocos()
{
	byte count = DCCItemList.CountItems(LOCOMOTIVE_TYPE);

	byte slot = 0;
	int add = 0;

	while (slot != 255)
	{
		slot = DCCItemList.FindItem(LOCOMOTIVE_TYPE, slot);
		if (slot == 255)
			break;
		EngineShed::LocoSlots[add++] = slot;

		slot++;
	}

	for (int i = add; i < LOCO_MAXNUMBER; i++)
		EngineShed::LocoSlots[i] = 255;

#ifdef DDC_DEBUG_MODE
	Serial.print(count);
	Serial.println(F(" Locomotives found"));
	for (int i = 0; i < count; i++)
	{
		Serial.print(EngineShed::LocoSlots[i]);
		Serial.print(F(" "));
	}
	Serial.println(F(""));
#endif
}

void EngineShed::SetCurrent(byte inSlotNumber)
{
	DCCItemList.GetLoco(inSlotNumber, &Current);
}

byte EngineShed::AddCurrentLoco()
{
	return DCCItemList.AddLoco(&Current);
}

void EngineShed::Remove(byte inSlotNumber)
{
	DCCItemList.FreeLoco(inSlotNumber);
}

char *EngineShed::GetName(byte inLocoNumber)
{
	Locomotive::LoadName(DCCItemList.GetItemPos(EngineShed::LocoSlots[inLocoNumber]), EngineShed::name);
	return EngineShed::name;
}

#endif