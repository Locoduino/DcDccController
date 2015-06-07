//-------------------------------------------------------------------
#ifndef __windowFunction_H__
#define __windowFunction_H__
//-------------------------------------------------------------------

#include "WindowInt.hpp"

//-------------------------------------------------------------------

#define WINDOWTYPE_FUNCTION	103

class WindowFunction : public WindowInt
{
private:
	int intValue;

public:
	WindowFunction(byte inFirstLine, int inNumber);

	inline byte GetType() const { return WINDOWTYPE_FUNCTION; }

	void Event(byte inEventType, LcdUi *inpLcd);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
