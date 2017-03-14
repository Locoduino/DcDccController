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
	static const byte divisors_11_3_pow2[];

	WindowChooseDcFreq() {}
	void begin(int inFirstLine, byte *inpFreq) { WindowChoiceText::begin(inFirstLine, inpFreq); }

	byte GetChoiceTextNumber() const;
	char *GetChoiceTextValue(unsigned int indexValue) const;

	static void BuildFreqString(unsigned int inDivisor);
	static void BuildFreqIndexString(byte inIndexValue) { BuildFreqString(1 << divisors_11_3_pow2[inIndexValue]); }
	inline void BuildFreqString() { return BuildFreqIndexString(*(this->pValue)); }
	inline byte GetType() const { return WINDOWTYPE_FREQCHOICE; }
	inline void MoveNextChoice() { move(true); }
	inline void MovePreviousChoice() {	move(false); }
	inline static unsigned int GetFrequencyDivisor(byte inIndexValue) { return 1 << divisors_11_3_pow2[inIndexValue]; }
	inline unsigned int GetFrequencyDivisor() const { return GetFrequencyDivisor(*(this->pValue)); }

private:
	void move(bool inMore);
};
#endif
//-------------------------------------------------------------------
