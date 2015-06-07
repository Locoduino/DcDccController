/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a decoder function>
*************************************************************/

#include "DcDccControler.h"
#include "Function.hpp"

Function::Function(int inDccId)
{
	this->DccIdFunction = inDccId;
	this->activated = false;
}

void Function::Copy(const Function &inFunction)
{
	this->SlotNumber = inFunction.SlotNumber;
	this->DccIdFunction = inFunction.DccIdFunction;
	this->activated = inFunction.activated;
}

void Function::Load(int inStartPos)
{
	int pos = inStartPos, size = 0;
	size = EEPROMextent.readAnything(pos, this->DccIdFunction);
	pos += size;
	this->activated = EEPROMextent.read(pos++);
}

void Function::Save(int inStartPos)
{
	inStartPos += EEPROMextent.updateAnything(inStartPos, this->DccIdFunction);
	EEPROMextent.update(inStartPos++, this->activated);
}
