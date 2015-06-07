/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one handle>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderPotentiometer.hpp"
#include "WindowLocoControl.hpp"
#include "WindowChooseLoco.hpp"
#include "WindowFunction.hpp"

#include <stdarg.h>

#ifdef DEBUG_MODE
#define CHECK(val, text)	CheckIndex(val, F(text))

void Handle::CheckIndex(int inIndex, const __FlashStringHelper *inFunc)
{
	if (this->functionsSize == 0)
	{
		Serial.print(F("Function handle size undefined in "));
		Serial.println(inFunc);
	}
	else
		if (inIndex < 0 || inIndex >= this->functionsSize)
		{
			Serial.print(F("Function handle index error in "));
			Serial.println(inFunc);
		}
}

#else
#define CHECK(val, text)
#endif

Handle::Handle()
{
	this->id = 0;
	this->pSpeedPotentiometer = 0;
	this->pSpeedEncoder = 0;
	this->pSpeedPushMore = 0;
	this->pSpeedPushLess = 0;
	this->pDirectionPush = 0;
	this->pModeButton = 0;
	this->windowInterruptDcDcc = 0;
	this->windowInterruptEmergency = 0;
	this->Speed = 0;
	this->MoreLessIncrement = 1;
	this->DirectionToLeft = true;
	this->pUi = 0;

	this->pFunctionHandleList = 0;
	this->functionsSize = 0;
	this->functionsAddCounter = 0;

	this->DccIdNbDigits = 4;
	this->ConfigBacklight = true;
}

Handle::Handle(byte inId) : Handle()
{
	this->id = inId;
}

void Handle::Setup(int inNumberOfFunctions)
{
	if (this->pUi == 0)
		this->pUi = new LcdUi();
	if (inNumberOfFunctions > 0)
	{
		this->pFunctionHandleList = new FunctionHandle*[inNumberOfFunctions];
		this->functionsSize = inNumberOfFunctions;
		this->functionsAddCounter = 0;
	}
}

void Handle::Setup(int inNumberOfFunctions, FunctionHandle *inpFirstFunction, ...)
{
	va_list argList;

	this->Setup(inNumberOfFunctions);

	this->AddFunction(inpFirstFunction);
	inNumberOfFunctions--;

	va_start(argList, inpFirstFunction);
	for (; inNumberOfFunctions; inNumberOfFunctions--)
	{
		FunctionHandle *pFunction;
		pFunction = va_arg(argList, FunctionHandle *);
		this->AddFunction(pFunction);
	}
	va_end(argList);
#ifdef DEBUG_MODE
	if (inNumberOfFunctions > 0)
		Serial.println(F("   Not enough items in the list !"));
#endif
}

