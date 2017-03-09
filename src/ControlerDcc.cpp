/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Dcc Controler>
*************************************************************/

#include "DcDccControler.h"
#include "ControlerDcc.hpp"

void ControlerDcc::begin(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{ 
	// Set the PWM pin, which is connected to the PWM of LM18200 at maximum !
	//pinMode(inDcPWMpin, OUTPUT);
	analogWrite(inDcPWMpin, 255);

	this->dps.setup();

#ifdef DDC_DEBUG_MODE
	Serial.println(F("Dcc mode."));
#endif
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

#ifdef DDC_DEBUG_MODE
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

#ifdef DDC_DEBUG_MODE
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

	for (int i = 0; i < FUNCTION_NUMBER; i++)
	{
		if (this->pControled->Functions[i].IsActivated())
			fcts ^= 1 << (this->pControled->Functions[i].DccIdFunction);
	}

	dps.setFunctions(this->pControled->GetDccId(), this->pControled->GetDccAddressKind(), fcts);
	dps.update();
}

void ControlerDcc::ToggleFunction(byte inFunctionNumber)
{
	Function &f = this->pControled->Functions[inFunctionNumber];
	f.Toggle();

#ifdef DDC_DEBUG_MODE
	Serial.print(F("ControlerDcc SetFunction "));
	Serial.print(this->pControled->Functions[inFunctionNumber].DccIdFunction);
	Serial.println(this->pControled->Functions[inFunctionNumber].IsActivated() ? F(" On") : F(" Off"));
#endif

	SetFunctionsRaw();
}

void ControlerDcc::PanicStop(bool inStop)
{
	this->panicStopped = inStop;

#ifdef DDC_DEBUG_MODE
	Serial.print(F("ControlerDcc PanicStop "));
	Serial.println(inStop ? F("pressed"):F("canceled"));
#endif

	SetSpeedRaw();
}

void ControlerDcc::StartProgramMode()
{
	this->programMode = true;
}

void ControlerDcc::EndProgramMode()
{
	this->programMode = false;
}

void ControlerDcc::SetCv1Raw(int inId)
{
	//PanicStop(true);
	dps.opsEndProgram();
	for (int i = 0; i < 10; i++)
		dps.update(); // send it NOW !
	dps.opsAddressOnly(inId);
	for (int i = 0; i < 10; i++)
		dps.update(); // send it NOW !
	dps.opsEndProgram();
	for (int i = 0; i < 10; i++)
		dps.update(); // send it NOW !

#ifdef DDC_DEBUG_MODE
	Serial.print(F("ControlerDcc SetCv 1 : "));
	Serial.println(inId);
#endif
}

void ControlerDcc::SetCv1(int inId)
{
	SetCv1Raw(inId);
}

#define FRENQUENCYPACKETSFUNCTION	0
#define FRENQUENCYPACKETSSPEED1		1
#define FRENQUENCYPACKETSSPEED2		2

static byte FrequencyPacketsCount = 0;

void ControlerDcc::loop() 
{ 
	if (!this->programMode)
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
	}
	dps.update();
}
