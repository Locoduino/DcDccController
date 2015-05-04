//-------------------------------------------------------------------
#ifndef __windowChooseFunction_H__
#define __windowChooseFunction_H__
//-------------------------------------------------------------------

#include "DcDccControler.h"

//-------------------------------------------------------------------

#define WINDOWTYPE_FUNCTIONCHOICE	102

class WindowChooseFunction: public Window
{
private:
	Handle *pHandle;

	Function *pSelected;

public:
	WindowChooseFunction(int inFirstLine, Handle *inpHandle);

	inline byte GetType() const { return WINDOWTYPE_FUNCTIONCHOICE; }
	void Event(byte inEventType, LcdUi *inpLcd);

	Function *GetNextSlot();
	Function *GetPrevSlot();
	inline void SetSelected(Function *inpSelected) { this->pSelected = inpSelected; }
	inline Function *GetSelected() { return this->pSelected; }
};

#endif
//-------------------------------------------------------------------
