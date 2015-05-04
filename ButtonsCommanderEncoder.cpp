/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Encoder returning a current value.>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderEncoder.hpp"

ButtonsCommanderEncoder::ButtonsCommanderEncoder(int inStartingValue, int inMinimum, int inMaximum)
{
	this->mini = inMinimum;
	this->maxi = inMaximum;
	this->currentValue = inStartingValue;
	this->lastEncoded = 0;
	this->pin1Set = false;
	this->pin2Set = false;
}

void ButtonsCommanderEncoder::Setup(int inPin1, int inPin2, int inMoveAccuracy)
{
	this->pin1 = Arduino_to_GPIO_pin(inPin1);
	this->pin2 = Arduino_to_GPIO_pin(inPin2);
	pinMode2f(this->pin1, INPUT_PULLUP);
	pinMode2f(this->pin2, INPUT_PULLUP);
	this->lastEncoded = 0;
	if (this->moveAccuracy <= 0)
		this->moveAccuracy = 1;
}

bool ButtonsCommanderEncoder::Loop()
{
	bool rotating = true;  // reset the debouncer

	if (rotating) delay(1);
	if (digitalRead2f(this->pin1) != this->pin1Set) {
		this->pin1Set = !this->pin1Set;
		if (this->pin1Set && !this->pin2Set)
		{
			this->currentValue--;

			if (this->currentValue < this->mini)
				this->currentValue = this->mini;
			else
			{
				this->SelectLastLoop();
				this->lastWasPositive = false;
			}
		}

		rotating = false;
	}

	if (digitalRead2f(this->pin2) != this->pin2Set) {  // debounce once more
		this->pin2Set = !this->pin2Set;

		if (this->pin2Set && !this->pin1Set)
		{
			this->currentValue++;
			if (this->currentValue > this->maxi)
				this->currentValue = this->maxi;
			else
			{
				this->SelectLastLoop();
				this->lastWasPositive = true;
			}
		}

		rotating = false;  // no more debouncing until loop() hits again
	}

	return !rotating;
}
