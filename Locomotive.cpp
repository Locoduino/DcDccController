/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one locomotive>
*************************************************************/

#include "DcDccControler.h"
#include "Locomotive.hpp"

#include <stdarg.h>
#include "EEPROM_ItemList.hpp"

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))

void Locomotive::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
	if (this->size == 0)
	{
		Serial.print(F("Size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->size)
		{
			Serial.print(F("Index error in "));
			Serial.println(inFunc);
		}
}
#else
#define CHECK(val, text)
#endif

Locomotive Locomotive::AnalogLocomotive(1, 0, "analog", 128);

Locomotive::Locomotive()
{
	this->pFunctions = 0;
	this->functionsAddCounter = 0;
	this->size = 0;

	Clear();
}

Locomotive::Locomotive(uint16_t inDccId, uint8_t inAdressKind, char *inName, uint8_t inSteps) : Locomotive()
{
	this->SlotNumber = 255;
	this->DccId = inDccId;
	this->addressKind = inAdressKind;
	STRCPY(this->Name, inName);
	this->steps = inSteps;
}

void Locomotive::Setup(byte inNumberOfFunctions)
{
	SetFunctionsSize(inNumberOfFunctions);
}

void Locomotive::SetFunctionsSize(byte inNumberOfFunctions)
{
	// Delete all existing functions
	for (int i = 0; i < this->GetFunctionNumber(); i++)
	{
		delete this->pFunctions[i];
		this->pFunctions[i] = 0;
	}

	this->functionsAddCounter = 0;

	// If the new size is the same, only restart the add counter
	if (this->size == inNumberOfFunctions)
		return;

	if (this->pFunctions != 0)
		delete[] this->pFunctions;
	this->size = inNumberOfFunctions;
	if (this->size == 0)
		this->pFunctions = 0;
	else
		this->pFunctions = new Function*[this->size];
}

void Locomotive::Setup(byte inNumberOfFunctions, Function *inpFirstFunction, ...)
{
	va_list argList;

	this->Setup(inNumberOfFunctions);

	this->AddFunction(inpFirstFunction);
	inNumberOfFunctions--;

	va_start(argList, inpFirstFunction);
	for (; inNumberOfFunctions; inNumberOfFunctions--)
	{
		Function *pFunction;
		pFunction = va_arg(argList, Function *);
		this->AddFunction(pFunction);
	}
	va_end(argList);
#ifdef DEBUG_MODE
	if (inNumberOfFunctions > 0)
		Serial.println(F("   Not enough items in the list !"));
#endif
}

byte Locomotive::AddFunction(Function *pFunction)
{
	CHECK(this->functionsAddCounter, "Locomotive::AddFunction");
	this->pFunctions[this->functionsAddCounter] = pFunction;

	return this->functionsAddCounter++;
}

Function *Locomotive::GetFunction(int inDccId) const
{
	for (int i = 0; i < this->functionsAddCounter; i++)
	{
		if (this->pFunctions[i]->DccIdFunction == inDccId)
			return this->pFunctions[i];
	}

	return 0;
}

byte Locomotive::GetFunctionIndex(Function *inpRef) const
{
	for (int i = 0; i < this->functionsAddCounter; i++)
	{
		if (this->pFunctions[i] == inpRef)
			return i;
	}

	return 255;
}

void Locomotive::Copy(const Locomotive &inLocomotive)
{
	this->SlotNumber = inLocomotive.SlotNumber;
	this->DccId = inLocomotive.DccId;
	this->addressKind = inLocomotive.addressKind;
	STRCPY(this->Name, inLocomotive.Name);
	this->steps = inLocomotive.steps;

	this->SetFunctionsSize(inLocomotive.GetFunctionNumber());

	for (int f = 0; f < inLocomotive.GetFunctionNumber(); f++)
		this->AddFunction(new Function(*inLocomotive.GetFunctionFromIndex(f)));
}

void Locomotive::Load(int inStartPos)
{
	inStartPos += EEPROMextent.readAnything(inStartPos, this->DccId);
	EEPROMextent.readString(inStartPos, this->Name, 12);
	inStartPos += 12;
	this->addressKind = EEPROMextent.read(inStartPos++);
	this->steps = EEPROMextent.read(inStartPos++);
}

void Locomotive::LoadName(int inStartPos, char *outpName)
{
	uint16_t dccId;
	inStartPos += EEPROMextent.readAnything(inStartPos, dccId);
	EEPROMextent.readString(inStartPos, outpName, 12);
}

void Locomotive::Save(int inStartPos)
{
	inStartPos += EEPROMextent.updateAnything(inStartPos, this->DccId);
	EEPROMextent.updateString(inStartPos, this->Name);
	inStartPos += 12;
	EEPROMextent.write(inStartPos++, this->addressKind);
	EEPROMextent.write(inStartPos, this->steps);
}

void Locomotive::Clear()
{
	this->SlotNumber = 255;

	this->DccId = 0;
	this->steps = 0;
	this->addressKind = 0;
	this->Name[0] = 0;

	SetFunctionsSize(0);
}
