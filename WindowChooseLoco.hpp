//-------------------------------------------------------------------
#ifndef __windowChooseLoco_H__
#define __windowChooseLoco_H__
//-------------------------------------------------------------------

#include "DcDccControler.h"

//-------------------------------------------------------------------

#define WINDOWTYPE_LOCOCHOICE	101

/*
A loco choice include a known list of locos. This list is handled by the slot list pLocoSlots.
This list is ended by an invalid slot number : 255.
It is not allowed to have a 255 in the middle of this list !

The list is build at each change on the locomotive list : start/add/remove .
*/

class WindowChooseLoco: public Window
{
private:
	Handle *pHandle;

	static byte *pLocoSlots;
	static byte selectedSlot;

public:
	WindowChooseLoco(int inFirstLine, Handle *inpHandle);

	inline byte GetType() const { return WINDOWTYPE_LOCOCHOICE; }
	void Event(byte inEventType, LcdUi *inpLcd);
	inline byte GetChoiceValue() const { return WindowChooseLoco::selectedSlot; }

	static void ClearChoices() { WindowChooseLoco::pLocoSlots = 0; WindowChooseLoco::selectedSlot = 255; }
	static void RebuildChoices();
	static byte GetNextLocoSlot(byte slot);
	static byte GetPrevLocoSlot(byte slot);
	inline static void SetSelectedLoco(byte inSelectedSlot) { WindowChooseLoco::selectedSlot = inSelectedSlot; }
};

#endif
//-------------------------------------------------------------------
