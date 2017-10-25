//-------------------------------------------------------------------
#ifndef __windowChooseSlot_H__
#define __windowChooseSlot_H__
//-------------------------------------------------------------------

#include "DcDccControler.h"

//-------------------------------------------------------------------

#ifndef NANOCONTROLER
#define WINDOWTYPE_SLOTCHOICE	18

enum CanSelectSlot
{
	FreeRollingStockSlot,
	UsedRollingStockSlot,
	AllStock
};

/*
A slot choice include a known list of slots. This list is handled by the slot list pLocoSlots.
A free place is given by a slot nomber set to 255.
*/

class WindowChooseSlot: public WindowChoiceText
{
private:
	CanSelectSlot canSelect;

public:
	WindowChooseSlot() { this->canSelect = AllStock; }
	void begin(int inFirstLine, byte *inpSlot, CanSelectSlot inCanSelect) { WindowChoiceText::begin(inFirstLine, inpSlot); this->canSelect = inCanSelect;  }

	inline byte GetType() const { return WINDOWTYPE_SLOTCHOICE; }
	byte GetChoiceTextNumber() const { return ROLLINGSTOCK_NUMBER; }

	char *GetChoiceTextValue(byte indexValue, LcdScreen *apScreen) const;
	void Event(byte inEventType, LcdUi *inpLcd);
};
#endif
#endif
//-------------------------------------------------------------------
