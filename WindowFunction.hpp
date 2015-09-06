//-------------------------------------------------------------------
#ifndef __windowFunction_H__
#define __windowFunction_H__
//-------------------------------------------------------------------

#include "Window.hpp"

//-------------------------------------------------------------------

/*
#define WINDOWTYPE_FUNCTION	13

class WindowFunction : public WindowInt
{
private:
	int intValue;

public:
	WindowFunction(byte inFirstLine, int inNumber);

	inline byte GetType() const { return WINDOWTYPE_FUNCTION; }

	void Event(byte inEventType, LcdUi *inpLcd);
};
  */

#define WINDOWTYPE_FUNCTION	13

#define FUNCTION_MIN	1
#define FUNCTION_MAX	10026

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
