/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Potentiometer returning a current value.>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderPotentiometer.hpp"

ButtonsCommanderPotentiometer::ButtonsCommanderPotentiometer(int inMinimum, int inMaximum) : ButtonsCommanderButton()
{
	this->mini = inMinimum;
	this->maxi = inMaximum;
}

void ButtonsCommanderPotentiometer::Setup(int inPin, int inMoveAccuracy)
{
	this->pin = inPin;
	this->moveAccuracy = inMoveAccuracy - 1;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;

	int val = analogRead(pin);
	this->currentValue = map(val, 0, 1023, this->mini, this->maxi);
}

bool ButtonsCommanderPotentiometer::Loop()
{
	this->UnselectLastLoop();
	int val = analogRead(pin);

	val = map(val, 0, 1023, this->mini, this->maxi);

	if (val < this->currentValue - this->moveAccuracy || val > this->currentValue + this->moveAccuracy)
	{
#ifdef DEBUG_MODE
		Serial.print(F("Potentiometer new value : "));
		Serial.println(val, DEC);
#endif
		this->currentValue = val;
		this->SelectLastLoop();
		return true;
	}

	return false;
}
