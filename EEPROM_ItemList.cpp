/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a full list of items of the same size stored in EEPROM>
*************************************************************/

#include "DcDccControler.h"
#include "EEPROM_ItemList.hpp"

/*

The item list area of the EEPROM is a long list of items of the same size.
Each item can be identified by a type.
Each item has its own structure, but is stored in a 'slot' of EEPROM_ItemList::SlotSize.
Each item slot begins with two bytes : the type, and the owner (255 if null)

The owner id is in reality the address of the slot of the owner !
*/

void EEPROM_ItemListClass::FreeItem(byte inSlotNumber)
{
	// Replace the type by '0' : free slot !
	EEPROM.write(GetItemPosRaw(inSlotNumber), 0);
	FreeOwnedItems(inSlotNumber);
}

void EEPROM_ItemListClass::FreeOwnedItems(byte inOwnerSlotNumber)
{
	int slot = 0;
	for (int pos = StartListPos; pos < EEPROM_SIZE; pos += ItemSize)
	{
		if (EEPROM.read(pos) == 0)
			continue;
		if (EEPROM.read(pos + 1) == inOwnerSlotNumber)
		{
			// Replace the type by '0' : free slot !
			EEPROM.write(pos, 0);
			// also remove the owned items, and their owned items in turn...
			FreeItem(slot);
		}
		slot++;
	}
}

byte EEPROM_ItemListClass::FindItem(byte inType, byte inStartSlotNumber, byte inOwnerId)
{
	byte slot = inStartSlotNumber;

	for (int pos = StartListPos + (inStartSlotNumber * ItemSize); pos < EEPROM_SIZE; pos += ItemSize)
	{
		if (EEPROM.read(pos) == inType)
			if (inOwnerId == 255 || EEPROM.read(pos + 1) == inOwnerId)
				return slot;
		slot++;
	}

	return 255;
}

byte EEPROM_ItemListClass::CountItems(byte inType)
{
	byte nb = 0;

	for (int pos = StartListPos; pos < EEPROM_SIZE; pos += ItemSize)
		if (EEPROM.read(pos) == inType)
			nb++;

	return nb;
}

byte EEPROM_ItemListClass::CountOwnedItems(byte inOwnerId)
{
	byte nb = 0;

	for (int pos = StartListPos; pos < EEPROM_SIZE; pos += ItemSize)
		if (EEPROM.read(pos) != 0 && EEPROM.read(pos + 1) == inOwnerId)
			nb++;

	return nb;
}

byte EEPROM_ItemListClass::GetFirstFreeSlot()
{
	byte slot = 0;

	for (int pos = StartListPos; pos < EEPROM_SIZE; pos += ItemSize)
	{
		if (EEPROM.read(pos) == 0)
			return slot;
		slot++;
	}

	return 255;
}

char *EEPROM_ItemListClass::GetItems()
{
	return 0;
}



