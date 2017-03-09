//-------------------------------------------------------------------
#ifndef __functionHandle_H__
#define __functionHandle_H__
//-------------------------------------------------------------------

#include "Commanders.h"

//-------------------------------------------------------------------

// This class describes a function handle.

class FunctionHandle
{
	private:
		int functionNumber;
		ButtonsCommanderButton *pButton;

	public:
		FunctionHandle();
		FunctionHandle(int inFunctionNumber, ButtonsCommanderButton *inpButton);
		
		unsigned long loop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
