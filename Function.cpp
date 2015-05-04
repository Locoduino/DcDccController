/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a decoder function>
*************************************************************/

#include "DcDccControler.h"
#include "Function.hpp"

Function::Function(int inDccId, char *inName, FunctionType inType)
{
	this->DccIdFunction = inDccId;
	STRCPY(this->name, inName);
	this->Type = inType;
}

void Function::Load(int inStartPos)
{
	int pos = inStartPos, size = 0;
	size = EEPROMextent.readAnything(pos, this->DccIdFunction);
	pos += size;
	EEPROMextent.readString(pos, this->name, 12);
	pos += 12;
	this->Type = (FunctionType)EEPROMextent.read(pos++);
}

void Function::Save(int inStartPos)
{
	int size = 0;
	size = EEPROMextent.updateAnything(inStartPos, this->DccIdFunction);
	inStartPos += size;
	EEPROMextent.updateString(inStartPos, this->name);
	inStartPos += 12;
	EEPROMextent.update(inStartPos++, this->Type);
}
