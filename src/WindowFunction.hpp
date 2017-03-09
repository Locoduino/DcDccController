//-------------------------------------------------------------------
#ifndef __windowFunction_H__
#define __windowFunction_H__
//-------------------------------------------------------------------

#include "LcdUi.h"

//-------------------------------------------------------------------

#define WINDOWTYPE_FUNCTION	13

#define FUNCTION_MIN	0
#define FUNCTION_MAX	28

class WindowFunction : public WindowInt
{
private:
	int intValue;
																			   public:
	inline WindowFunction() {}
	inline void begin(byte inFirstLine, int *inpValue) { this->WindowInt::begin(inFirstLine, inpValue, FUNCTION_MIN, FUNCTION_MAX);	}

	inline byte GetType() const { return WINDOWTYPE_FUNCTION; }
	void Event(byte inEventType, LcdUi *inpLcd);
};																														 
#endif
//-------------------------------------------------------------------
