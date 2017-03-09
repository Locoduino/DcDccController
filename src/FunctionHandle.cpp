/**********************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a decoder function on a handle>
***********************************************************************/

#include "DcDccControler.h"
#include "FunctionHandle.hpp"

FunctionHandle::FunctionHandle()
{
	this->functionNumber = 0;
	this->pButton = 0;
}

FunctionHandle::FunctionHandle(int inFunctionNumber, ButtonsCommanderButton *inpButton)
{
	this->functionNumber = inFunctionNumber;
	this->pButton = inpButton;
}

unsigned long FunctionHandle::loop()
{
	bool changed = this->pButton->loop();

	return changed;
}
