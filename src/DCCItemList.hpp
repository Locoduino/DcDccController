//-------------------------------------------------------------------
#ifndef __DCCItemList_HPP__
#define __DCCItemList_HPP__
//-------------------------------------------------------------------

//-------------------------------------------------------------------
#include "Locomotive.hpp"
#include "Function.hpp"
#include "EEPROMextent.h"

//-------------------------------------------------------------------

#define DCCItemList	DCCItemListClass::DCCItemListInstance

class DCCItemListClass : public EEPROM_ItemListClass
{
public:
	DCCItemListClass() {}

	static DCCItemListClass DCCItemListInstance;

	byte AddLoco(Locomotive *outpNewLoco);
	void FreeLoco(Locomotive *outpNewLoco);
	void FreeLoco(byte inSlotNumber);
	void UpdateLoco(Locomotive *outpNewLoco);
	void GetLoco(byte inSlotNumber, Locomotive *outpLoco);

#ifdef DDC_DEBUG_MODE
	void printList(byte numberMax = 0);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
