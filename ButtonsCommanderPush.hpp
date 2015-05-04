//-------------------------------------------------------------------
#ifndef __buttonsCommanderPush_H__
#define __buttonsCommanderPush_H__
//-------------------------------------------------------------------

#define  GPIO2_PREFER_SPEED    1
#ifdef VISUALSTUDIO
	#include "VStudioLcdUI\arduino2.hpp"
#else
	#include "arduino2.hpp"
#endif

#include "ButtonsCommanderButton.hpp"

#define PUSH(list, nb)	((ButtonsCommanderPush *) list[nb])

//-------------------------------------------------------------------
// A push button is a hardware device giving a time limited impulsion.
//-------------------------------------------------------------------

class ButtonsCommanderPush : public ButtonsCommanderButton
{
 private:
	GPIO_pin_t buttonPin;	// the number of the pushbutton pin
	int buttonState;		// the current reading from the input pin
	int lastButtonState;	// the previous reading from the input pin

	unsigned long lastDebounceTime;  // the last time the output pin was toggled
	unsigned long debounceDelay;    // the debounce time; increase if the output flickers

 public:
	ButtonsCommanderPush();

	void Setup(int inButtonPin);
	bool IsPushed();
	bool Loop();
	void EndLoop();

	inline int GetState() const { return this->buttonState; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
