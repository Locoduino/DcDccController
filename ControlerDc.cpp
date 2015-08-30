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
	//this->DCFrequencyDivisor = 64;

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

#ifdef DEBUG_MODE
	Serial.print(F("analogWrite pin "));
	Serial.print(this->dcPWMpin);
	Serial.print(F(" / value "));
	Serial.println(val);
#endif
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

void ControlerDc::SetFrequencyDivisor(unsigned int inDivisor)
{
	this->DCFrequencyDivisor = inDivisor;

	SetFrequencyDivisorRaw(inDivisor);
}

void ControlerDc::SetFrequencyDivisorRaw(unsigned int inDivisor)
{
#ifndef VISUALSTUDIO
	byte mode;
	if (/*this->dcPWMpin == 5 || this->dcPWMpin == 6 || */this->dcPWMpin == 9 || this->dcPWMpin == 10) 
	{
		switch (inDivisor) 
		{
			case 1: mode = 0x01; break;
			case 8: mode = 0x02; break;
			case 64: mode = 0x03; break;
			case 256: mode = 0x04; break;
			case 1024: mode = 0x05; break;
			default: return;
		}
		/*if (this->dcPWMpin == 5 || this->dcPWMpin == 6)
			TCCR0B = TCCR0B & 0b11111000 | mode;
		else*/
			TCCR1B = TCCR1B & 0b11111000 | mode;
		return;
	}
/*	Only 9 or 10 are allowed for the PWM pins...
	if (this->dcPWMpin == 3 || this->dcPWMpin == 11) 
	{
		switch (inDivisor) 
		{
			case 1: mode = 0x01; break;
			case 8: mode = 0x02; break;
			case 32: mode = 0x03; break;
			case 64: mode = 0x04; break;
			case 128: mode = 0x05; break;
			case 256: mode = 0x06; break;
			case 1024: mode = 0x7; break;
			default: return;
		}
		TCCR2B = TCCR2B & 0b11111000 | mode;
	}
	*/
#endif
}


