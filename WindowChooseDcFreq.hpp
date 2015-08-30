//-------------------------------------------------------------------
#ifndef __windowChooseDcFreq_H__
#define __windowChooseDcFreq_H__
//-------------------------------------------------------------------

#include "DcDccControler.h"

//-------------------------------------------------------------------

#define WINDOWTYPE_FREQCHOICE	12

/*
Choose a frequency for the DC PWM output.
*/

class WindowChooseDcFreq : public WindowChoiceText
{
public:
	WindowChooseDcFreq(int inFirstLine, int inTag = 0);

	static void BuildFreqString(unsigned int inDivisor);
	void SetCurrentChoice(unsigned int inDivisor);
	inline byte GetType() const { return WINDOWTYPE_FREQCHOICE; }
	inline void MoveNextChoice() { move(true); }
	inline void MovePreviousChoice() {	move(false); }

private:
	void move(bool inMore);
};
#endif
//-------------------------------------------------------------------
