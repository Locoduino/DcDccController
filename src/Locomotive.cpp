/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one locomotive>
*************************************************************/

#include "DcDccControler.h"
#include "Locomotive.hpp"

#include <stdarg.h>
#include "EEPROMextent.h"

#ifdef DDC_DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))

void Locomotive::CheckIndex(byte inIndex, const __FlashStringHelper *inFunc)
{
	if (inIndex >= FUNCTION_NUMBER)
	{
		Serial.print(F("Index error in "));
		Serial.println(inFunc);
	}
}
#else
#define CHECK(val, text)
#endif

Locomotive Locomotive::AnalogLocomotive(1, 0, (char *)"", 255);

Locomotive::Locomotive()
{
	Clear();
}

Locomotive::Locomotive(int inDccId, uint8_t inAdressKind, char *inName, uint8_t inSteps) : Locomotive()
{
	this->SlotNumber = 255;
	this->DccId = inDccId;
#ifndef NANOCONTROLER
	STRCPY(this->Name, inName);
#endif
	this->steps = inSteps;
}

void Locomotive::ClearFunctions()
{
	// Delete all existing functions
	for (int i = 0; i < FUNCTION_NUMBER; i++)
	{
		this->Functions[i].Clear();
	}
}

byte Locomotive::GetFunctionIndex(const int inDccId) const
{
	for (int i = 0; i < FUNCTION_NUMBER; i++)
	{
		if (this->Functions[i].DccIdFunction == inDccId)
			return i;
	}

	return 255;
}

void Locomotive::Copy(Locomotive &inLocomotive)
{
	this->SlotNumber = inLocomotive.SlotNumber;
	this->DccId = inLocomotive.DccId;
#ifndef NANOCONTROLER
	STRCPY(this->Name, inLocomotive.Name);
#endif
	this->steps = inLocomotive.steps;

	for (int f = 0; f < FUNCTION_NUMBER; f++)
		this->Functions[f].Copy(inLocomotive.Functions[f]);
}

void Locomotive::Load(int inStartPos)
{
	inStartPos += EEPROMextent.readAnything(inStartPos, this->DccId);
#ifndef NANOCONTROLER
	EEPROMextent.readString(inStartPos, this->Name, DCC_LOCONAME_LENGTH);
	inStartPos += DCC_LOCONAME_LENGTH;
#endif
	this->steps = EEPROMextent.readByte(inStartPos++);

	if (this->DccId == 0)
		this->Clear();
}

#ifndef NANOCONTROLER
void Locomotive::LoadName(int inStartPos, char *outpName)
{
	int dccId;
	inStartPos += EEPROMextent.readAnything(inStartPos, dccId);
	EEPROMextent.readString(inStartPos, outpName, DCC_LOCONAME_LENGTH);
}
#endif

void Locomotive::Save(int inStartPos)
{
	inStartPos += EEPROMextent.updateAnything(inStartPos, this->DccId);
#ifndef NANOCONTROLER
	EEPROMextent.updateString(inStartPos, this->Name);
	inStartPos += DCC_LOCONAME_LENGTH;
#endif
	EEPROMextent.writeByte(inStartPos, this->steps);
}

void Locomotive::Clear()
{
	this->SlotNumber = 255;

	this->DccId = 3;
	this->steps = 128;
#ifndef NANOCONTROLER
	this->Name[0] = 0;
#endif
	this->directionToLeft = true;
	this->ClearFunctions();
}

#ifdef DDC_DEBUG_MODE
void Locomotive::print()
{
	Serial.print("Loco Id ");
	Serial.print(this->DccId);
	Serial.print(" / Steps ");
	Serial.print(this->steps);

	for (int f = 0; f < FUNCTION_NUMBER; f++)
	{
		Serial.print(" / F");
		Serial.print(f);
		Serial.print(" = ");
		Serial.print(this->Functions[f].DccIdFunction);
	}

	Serial.println("");
}
#endif