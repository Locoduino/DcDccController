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
public:
	inline WindowFunction() {}
	void begin(byte inFirstLine, int *inpValue);

	byte GetType() const { return WINDOWTYPE_FUNCTION; }
	void Event(byte inEventType, LcdUi *inpLcd);

#ifdef LCDUI_PRINT_WINDOWS
	void printWindow();
#endif
};
#endif
//-------------------------------------------------------------------
