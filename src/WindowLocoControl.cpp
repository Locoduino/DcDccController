/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco control window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseDcFreq.hpp"
#include "WindowLocoControl.hpp"

void WindowLocoControl::begin(int inFirstLine, Handle *inpHandle)
{
	this->Window::begin(inFirstLine);

	this->pHandle = inpHandle;
	this->dcMsg = STR_DC;
	this->dccMsg = STR_DCC;
}

// #define DDC_DEBUG_MODE

void WindowLocoControl::Event(byte inEventType, LcdUi *inpLcd)
{
	bool showValue = false;

	if (this->state == STATE_INITIALIZE)
	{
		this->state = STATE_NONE;
		showValue = true;
	}

	if (this->state == STATE_START)
	{
		if (DcDccControler::dcType == Dc)
		{
			inpLcd->GetScreen()->DisplayHeader(((ControlerDc *)DcDccControler::pControler)->IsSlowMode() ? STR_DCSLOW : this->dcMsg);
			WindowChooseDcFreq::BuildFreqString(((ControlerDc *)DcDccControler::pControler)->GetFrequencyDivisor());
			byte len = LcdScreen::BuildStringLeft(LcdScreen::buffer, inpLcd->GetScreen()->GetSizeX() - 4, LcdScreen::buffer);
			inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, inpLcd->GetScreen()->GetSizeX() - len, 0);
		}
		else
		{
			inpLcd->GetScreen()->DisplayHeader(this->dccMsg);
			inpLcd->GetScreen()->DisplayText(" ", 3, 0);
			LcdScreen::BuildString(this->pHandle->GetControledLocomotive().GetDccId(), LcdScreen::buffer, this->pHandle->DccIdNbDigits);
			inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, 4, 0);
#ifndef NANOCONTROLER
			inpLcd->GetScreen()->setCursor(4 + this->pHandle->DccIdNbDigits, 0);
			inpLcd->GetScreen()->write(32);
			byte len = Screen::BuildStringLeft(this->pHandle->GetControledLocomotive().GetName(), inpLcd->GetScreen()->GetSizeX() - (4 + this->pHandle->DccIdNbDigits + 1), Screen::buffer);
			inpLcd->GetScreen()->setCursor(inpLcd->GetScreen()->GetSizeX() - len, 0);
			inpLcd->GetScreen()->print(Screen::buffer);
#endif
		}

		this->state = STATE_INITIALIZE;
	}

	byte inc;
	byte steps = this->pHandle->GetControledLocomotive().GetSteps();
	if (DcDccControler::dcType == Dcc)
	{
#ifdef DDC_DEBUG_MODE
		//Serial.println(steps);
#endif
		switch (steps)
		{
		case 128: inc = this->pHandle->MoreLessIncrement; break;
		default: inc = 1; break;
		}
	}
	else
	{
		inc = this->pHandle->MoreLessIncrement;
		steps = ((ControlerDc *)DcDccControler::pControler)->GetMaxSpeed();
	}

	switch (inEventType)
	{
		case EVENT_MORE:
			{
#ifdef DDC_DEBUG_MODE
			Serial.print(F("MORE "));
#endif
			unsigned int newValue = abs(this->pHandle->GetControledLocomotive().GetMappedSpeed()) + inc;
			if (newValue > steps)
				newValue = steps;
			this->pHandle->SetSpeed(newValue);
#ifdef DDC_DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

		case EVENT_LESS:
			{
#ifdef DDC_DEBUG_MODE
			Serial.print(F("LESS "));
#endif
			int newValue = abs(this->pHandle->GetControledLocomotive().GetMappedSpeed()) - inc;
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
#ifdef DDC_DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

			/*
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
		case EVENT_MOVE:
		{
#ifdef DDC_DEBUG_MODE
			Serial.print(F("MOVE "));
#endif
			int newValue = this->pHandle->pSpeedPotentiometer->GetPosition();
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
#ifdef DDC_DEBUG_MODE
			Serial.println(newValue);
#endif
		}
		showValue = true;
		break;
#endif
			  */

		case EVENT_SELECT:
#ifdef DDC_DEBUG_MODE
			Serial.println(F("SELECT"));
#endif
			this->pHandle->SetDirection(!this->pHandle->GetControledLocomotive().GetDirectionToLeft());
			showValue = true;
			break;
		case EVENT_CANCEL:
#ifdef DDC_DEBUG_MODE
			Serial.println(F("CANCEL"));
#endif
			this->state = STATE_ABORTED;
			break;
	}

	if (showValue)
	{
		//   01234567879012345
		// 0 Dcc 003 nomloco11
		// 1 +>>>>>			 -
		//   01234567879012345
		int speed = abs(this->pHandle->GetControledLocomotive().GetMappedSpeed());
		if (speed == 1)
			speed = 0;
		LcdScreen::BuildProgress(speed, DcDccControler::pControler->GetMaxSpeed(),
			this->pHandle->GetControledLocomotive().GetDirectionToLeft(), inpLcd->GetScreen()->GetSizeX(), LcdScreen::buffer);
		inpLcd->GetScreen()->DisplayText(LcdScreen::buffer, 0, 1);
	}
}

