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
	digitalWrite(inDcPWMpin, HIGH);

	this->dps.setup();
}

bool ControlerDcc::SetSpeed(int inNewSpeed)
{
	int val = inNewSpeed * (this->pControled->GetDirectionToLeft() ? -1 : 1);

	if (this->pControled->GetMappedSpeed() == val)
		return false;

	// a value of 0 = estop
	// a value of 1/-1 = stop
	// a value >1 (or <-1) means go.
	if (val == 0)
	{
		val = 1;
		if (this->pControled->GetMappedSpeed() < 0)
			val = -1;
	}

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc SetSpeed "));
	Serial.print(inNewSpeed);
	Serial.print(F(" (in Dcc "));
	Serial.print(val);
	Serial.println(F(" )"));
#endif

	dps.setSpeed(this->pControled->GetDccId(), this->pControled->GetDccAddressKind(), val, this->pControled->GetSteps());
	dps.update();

	this->pControled->SetMappedSpeed(val);

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
	this->SetSpeed(inToLeft ? -1 : 1);
	return true;
}

void ControlerDcc::ToggleFunction(byte inFunctionNumber)
{
	this->pControled->GetFunctionFromIndex(inFunctionNumber).Toggle();

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDcc SetFunction "));
	Serial.print(inFunctionNumber);
	Serial.println(this->pControled->GetFunctionFromIndex(inFunctionNumber).IsActivated() ? F("On") : F("Off"));
#endif

	byte fcts = 0;

	for (int i = 0; i < this->pControled->FunctionNumber; i++)
		if (this->pControled->GetFunctionFromIndex(i).IsActivated())
			fcts ^= 1 << i;

	dps.setFunctions(this->pControled->GetDccId(), this->pControled->GetDccAddressKind(), fcts);
	dps.update();
}

void ControlerDcc::PanicStop(bool inStop)
{
#ifdef DEBUG_MODE
	int val = 0;

	if (!inStop)
		if (this->pControled != 0)
			val = this->pControled->GetMappedSpeed();

	Serial.print(F("ControlerDcc PanicStop "));
	Serial.println(val == 0 ? F("pressed"):F("canceled"));
#endif

	dps.eStop();
	dps.update();
}