void Handle::StartUI()
{
	if (this->pUi != 0)
	{
		this->pUi->SetWindowsNumber(24 + Locomotive::FunctionNumber);
		/*
			Splash
			Config							Choice
				Nb digits					EditInt 2-4
				Back light					YesNo
				Reset						YesNo
			LocoControl						WindowLocoControl
			LocoChange						Choose loco (current is selected)
			LocoEdit						Choice
				Edit						Choose loco (current is selected)
						Address long		YesNo
						ID					EditInt	1-127 or 1-10126
						Name				EditText
						Steps				Choice 14/28/128
						Function n			Int
						Function n+1        Int
				Add							
						Address long		YesNo
						ID					EditInt	1-127 or 1-10126
						Name				EditText
						Steps				Choose 14/28/128
						Function n			Int
						Function n+1        Int
				Remove						Choose loco (current is selected)
						Confirm				Confirm
		*/

		WindowSplash *pSplash = (WindowSplash *)this->pUi->AddWindow(new WindowSplash(STR_TITLE, STR_COPYRIGHT));	// menu
		WindowChoice *pChoiceMain = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODEMODECHOICE));	// menu
			pChoiceMain->AddChoice(STR_MODECONFIG);
				WindowChoice *pChoiceConfig = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODECONFIG), pChoiceMain, 0);	// config
				pChoiceConfig->AddChoice(STR_HANDLECFGDIGITS);
					this->pUi->AddWindow(new WindowInt(STR_HANDLECFGDIGITS, 4, 2), pChoiceConfig, 0);
				pChoiceConfig->AddChoice(STR_BACKLIGHTCFG);
					this->pUi->AddWindow(new WindowYesNo(STR_BACKLIGHTCFG), pChoiceConfig, 1);	// config back light
				pChoiceConfig->AddChoice(STR_RESETCONFIG);
					this->pUi->AddWindow(new WindowConfirm(STR_RESETCONFIG, STR_CONFIRM), pChoiceConfig, 2);	// reset config
			pChoiceMain->AddChoice(STR_MODELOCOCTRL);
				this->pUi->AddWindow(new WindowLocoControl(STR_MODELOCOCTRL, this), pChoiceMain, 1); // run
			pChoiceMain->AddChoice(STR_MODELOCOCHANGE);
				this->pUi->AddWindow(new WindowChooseLoco(STR_MODELOCOCHANGE, this), pChoiceMain, 2);
			pChoiceMain->AddChoice(STR_MODELOCOEDIT);
				WindowChoice *pChoiceLocoEdit = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODELOCOEDIT), pChoiceMain, 3);
					pChoiceLocoEdit->AddChoice(STR_LOCOEDIT, STR_SAVELOCO);
						Window *pWinChooseLoco = this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOSELECT, this), pChoiceLocoEdit, 0);
						Window *pWinLocoAddress = this->pUi->AddWindow(new WindowYesNo(STR_LONGADDRESS), pChoiceLocoEdit, 0);
						Window *pWinLocoId = this->pUi->AddWindow(new WindowInt(STR_LOCOID, 10026, 1), pChoiceLocoEdit, 0);
						Window *pWinLocoName = this->pUi->AddWindow(new WindowText(STR_LOCONAME), pChoiceLocoEdit, 0);
						WindowChoice *pChoiceSteps = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOSTEPS), pChoiceLocoEdit, 0);
							pChoiceSteps->AddChoice(STR_LOCOSTEPS14);
							pChoiceSteps->AddChoice(STR_LOCOSTEPS28);
							pChoiceSteps->AddChoice(STR_LOCOSTEPS128);
						for (int i = 0; i < Locomotive::FunctionNumber; i++)
							this->pUi->AddWindow(new WindowFunction(STR_FUNCTIONID, i), pChoiceLocoEdit, 0);
					pChoiceLocoEdit->AddChoice(STR_LOCONEW);
						this->pUi->AddWindow(pWinLocoAddress, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pWinLocoId, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pWinLocoName, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pChoiceSteps, pChoiceLocoEdit, 1);
					pChoiceLocoEdit->AddChoice(STR_LOCOREMOVE);
						this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOREMOVE, this), pChoiceLocoEdit, 2);
						this->pUi->AddWindow(new WindowConfirm(STR_LOCOREMOVE, STR_CONFIRM), pChoiceLocoEdit, 2);

		this->windowInterruptDcDcc = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_DCDCC, STR_DCDCC2))); // Dc/Dcc mode change
		this->windowInterruptEmergency = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_STOP, STR_STOP2))); // Emergency stop
		this->windowInterruptSaveLoco = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterruptConfirm(STR_SAVELOCO, STR_CONFIRM))); // Save the loco after modif
	}
}

void Handle::StartContent()
{
	this->controled.SetFunctionsSize(Locomotive::FunctionNumber);
	this->edited.SetFunctionsSize(Locomotive::FunctionNumber);
}
	
void Handle::AddFunction(FunctionHandle *pFunction)
{
	CHECK(this->functionsAddCounter, "Handle::AddFunction");
	this->pFunctionHandleList[this->functionsAddCounter++] = pFunction;
}

#define DEBUG_MODE

