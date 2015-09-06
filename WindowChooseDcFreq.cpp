/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco choice window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseDcFreq.hpp"

const byte divisors_11_3_pow2[] { 10, 8, 7, 6, 5, 3, 0, 255 };

#define BASE_PWM_FREQ_11_3	31250
#define NB_PWM_FREQ_11_3	7

WindowChooseDcFreq::WindowChooseDcFreq(int inFirstLine, int inTag) : WindowChoiceText(inFirstLine, inTag)
{
}

void WindowChooseDcFreq::BuildFreqString(unsigned int inDivisor)
{
	unsigned int val = BASE_PWM_FREQ_11_3 / inDivisor;

	Screen::BuildString(val, Screen::buffer);

	int len = strlen(Screen::buffer);
	Screen::buffer[len++] = ' ';
	Screen::buffer[len++] = 'H';
	Screen::buffer[len++] = 'z';
	Screen::buffer[len] = 0;
}

void WindowChooseDcFreq::SetCurrentChoice(unsigned int inDivisor)
{
	BuildFreqString(inDivisor);
	WindowChoiceText::SetCurrentChoice(Screen::buffer, inDivisor);
}

void WindowChooseDcFreq::move(bool inMore)
{
	int i = 0;
	for (; i < 100; i++)
	{
		if (1 << divisors_11_3_pow2[i] == this->currentValue)
		{
			if (inMore)
			{
				i++;
				if (divisors_11_3_pow2[i] == 255)
					i = NB_PWM_FREQ_11_3 - 1;
				break;
			}
			else
			{
				if (i > 0)
					i--;
				break;
			}
		}
	}
	if (i >= 99)
		i = 0;
	SetCurrentChoice(1 << divisors_11_3_pow2[i]);
}

