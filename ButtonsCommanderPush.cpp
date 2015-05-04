/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Basic push button with debounce.>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderPush.hpp"
#include "arduino2.hpp"

ButtonsCommanderPush::ButtonsCommanderPush() : ButtonsCommanderButton()
{
	this->buttonPin = (GPIO_pin_t)DP_INVALID;
	this->lastButtonState = LOW;

	this->lastDebounceTime = 0;
	this->debounceDelay = 50;
}

void ButtonsCommanderPush::Setup(int inButtonPin)
{	
//	CHECKPIN(inButtonPin, "ButtonsCommanderPush::Setup");

	this->buttonPin = Arduino_to_GPIO_pin(inButtonPin);

	pinMode2f(this->buttonPin, INPUT_PULLUP);
}

bool ButtonsCommanderPush::IsPushed()
{
	return digitalRead2f(this->buttonPin) == HIGH;
}

bool ButtonsCommanderPush::Loop()
{
	this->UnselectLastLoop();
	if (this->buttonPin == DP_INVALID)
		return false;

	bool haveChanged = false;
	
	// read the state of the switch into a local variable:
	int reading = digitalRead2f(this->buttonPin);

	// check to see if you just pressed the button 
	// (i.e. the input went from LOW to HIGH),  and you've waited 
	// long enough since the last press to ignore any noise:  

	// If the switch changed, due to noise or pressing:
	if (reading != this->lastButtonState)
	{
		// reset the debouncing timer
		this->lastDebounceTime = millis();
	}

	if (this->lastDebounceTime > 0 && (millis() - this->lastDebounceTime) > this->debounceDelay)
	{
		// whatever the reading is at, it's been there for longer
		// than the debounce delay, so take it as the actual current state:

		// if the button state has changed:
		if (reading != this->buttonState)
		{
			this->buttonState = reading;

			// only toggle the state if the new button state is HIGH
			if (this->buttonState == LOW)
			{
				haveChanged = true;
				this->SelectLastLoop();
#ifdef DEBUG_MODE
				Serial.println(F("Push button pressed "));
#endif
			}
		}
		this->lastDebounceTime = 0;    
	}
  
	// save the reading.  Next time through the loop,
	// it'll be the lastButtonState:
	lastButtonState = reading;
	return haveChanged;
}

void ButtonsCommanderPush::EndLoop()
{
}