void Handle::SetLocomotive(const Locomotive &inLocomotive)
{
#ifdef DEBUG_MODE
	Serial.println(F("SetLocomotive"));
#endif
	this->controled.Copy(inLocomotive);
	if (this->pSpeedPotentiometer != 0)
		((ButtonsCommanderPotentiometer *) this->pSpeedPotentiometer)->SetMinMax(0, inLocomotive.GetSteps());
}

void Handle::Interrupt(int inEvent)
{
	switch(inEvent)
	{
	case EVENT_DCDCC:		this->pUi->Interrupt(this->windowInterruptDcDcc);		break;
	case EVENT_EMERGENCY:	this->pUi->Interrupt(this->windowInterruptEmergency);	break;
	}	
}

void Handle::InterruptEnd()
{
	this->pUi->InterruptEnd();
}

bool Handle::Loop()
{
	bool changed = false;

	int event = EVENT_NONE;

	if (this->pSpeedPotentiometer != 0)
		if (this->pSpeedPotentiometer->IsSelectedLastLoop())
		{
			event = EVENT_MOVE;
			this->Speed = this->pSpeedPotentiometer->GetPosition();
			this->pSpeedPotentiometer->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Speed Potentiometer Moved"));
#endif
		}

	if (this->pSpeedEncoder != 0)
		if (this->pSpeedEncoder->IsSelectedLastLoop())
		{
			event = EVENT_MOVE;
			ButtonsCommanderEncoder *pEncoder = (ButtonsCommanderEncoder*) this->pSpeedEncoder;
			if (pEncoder->WasLastPositive())
				event = EVENT_MORE;
			else
				event = EVENT_LESS;

			this->pSpeedEncoder->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Speed Encoder moved"));
#endif
		}

	if (this->pSpeedPushMore != 0)
		if (this->pSpeedPushMore->IsSelectedLastLoop())
		{
			event = EVENT_MORE;
			this->pSpeedPushMore->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("SpeedPushMore"));
#endif
		}

	if (this->pSpeedPushLess != 0)
		if (this->pSpeedPushLess->IsSelectedLastLoop())
		{
			event = EVENT_LESS;
			this->pSpeedPushLess->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("SpeedPushLess"));
