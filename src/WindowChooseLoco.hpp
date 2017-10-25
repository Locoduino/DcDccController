//-------------------------------------------------------------------
#ifndef __windowChooseLoco_H__
#define __windowChooseLoco_H__
//-------------------------------------------------------------------

#include "DcDccControler.h"

//-------------------------------------------------------------------

#ifndef NANOCONTROLER
#define WINDOWTYPE_LOCOCHOICE	17

enum CanSelectLoco
{
	InRollingStock,
	NotInRollingStock,
	AllLocos
};

/*
A loco choice include a known list of locos. This list is handled by the slot list LocoSlots.
This list is ended by an invalid slot number : 255.
It is not allowed to have a 255 in the middle of this list !

The list is build at each change on the locomotive list : start/add/remove/edit .
*/

class WindowChooseLoco: public WindowChoiceText
{
private:
	CanSelectLoco canSelect;

public:
	WindowChooseLoco() {}
	void begin(int inFirstLine, byte *inpLocoSlot, CanSelectLoco inCanSelect) { WindowChoiceText::begin(inFirstLine, inpLocoSlot); this->canSelect = inCanSelect; }

	inline byte GetType() const { return WINDOWTYPE_LOCOCHOICE; }
	byte GetChoiceTextNumber() const;
	char *GetChoiceTextValue(byte indexValue, LcdScreen *apScreen) const;
	void Event(byte inEventType, LcdUi *inpLcd);
};
#endif
#endif
//-------------------------------------------------------------------
