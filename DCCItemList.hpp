//-------------------------------------------------------------------
#ifndef __DCCItemList_HPP__
#define __DCCItemList_HPP__
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#include "Locomotive.hpp"
#include "Function.hpp"
#include "EEPROM_ItemList.hpp"
//-------------------------------------------------------------------

#define DCCItemList	DCCItemListClass::DCCItemListInstance

class DCCItemListClass : public EEPROM_ItemListClass
{
public:
	DCCItemListClass() {}

	static DCCItemListClass DCCItemListInstance;

	void AddLoco(Locomotive *outpNewLoco);
	void FreeLoco(Locomotive *outpNewLoco);
	void UpdateLoco(Locomotive *outpNewLoco);
	void GetLoco(byte inSlotNumber, Locomotive *outpLoco);
/*
	void GetLocos(TaggedStringList *outpLocos);
*/
#ifdef DEBUG_MODE
	void printList(byte numberMax = 0);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
