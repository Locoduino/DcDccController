//-------------------------------------------------------------------
#ifndef __buttonsCommanderSwitch_H__
#define __buttonsCommanderSwitch_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
#include "VStudioLcdUi\arduino2.hpp"
#else
#include "arduino2.hpp"
#endif
#include "ButtonsCommanderButton.hpp"

//-------------------------------------------------------------------

#define SWITCH(list, nb)	((ButtonsCommanderSwitch *) list[nb])

class ButtonsCommanderSwitch : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t pin;

	unsigned long debounceDelay;    // the debounce time; increase if the output flickers
	unsigned long lastDebounceTime;  // the last time the output pin was toggled

	bool buttonState;       // the current reading from the input pin
	bool lastButtonState;   // the previous reading from the input pin. Initialized from current value during setup.
	
public:
	ButtonsCommanderSwitch();
	
	void Setup(int inPin);
	GPIO_pin_t GetPin() const { return this->pin; }
	bool GetLastButtonState() const { return this->lastButtonState; }
	bool Loop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
