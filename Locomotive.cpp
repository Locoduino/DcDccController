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

Locomotive Locomotive::AnalogLocomotive(1, 0, "analog", 255);
byte Locomotive::FunctionNumber = 255;

Locomotive::Locomotive()
{
	this->pFunctions = 0;
	this->functionsAddCounter = 0;
	this->size = 0;
	SetFunctionsSize(FunctionNumber);

	Clear();
}

Locomotive::Locomotive(uint16_t inDccId, uint8_t inAdressKind, char *inName, uint8_t inSteps) : Locomotive()
{
	this->SlotNumber = 255;
	this->DccId = inDccId;
	this->addressKind = inAdressKind;
#ifndef NANOCONTROLER
	STRCPY(this->Name, inName);
#endif
	this->steps = inSteps;
}

void Locomotive::SetFunctionsSize(byte inNumberOfFunctions)
{
	// Delete all existing functions
	if (this->pFunctions != 0)
		for (int i = 0; i < this->GetFunctionNumber(); i++)
		{
			this->pFunctions[i].DccIdFunction = 0;
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
		this->pFunctions = new Function[this->size];
}

byte Locomotive::AddFunction(const Function &inFunction)
{
	CHECK(this->functionsAddCounter, "Locomotive::AddFunction");
	this->pFunctions[this->functionsAddCounter].Copy(inFunction);

	return this->functionsAddCounter++;
}

byte Locomotive::GetFunctionIndex(const Function &inRef) const
{
	for (int i = 0; i < this->functionsAddCounter; i++)
	{
		if (this->pFunctions[i].DccIdFunction == inRef.DccIdFunction)
			return i;
	}

	return 255;
}

void Locomotive::Copy(const Locomotive &inLocomotive)
{
	this->SlotNumber = inLocomotive.SlotNumber;
	this->DccId = inLocomotive.DccId;
	this->addressKind = inLocomotive.addressKind;
#ifndef NANOCONTROLER
	STRCPY(this->Name, inLocomotive.Name);
#endif
	this->steps = inLocomotive.steps;

	this->SetFunctionsSize(inLocomotive.GetFunctionNumber());

	for (int f = 0; f < inLocomotive.GetFunctionNumber(); f++)
		this->AddFunction(inLocomotive.GetFunctionFromIndex(f));
}

void Locomotive::Load(int inStartPos)
{
	inStartPos += EEPROMextent.readAnything(inStartPos, this->DccId);
#ifndef NANOCONTROLER
	EEPROMextent.readString(inStartPos, this->Name, 12);
	inStartPos += 12;
#endif
	this->addressKind = EEPROMextent.read(inStartPos++);
	this->steps = EEPROMextent.read(inStartPos++);
}

#ifndef NANOCONTROLER
void Locomotive::LoadName(int inStartPos, char *outpName)
{
	uint16_t dccId;
	inStartPos += EEPROMextent.readAnything(inStartPos, dccId);
	EEPROMextent.readString(inStartPos, outpName, 12);
}
#endif

void Locomotive::Save(int inStartPos)
{
	inStartPos += EEPROMextent.updateAnything(inStartPos, this->DccId);
#ifndef NANOCONTROLER
	EEPROMextent.updateString(inStartPos, this->Name);
	inStartPos += 12;
#endif
	EEPROMextent.write(inStartPos++, this->addressKind);
	EEPROMextent.write(inStartPos, this->steps);
}

void Locomotive::Clear()
{
	this->SlotNumber = 255;

	this->DccId = 0;
	this->steps = 0;
	this->addressKind = 0;
#ifndef NANOCONTROLER
	this->Name[0] = 0;
#endif
	this->directionToLeft = true;

	SetFunctionsSize(0);
}
