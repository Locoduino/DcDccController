/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a full list of DCC items>
*************************************************************/

#include "DcDccControler.h"
#include "arduino.h"
#include "EEPROM.h"

/*
Each item is defined by a byte giving a type 'L', 'F' or 'A', followed by the owner of the item or 255. 
Each item has also a Dcc id on two bytes, and a name as a string.
For a Loco, there is a byte for the address kind (long or not), and the steps numbers.
For a function, there is a type as a byte, and an integer giving the id of owner loco.
For an accessory, there is a byte for a type, and another one for the delay.

The loco id is in reality the address of the starting byte of the loco in the EEPROM !

Loco:		L   ID     NAME 	   STEPS
			 OWNER			LONGADDRESS 
			_ _ __ ____________ _ _

Function	F   ID     NAME	  TYPE
			 OWNER
			_ _ __ ____________ _

Accessory	A   ID     NAME	   DELAY
			 OWNER			  TYPE
			_ _ __ ____________ _ _

0: L0
1: F1 (0)
2: F2 (0)
3: L1
4: F1 (3)
5: F2 (3)

*/

DCCItemListClass DCCItemListClass::DCCItemListInstance;

void DCCItemListClass::AddLoco(Locomotive *outpNewLoco)
{
	byte slotLoco = GetFirstFreeSlot();

	// no more place !
	if (slotLoco == 255)
		return;

	int pos = SaveItemPrefix(slotLoco, LOCOMOTIVE_TYPE, EEPROMLIST_EMPTY_OWNER);
	outpNewLoco->Save(pos);

	for (int i = 0; i < FUNCTION_NUMBER; i++)
	{
		byte slot = GetFirstFreeSlot();

		// no more place !
		if (slotLoco == 255)
			return;

		int pos = SaveItemPrefix(slot, FUNCTION_TYPE, slotLoco);
		outpNewLoco->Functions[i].Save(pos);
		outpNewLoco->Functions[i].SetSlotNumber(slot);
	}
	outpNewLoco->SetSlotNumber(slotLoco);

#ifdef DDC_DEBUG_MODE
	Serial.println(F("Loco created !"));
#endif
}

void DCCItemListClass::FreeLoco(Locomotive *outpLoco)
{
	byte slotLoco = outpLoco->GetSlotNumber();

	// not yet saved !
	if (slotLoco == 255)
		return;

	FreeItem(slotLoco);

#ifdef DDC_DEBUG_MODE
	Serial.println(F("Loco Deleted !"));
#endif
}

void DCCItemListClass::UpdateLoco(Locomotive *outpLoco)
{
	byte slotLoco = outpLoco->GetSlotNumber();

	// not yet saved !
	if (slotLoco == 255)
		return;

	int pos = SaveItemPrefix(slotLoco, LOCOMOTIVE_TYPE, EEPROMLIST_EMPTY_OWNER);
	outpLoco->Save(pos);

	// Remove old functions
	FreeOwnedItems(slotLoco);

	// Then Save new functions !
	for (int i = 0; i < FUNCTION_NUMBER; i++)
	{
		byte slot = GetFirstFreeSlot();

		int pos = SaveItemPrefix(slot, FUNCTION_TYPE, slotLoco);
		outpLoco->Functions[i].Save(pos);
		outpLoco->Functions[i].SetSlotNumber(slot);
	}

#ifdef DDC_DEBUG_MODE
	Serial.println(F("Loco updated !"));
#endif
}

void DCCItemListClass::GetLoco(byte inSlotNumber, Locomotive *outpLoco)
{
	outpLoco->Load(GetItemPos(inSlotNumber));
	outpLoco->SetSlotNumber(inSlotNumber);

	byte count = this->CountOwnedItems(inSlotNumber);
	byte slotCurr = 0;

	outpLoco->ClearFunctions();

	// Load new functions !
	int iFunction = 0;
	for ( ; iFunction < count && iFunction < FUNCTION_NUMBER; iFunction++)
	{
		slotCurr = FindItem(FUNCTION_TYPE, slotCurr, inSlotNumber);
		Function *f = new Function();
		f->Load(GetItemPos(slotCurr));
		outpLoco->Functions[iFunction].Copy(*f);
		slotCurr++;
	}
}

#ifdef DDC_DEBUG_MODE
void DCCItemListClass::printList(byte inNumberMax)
{
	int pos = StartListPos;
	byte found = 0;

	for (; pos < EEPROM.length(); pos += ItemSize)
	{
		if (GetSlotFromPos(pos) > inNumberMax)
			break;

		Serial.print(GetSlotFromPos(pos));
		byte type = EEPROMextent.read(pos);
		Serial.print(F(" :  type="));
		Serial.print(type);
		if (type == 0)
		{
			Serial.println(F("  free slot"));
			continue;
		}
		byte owner = EEPROMextent.read(pos + 1);
		Serial.print(F(" Owner="));
		Serial.print(owner);
		int dccId;
		int size = EEPROMextent.readAnything(pos+2, dccId);
		Serial.print(F(" DccId="));
		Serial.print(dccId);
		char name[12];
		EEPROMextent.readString(pos+2+size, name, 12);
		Serial.print(F(" Name='"));
		Serial.print(name);
		Serial.println(F("'"));
	}
}
#endif
