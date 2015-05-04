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
	this->activated = false;
	this->pButton = 0;
}

FunctionHandle::FunctionHandle(int inFunctionNumber, ButtonsCommanderButton *inpButton)
{
	this->functionNumber = inFunctionNumber;
	this->activated = false;
	this->pButton = inpButton;
}

bool FunctionHandle::Loop()
{
	bool changed = this->pButton->Loop();

	return changed;
}
