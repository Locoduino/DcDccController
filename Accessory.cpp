/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one Dcc accessory>
*************************************************************/

#include "DcDccControler.h"
#include "Accessory.hpp"

#include <stdarg.h>

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))

void Accessory::CheckIndex(int inIndex, const __FlashStringHelper *inFunc)
{
	if (this->allSize == 0)
	{
		Serial.print(F("Accessories size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->allSize)
		{
			Serial.print(F("Accessories index error in "));
			Serial.println(inFunc);
		}
}

#define CHECKACCESSORY(val, text)	AllCheckIndex(val, F(text))

void Accessory::AllCheckIndex(int inIndex, const __FlashStringHelper *inFunc)
{
	if (allSize == 0)
	{
		Serial.print(F("Locomotives size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= allSize)
		{
			Serial.print(F("Locomotives index error in "));
			Serial.println(inFunc);
		}
}
#else
#define CHECK(val, text)
#define CHECKACCESSORY(val, text)
#endif

Accessory* *Accessory::pAllAccessories;
int Accessory::allSize;
int Accessory::allAddCounter;

Accessory::Accessory(int inDccId, char *inName, int inDuration)
{
	this->DccId = inDccId;
	//this->Name = inName;
	this->duration = inDuration;
}

/////////////////////////////////////////////////////////////////////////
//		Global accessory list										   //
/////////////////////////////////////////////////////////////////////////

void Accessory::AllSetup(int inNumberOfLocomotives)
{
	allSize = allAddCounter;
	allAddCounter = 0;
	pAllAccessories = new Accessory*[allSize];
}

void Accessory::AllSetup(int inNumberOfAccessories, Accessory *inpFirstAccessory, ...)
{
	va_list argList;

	AllSetup(inNumberOfAccessories);

	AllAdd(inpFirstAccessory);
	inNumberOfAccessories--;

	va_start(argList, inpFirstAccessory);
	for (; inNumberOfAccessories; inNumberOfAccessories--)
	{
		Accessory *pAccessory;
		pAccessory = va_arg(argList, Accessory *);
		AllAdd(pAccessory);
	}
	va_end(argList);
#ifdef DEBUG_MODE
	if (inNumberOfAccessories > 0)
		Serial.println(F("   Not enough items in the list !"));
#endif
}

// Returns the index of the new added accessory.
unsigned char Accessory::AllAdd(Accessory *pAccessory)
{
	CHECKACCESSORY(allAddCounter, "Accessory::AllAdd");
	pAllAccessories[allAddCounter++] = pAccessory;

	return allAddCounter;
}

