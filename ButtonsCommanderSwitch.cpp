/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Switch button with debounce.>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderSwitch.hpp"

ButtonsCommanderSwitch::ButtonsCommanderSwitch() : ButtonsCommanderButton()
{
	this->debounceDelay = 50;
	this->buttonState = false;
	this->lastButtonState = false;
	this->lastDebounceTime = 0;

	this->pin = DP_INVALID;
}

void ButtonsCommanderSwitch::Setup(int inPin)
{
//	CHECKPIN(inPin1, "ButtonsCommanderSwitch::Setup");

	this->pin = Arduino_to_GPIO_pin(inPin);
	pinMode2f(this->pin, INPUT_PULLUP);

	this->lastButtonState = digitalRead2f(this->pin) > 0;
	this->buttonState = this->lastButtonState;
}

bool ButtonsCommanderSwitch::Loop()
{
	// read the state of the switch into a local variable:
	bool reading = digitalRead2f(this->pin) > 0;
	this->UnselectLastLoop();

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->lastButtonState)
	{
		// reset the debouncing timer
		this->lastDebounceTime = millis();
	}

	bool haveFound = false;

	if (this->lastDebounceTime > 0 && (millis() - this->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->buttonState)
		{
			this->buttonState = reading;
			haveFound = true;
			this->SelectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Switch pressed "));
#endif
		}
		this->lastDebounceTime = 0;
	}

	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	this->lastButtonState = reading;

	return haveFound;
}
