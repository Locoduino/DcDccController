//-------------------------------------------------------------------
#ifndef __functionHandle_H__
#define __functionHandle_H__
//-------------------------------------------------------------------

#include "ButtonsCommanderButton.hpp"

//-------------------------------------------------------------------

// This class describes a function handle.

class FunctionHandle
{
	private:
		int functionNumber;
		bool activated;
		ButtonsCommanderButton *pButton;

	public:
		FunctionHandle();
		FunctionHandle(int inFunctionNumber, ButtonsCommanderButton *inpButton);
		
		inline bool IsActivated() const { return this->activated; }
		inline void SetActivated(bool inActivated) { this->activated = inActivated; }
		bool Loop();
		void Setup();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