#endif
		}

	if (this->pDirectionPush != 0)
		if (this->pDirectionPush->IsSelectedLastLoop())
		{
			event = EVENT_SELECT;
			this->pDirectionPush->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Button select/dir pressed"));
#endif
		}

	if (this->pModeButton != 0)
		if (this->pModeButton->IsSelectedLastLoop())
		{
			event = EVENT_CANCEL;
			this->pModeButton->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Button Cancel pressed"));
#endif
		}

	for (int i = 0; i < this->GetFunctionNumber(); i++)
	{
		if (this->pFunctionHandleList[i]->Loop())
		{
			this->ToggleFunction(i);
#ifdef DEBUG_MODE
			Serial.print(F("Function button "));
			Serial.print(i+1);
			Serial.println(F(" pressed"));
#endif
		}
	}

	if (pUi->Loop(event))
	{
		Window *pCurrent = pUi->GetGlobalCurrentWindow();
		Locomotive &loco = this->edited;

#ifdef DEBUG_MODE
		//LcdUi::printEvent(event, F("pUi->Loop"));
		//Window::printState(pUi->GetState(), F("pUi->Loop"));
#endif

		switch (pUi->GetState())
		{
		case STATE_INITIALIZE:
			switch (pUi->GetWindowId())
			{
			case STR_BACKLIGHTCFG:
				pCurrent->SetChoiceValue(this->ConfigBacklight ? Screen::YesMsg : Screen::NoMsg);
				break;
			case STR_HANDLECFGDIGITS:
				pCurrent->SetValue(this->DccIdNbDigits);
				break;
			case STR_LONGADDRESS:
				pCurrent->SetValue(loco.GetDccAddressKind());
				break;
			case STR_LOCOID:
				pCurrent->SetValue(loco.GetDccId());
				break;
			case STR_LOCONAME:
				pCurrent->SetValue(loco.GetName());
				break;
			case STR_LOCOSTEPS:
				pCurrent->SetValue(loco.GetSteps());
				break;
			case STR_FUNCTIONID:
				pCurrent->SetValue(loco.GetFunctionFromIndex(pCurrent->GetTag()).GetDccId());
				break;
			}
			break;

		case STATE_CONFIRMED:
			switch (pUi->GetWindowId())
			{
			case STR_BACKLIGHTCFG:
				this->ConfigBacklight = pCurrent->GetChoiceValue() == Screen::YesMsg;
				this->ConfigSave();
				break;

			case STR_HANDLECFGDIGITS:
				this->DccIdNbDigits = pCurrent->GetIntValue();
				this->ConfigSave();
				break;

			case STR_RESETCONFIG:
				DDC.ResetConfig();
				break;

			case STR_MODELOCOCHANGE:
				DCCItemList.GetLoco(pCurrent->GetChoiceValue(), &this->controled);
				this->ConfigSave();
				break;

			case STR_LOCOSELECT:
				DCCItemList.GetLoco(pCurrent->GetChoiceValue(), &this->edited);
				break;

			case STR_MODELOCOEDIT:
				switch (pCurrent->GetChoiceValue())
				{
				case STR_LOCONEW:
					this->edited.Clear();
					break;

				case STR_LOCOREMOVE:
					if (pCurrent->GetType() == WINDOWTYPE_CONFIRM)
					{
						WindowConfirm *pConfirm = (WindowConfirm *)pCurrent;

						if (pConfirm->GetChoiceValue() == STR_YES)
						{
							DCCItemList.FreeLoco(&this->edited);
							this->edited.Clear();
							WindowChooseLoco::SetSelectedLoco(255);
							WindowChooseLoco::RebuildChoices();
						}
					}
					else
						DCCItemList.GetLoco(pCurrent->GetChoiceValue(), &this->edited);
					break;

				}
				break;

			case STR_LONGADDRESS:
				loco.SetDccAddressKind(pCurrent->GetIntValue());
				break;
			case STR_LOCOID:
				loco.SetDccId(pCurrent->GetIntValue());
				break;
			case STR_LOCONAME:
				loco.SetName(pCurrent->GetTextValue());
				break;
			case STR_LOCOSTEPS:
				loco.SetSteps(pCurrent->GetChoiceValue());
				break;
			case STR_FUNCTIONID:
				loco.GetFunctionFromIndex(pCurrent->GetTag()).SetDccId(pCurrent->GetIntValue());
				break;

			case STR_SAVELOCO:
				if (pCurrent->GetChoiceValue() == STR_YES)
				{
					DCCItemList.UpdateLoco(&this->edited);
					WindowChooseLoco::RebuildChoices();
				}
				break;
			}
			pUi->SetState(STATE_POSTCONFIRMED);
			break;

		}
	}

	return changed;
}

void Handle::SetSpeed(int inNewSpeed)
{
	this->Speed = inNewSpeed;
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->SetSpeed(this->Speed);
}

void Handle::SetDirection(bool inToLeft)
{
	this->DirectionToLeft = inToLeft;
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->SetDirection(this->DirectionToLeft);
}

void Handle::ToggleFunction(int inFunctionNumber)
{
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->ToggleFunction(inFunctionNumber);
}

void Handle::ConfigLoad()
{
	byte i = DDC.IndexOf(this);
	int pos= EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	this->ConfigBacklight = EEPROMextent.read(pos) > 0;
	this->DccIdNbDigits = EEPROMextent.read(pos + 1);
	byte slot = EEPROMextent.read(pos + 2);
	if (slot < DCCItemList.ListSize / DCCItemList.ItemSize)
		DCCItemList.GetLoco(slot, &this->controled);
}

void Handle::ConfigSave()
{
	byte i = DDC.IndexOf(this);
	int pos = EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	EEPROMextent.write(pos, (byte) this->ConfigBacklight);
	EEPROMextent.write(pos + 1, this->DccIdNbDigits);
	EEPROMextent.write(pos + 2, this->controled.GetSlotNumber());
}