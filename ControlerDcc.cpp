/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Dcc Controler>
*************************************************************/

#include "DcDccControler.h"
#include "ControlerDcc.hpp"

void ControlerDcc::Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{ 
	// Set the PWM pin, which is connected to the PWM of LM18200 at maximum !
	pinMode(inDcPWMpin, OUTPUT);
	analogWrite(inDcPWMpin, 255);

	this->dps.setup();
}

void ControlerDcc::SetSpeedRaw()
{
	if (this->panicStopped)
		dps.eStop();
	else
		dps.setSpeed(this->pControled->GetDccId(), this->pControled->GetDccAddressKind(), this->pControled->GetMappedSpeed(), this->pControled->GetSteps());
	dps.update();
}

bool ControlerDcc::SetSpeed(int inNewSpeed)
{
	// a value of 0 = estop
	// a value of 1/-1 = stop
	// a value >1 (or <-1) means go.
	// valid non-estop speeds are in the range [1,127] / [-127,-1] with 1 = stop
	int val = map(inNewSpeed, 0, this->pControled->GetSteps(), 0, 127);

	if (val == 0)
		val = 1;

	val = val * (this->pControled->GetDirectionToLeft() ? -1 : 1);

	if (this->pControled->GetMappedSpeed() == val)
		return false;

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc SetSpeed "));
	Serial.print(inNewSpeed);
	Serial.print(F("/"));
	Serial.print(this->pControled->GetSteps());
	Serial.print(F(" (in Dcc "));
	Serial.print(val);
	Serial.println(F(" )"));
#endif

	this->pControled->SetMappedSpeed(val);
	SetSpeedRaw();

	return true;
}

bool ControlerDcc::SetDirection(bool inToLeft)
{
	if (this->pControled->GetDirectionToLeft() == inToLeft)
		return false;

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc SetDirection "));
	Serial.println(inToLeft ? F("ToLeft") : F("ToRight"));
#endif

	this->pControled->SetDirectionToLeft(inToLeft);
	this->SetSpeed(0);
	return true;
}

void ControlerDcc::SetFunctionsRaw()
{
	uint16_t fcts = 0;

	for (int i = 0; i < this->pControled->FunctionNumber; i++)
	{
		if (this->pControled->GetFunctionFromIndex(i).IsActivated())
			fcts ^= 1 << (this->pControled->GetFunctionFromIndex(i).DccIdFunction - 1);
	}

	dps.setFunctions(this->pControled->GetDccId(), this->pControled->GetDccAddressKind(), fcts);
	dps.update();
}

void ControlerDcc::ToggleFunction(byte inFunctionNumber)
{
	Function &f = this->pControled->GetFunctionFromIndex(inFunctionNumber);
	f.Toggle();

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc SetFunction "));
	Serial.print(this->pControled->GetFunctionFromIndex(inFunctionNumber).DccIdFunction);
	Serial.println(this->pControled->GetFunctionFromIndex(inFunctionNumber).IsActivated() ? F(" On") : F(" Off"));
#endif

	SetFunctionsRaw();
}

void ControlerDcc::PanicStop(bool inStop)
{
	this->panicStopped = inStop;

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc PanicStop "));
	Serial.println(inStop ? F("pressed"):F("canceled"));
#endif

	SetSpeedRaw();
}

#define FRENQUENCYPACKETSFUNCTION	0
#define FRENQUENCYPACKETSSPEED1		1
#define FRENQUENCYPACKETSSPEED2		2

static byte FrequencyPacketsCount = 0;

void ControlerDcc::Loop() 
{ 
	byte mod = FrequencyPacketsCount % 10;
	FrequencyPacketsCount++;
	if (mod == FRENQUENCYPACKETSFUNCTION)
	{
		SetFunctionsRaw();
		return;
	}
	if (mod == FRENQUENCYPACKETSSPEED1 || mod == FRENQUENCYPACKETSSPEED2)
	{
		SetSpeedRaw();
		return;
	}
	dps.update();
}
