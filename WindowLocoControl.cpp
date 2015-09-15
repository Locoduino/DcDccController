/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for a loco control window>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseDcFreq.hpp"
#include "WindowLocoControl.hpp"

WindowLocoControl::WindowLocoControl(int inFirstLine, Handle *inpHandle) : Window(inFirstLine)
{
	this->pHandle = inpHandle;
	this->dcMsg = STR_DC;
	this->dccMsg = STR_DCC;
}

// #define DEBUG_MODE

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
		if (DDC.dcType == Dc)
		{
			inpLcd->GetScreen()->DisplayHeader(((ControlerDc *)DDC.pControler)->IsSlowMode() ? STR_DCSLOW : this->dcMsg);
			WindowChooseDcFreq::BuildFreqString(((ControlerDc *)DDC.pControler)->GetFrequencyDivisor());
			char text[20];
			STRNCPY(text, 20, Screen::buffer);
			byte len = Screen::BuildStringLeft(text, inpLcd->GetScreen()->GetSizeX() - 4, Screen::buffer);
			inpLcd->GetScreen()->setCursor(inpLcd->GetScreen()->GetSizeX() - len, 0);
			inpLcd->GetScreen()->print(Screen::buffer);
		}
		else
		{
			inpLcd->GetScreen()->DisplayHeader(this->dccMsg);
			inpLcd->GetScreen()->setCursor(3, 0);
			inpLcd->GetScreen()->write(32);
			inpLcd->GetScreen()->setCursor(4, 0);
			Screen::BuildString(this->pHandle->GetControledLocomotive().GetDccId(), Screen::buffer, this->pHandle->DccIdNbDigits);
			inpLcd->GetScreen()->print(Screen::buffer);
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
	if (DDC.dcType == Dcc)
	{
#ifdef DEBUG_MODE
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
		steps = ((ControlerDc *)DDC.pControler)->GetMaxSpeed();
	}

	switch (inEventType)
	{
		case EVENT_MORE:
			{
#ifdef DEBUG_MODE
			Serial.print(F("MORE "));
#endif
			unsigned int newValue = abs(this->pHandle->GetControledLocomotive().GetMappedSpeed()) + inc;
			if (newValue > steps)
				newValue = steps;
			this->pHandle->SetSpeed(newValue);
#ifdef DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

		case EVENT_LESS:
			{
#ifdef DEBUG_MODE
			Serial.print(F("LESS "));
#endif
			int newValue = abs(this->pHandle->GetControledLocomotive().GetMappedSpeed()) - inc;
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
#ifdef DEBUG_MODE
			Serial.println(newValue);
#endif
			}
			showValue = true;
			break;

#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
		case EVENT_MOVE:
		{
#ifdef DEBUG_MODE
			Serial.print(F("MOVE "));
#endif
			int newValue = this->pHandle->pSpeedPotentiometer->GetPosition();
			if (newValue < 0)
				newValue = 0;
			this->pHandle->SetSpeed(newValue);
#ifdef DEBUG_MODE
			Serial.println(newValue);
#endif
		}
		showValue = true;
		break;
#endif

		case EVENT_SELECT:
#ifdef DEBUG_MODE
			Serial.println(F("SELECT"));
#endif
			this->pHandle->SetDirection(!this->pHandle->GetControledLocomotive().GetDirectionToLeft());
			showValue = true;
			break;
		case EVENT_CANCEL:
#ifdef DEBUG_MODE
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
		Screen::BuildProgress(speed, DDC.pControler->GetMaxSpeed(), 
			this->pHandle->GetControledLocomotive().GetDirectionToLeft(), inpLcd->GetScreen()->GetSizeX(), Screen::buffer);
		inpLcd->GetScreen()->setCursor(0, 1);
		inpLcd->GetScreen()->print(Screen::buffer);
	}
}

