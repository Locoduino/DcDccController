/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco choice window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseDcFreq.hpp"

const byte WindowChooseDcFreq::divisors_11_3_pow2[] { 10, 8, 7, 6, 5, 3, 0, 255 };

#define BASE_PWM_FREQ_11_3	31250
#define NB_PWM_FREQ_11_3	7

byte WindowChooseDcFreq::GetChoiceTextNumber() const
{
	return NB_PWM_FREQ_11_3;
}

char *WindowChooseDcFreq::GetChoiceTextValue(unsigned int indexValue) const
{
	this->BuildFreqIndexString(indexValue);

	return LcdScreen::buffer;
}

void WindowChooseDcFreq::BuildFreqString(unsigned int inDivisor)
{
	unsigned int val = BASE_PWM_FREQ_11_3 / inDivisor;

	LcdScreen::BuildString(val, LcdScreen::buffer);

	int len = strlen(LcdScreen::buffer);
	LcdScreen::buffer[len++] = ' ';
	LcdScreen::buffer[len++] = 'H';
	LcdScreen::buffer[len++] = 'z';
	LcdScreen::buffer[len] = 0;
}

/*void WindowChooseDcFreq::SetCurrentChoice(unsigned int inDivisor)
{
	BuildFreqString(inDivisor);
	WindowChoiceText::SetCurrentChoice(LcdScreen::buffer, inDivisor);
} */

void WindowChooseDcFreq::move(bool inMore)
{
	int i = *(this->pValue);
	if (inMore)
	{
		i++;
		if (divisors_11_3_pow2[i] == 255)
			i = NB_PWM_FREQ_11_3 - 1;
	}
	else
	{
		if (i > 0)
			i--;
	}

	if (i >= 99)
		i = 0;
	SetCurrentChoice(i);
}

