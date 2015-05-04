//-------------------------------------------------------------------
#ifndef __EEPROM_ItemList_H__
#define __EEPROM_ItemList_H__
//-------------------------------------------------------------------

//-------------------------------------------------------------------
//#include "EEPROM_Item.hpp"
//-------------------------------------------------------------------

#define EEPROMLIST_EMPTY_OWNER	255

class EEPROM_ItemListClass
{
public:
	int StartListPos;
	byte ItemSize;
	int EEPROMSize;

	EEPROM_ItemListClass() { this->ItemSize = 0; this->EEPROMSize = 0; this->StartListPos = 0; }

	void Setup(int inStartListPos, byte inItemSize, int inEEPROMSize) { this->StartListPos = inStartListPos;  this->ItemSize = inItemSize; this->EEPROMSize = inEEPROMSize; }

	void FreeItem(byte inSlotNumber);
	void FreeOwnedItems(byte inOwnerSlotNumber);

	inline int GetItemPos(byte inSlotNumber)
	{
		return GetItemPosRaw(inSlotNumber) + 2;
	}

	inline int GetItemPosRaw(byte inSlotNumber)
	{
		return (inSlotNumber * ItemSize) + StartListPos;
	}

	inline byte GetSlotFromPos(int inPosition)
	{
		return (inPosition - StartListPos) / ItemSize;
	}

	inline byte GetItemType(byte inSlotNumber)
	{
		return EEPROM.read(GetItemPosRaw(inSlotNumber));
	}

	inline byte GetItemOwner(byte inSlotNumber)
	{
		return EEPROM.read(GetItemPosRaw(inSlotNumber) + 1);
	}

	inline int SaveItemPrefix(byte inSlotNumber, byte inType, byte inOwner)
	{
		int pos = GetItemPosRaw(inSlotNumber);
		EEPROM.write(pos++, inType);
		EEPROM.write(pos++, inOwner);
		return pos;
	}

	byte GetFirstFreeSlot();
	byte FindItem(byte inType, byte inStartSlotNumber = 0, byte inOwnerId = 255);
	byte CountItems(byte inType);
	byte CountOwnedItems(byte inOwnerId);
	char *GetItems();

	//static void AddType(byte inType, byte(*Load)(int inStartPos) inLoad, void(*Save)(int inStartPos, byte inOwnerSlot) inSave);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
