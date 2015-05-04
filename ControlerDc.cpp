/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Dc Controler>
*************************************************************/

#include "DcDccControler.h"
#include "ControlerDc.hpp"

void ControlerDc::Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{ 
	this->dcPWMpin = inDcPWMpin; 
	this->dcDirPin = inDcDirPin; 

	analogWrite(this->dcPWMpin, 0);
	pinMode(this->dcDirPin, OUTPUT);
}

bool ControlerDc::SetSpeed(int inNewSpeed)
{
	int val;

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDc SetSpeed "));
	Serial.println(inNewSpeed);
#endif

	val = map(inNewSpeed, 0, this->pControled->GetSteps(), 0, 255);

	if (this->pControled->GetMappedSpeed() == val)
		return false;

	analogWrite(this->dcPWMpin, val);
	if (this->pControled->GetDirectionToLeft())
		digitalWrite(this->dcDirPin, LOW);
	else
		digitalWrite(this->dcDirPin, HIGH);

	this->pControled->SetMappedSpeed(val);

	return true;
}

bool ControlerDc::SetDirection(bool inToLeft)
{
	if (this->pControled->GetDirectionToLeft() == inToLeft)
		return false;

#ifdef DEBUG_MODE
	Serial.print(F("ControlerDc SetDirection "));
	Serial.println(inToLeft ? F("ToLeft"):F("ToRight"));
#endif

	this->pControled->SetDirectionToLeft(inToLeft);
	this->SetSpeed(0);
	return true;
}

void ControlerDc::PanicStop(bool inStop) 
{
	this->panicStopped = inStop;

	if (inStop)
	{
#ifdef DEBUG_MODE
		Serial.println(F("ControlerDc PanicStop pressed"));
#endif

		analogWrite(this->dcPWMpin, 0);
	}
	else
	{
#ifdef DEBUG_MODE
		Serial.println(F("ControlerDc PanicStop canceled"));
#endif
		analogWrite(this->dcPWMpin, this->pControled->GetMappedSpeed());
		if (this->pControled->GetDirectionToLeft())
			digitalWrite(this->dcDirPin, LOW);
		else
			digitalWrite(this->dcDirPin, HIGH);
	}
}

