/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one handle>
*************************************************************/

#include "DcDccControler.h"
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "ButtonsCommanderPotentiometer.hpp"
#endif
#include "WindowLocoControl.hpp"
#include "WindowChooseLoco.hpp"
#include "WindowChooseDcFreq.hpp"
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
//	this->id = 0;
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
	this->pSpeedPotentiometer = 0;
#endif
	this->pSpeedEncoder = 0;
	this->pSpeedPushMore = 0;
	this->pSpeedPushLess = 0;
	this->pDirectionPush = 0;
	this->pCancelButton = 0;
	this->windowInterruptDcDcc = 0;
	this->windowInterruptEmergency = 0;
	//this->Speed = 0;
	this->MoreLessIncrement = 1;
	//this->DirectionToLeft = true;
	this->pUi = 0;

	this->pFunctionHandleList = 0;
	this->functionsSize = 0;
	this->functionsAddCounter = 0;

	this->DccIdNbDigits = 4;
}

/*Handle::Handle(byte inId) : Handle()
{
	this->id = inId;
}*/

void Handle::Setup(byte inNumberOfFunctions)
{
	if (this->pUi == 0)
		this->pUi = new LcdUi();
	if (inNumberOfFunctions > 0)
	{
		this->pFunctionHandleList = new FunctionHandle*[inNumberOfFunctions];
		this->functionsSize = inNumberOfFunctions;
		this->functionsAddCounter = 0;
		this->controled.SetFunctionsSize(inNumberOfFunctions);
#ifdef NANOCONTROLER
		Function dummy;
		for (int i = 0; i < inNumberOfFunctions; i++)
			this->controled.AddFunction(dummy);

#else
		this->edited.SetFunctionsSize(inNumberOfFunctions);
#endif
	}
}

void Handle::Setup(byte inNumberOfFunctions, FunctionHandle *inpFirstFunction, ...)
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
#ifndef NANOCONTROLER
	{
		this->pUi->SetWindowsNumber(24 + (2 * Locomotive::FunctionNumber));
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

		// Master version
		WindowSplash *pSplash = (WindowSplash *)this->pUi->AddWindow(new WindowSplash(STR_TITLE, STR_COPYRIGHT));	// menu
		WindowChoice *pChoiceMain = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODEMODECHOICE, 4, false, 0));	// menu
		pChoiceMain->AddChoice(STR_MODECONFIG);
		WindowChoice *pChoiceConfig = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODECONFIG, 3, false), pChoiceMain);	// config
		pChoiceConfig->AddChoice(STR_HANDLECFGDIGITS);
		this->pUi->AddWindow(new WindowInt(STR_HANDLECFGDIGITS, 4, 2), pChoiceConfig, 0);
		pChoiceConfig->AddChoice(STR_PWMFREQCFG);
		this->pUi->AddWindow(new WindowChooseDcFreq(STR_PWMFREQCFG), pChoiceConfig, 1);	// DC Freq
		pChoiceConfig->AddChoice(STR_RESETCONFIG);
		this->pUi->AddWindow(new WindowConfirm(STR_RESETCONFIG, STR_CONFIRM), pChoiceConfig, 2);	// reset config
		pChoiceMain->AddChoice(STR_MODELOCOCTRL);
		this->pUi->AddWindow(new WindowLocoControl(STR_MODELOCOCTRL, this), pChoiceMain, 1); // run
		pChoiceMain->AddChoice(STR_MODELOCOCHANGE);
		this->pUi->AddWindow(new WindowChooseLoco(STR_MODELOCOCHANGE, this), pChoiceMain, 2);
		pChoiceMain->AddChoice(STR_MODELOCOEDIT);
		WindowChoice *pChoiceLocoEdit = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODELOCOEDIT, 3, true), pChoiceMain, 3);
		pChoiceLocoEdit->AddChoice(STR_LOCOEDIT, STR_SAVELOCO);
		Window *pWinChooseLoco = this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOSELECT, this), pChoiceLocoEdit, 0);
		Window *pWinLocoAddress = this->pUi->AddWindow(new WindowYesNo(STR_LONGADDRESS), pChoiceLocoEdit, 0);
		Window *pWinLocoId = this->pUi->AddWindow(new WindowInt(STR_LOCOID, 10026, 1), pChoiceLocoEdit, 0);
		Window *pWinLocoName = this->pUi->AddWindow(new WindowText(STR_LOCONAME), pChoiceLocoEdit, 0);
		WindowChoice *pChoiceSteps = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOSTEPS, 3, false), pChoiceLocoEdit, 0);
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
		for (int i = 0; i < Locomotive::FunctionNumber; i++)
			this->pUi->AddWindow(new WindowFunction(STR_FUNCTIONID, i), pChoiceLocoEdit, 1);
		pChoiceLocoEdit->AddChoice(STR_LOCOREMOVE);
		this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOREMOVE, this), pChoiceLocoEdit, 2);
		this->pUi->AddWindow(new WindowConfirm(STR_LOCOREMOVE, STR_CONFIRM), pChoiceLocoEdit, 2);

		this->windowInterruptDcDcc = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_DCDCC, STR_DCDCC2))); // Dc/Dcc mode change
		this->windowInterruptEmergency = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_STOP, STR_STOP2))); // Emergency stop
		this->windowInterruptSaveLoco = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterruptConfirm(STR_SAVELOCO, STR_CONFIRM))); // Save the loco after modif
	}
#else
	{
		this->pUi->SetWindowsNumber(12 + Locomotive::FunctionNumber);
		/*
		In Dc :
		0	Splash
		1		Mode Choice				Choice
		2			Config DDC			Choice
		3				PWM freq		WindowChooseDcFreq
		4			LocoControl			WindowLocoControl
		5	Dc-Dcc						Interrupt
		6	Stop						Interrupt
		
		In Dcc :
		0	Splash
		1		Mode Choice				Choice
		2			Config DDC			Choice
		3				Nb digits		EditInt 2-4
		4				Reset			Confirm
		5			Loco Edit			Choice
		6				Address long	YesNo
		7				ID				EditInt	1-127 or 1-10126
		8				Steps			Choice 14/28/128
		9				Function n		EditInt	1-127 or 1-10126
		10				Function n+1	EditInt	1-127 or 1-10126
		11			LocoControl			WindowLocoControl
		12	Dc-Dcc						Interrupt
		13	Stop						Interrupt
		*/

		WindowSplash *pSplash = (WindowSplash *)this->pUi->AddWindow(new WindowSplash(STR_TITLE, STR_COPYRIGHT));	// splash
		WindowChoice *pChoiceMain = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODEMODECHOICE, 2, false));	// menu
		int nb = pChoiceMain->AddChoice(STR_MODECONFIG);
			WindowChoice *pChoiceConfig = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_MODECONFIG, 2, false), pChoiceMain, nb);	// config DDC
			if (DDC.dcType == Dc)
			{
				nb = pChoiceConfig->AddChoice(STR_PWMFREQCFG);
					this->pUi->AddWindow(new WindowChooseDcFreq(STR_PWMFREQCFG), pChoiceConfig, nb);	// DC Freq
			}
			else
			{
				nb = pChoiceConfig->AddChoice(STR_HANDLECFGDIGITS);
					this->pUi->AddWindow(new WindowInt(STR_HANDLECFGDIGITS, 4, 2), pChoiceConfig, nb);
				nb = pChoiceConfig->AddChoice(STR_RESETCONFIG);
					this->pUi->AddWindow(new WindowConfirm(STR_RESETCONFIG, STR_CONFIRM), pChoiceConfig, nb);	// reset config
				nb = pChoiceMain->AddChoice(STR_LOCOEDIT);
					WindowChoice *pChoiceConfigLoco = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOEDIT, 3+ Locomotive::FunctionNumber, false, true), pChoiceMain, nb);	// config loco
						nb = pChoiceConfigLoco->AddChoice(STR_LONGADDRESS);
							Window *pWinLocoAddress = this->pUi->AddWindow(new WindowYesNo(STR_LONGADDRESS), pChoiceConfigLoco, nb);
						nb = pChoiceConfigLoco->AddChoice(STR_LOCOID);
							Window *pWinLocoId = this->pUi->AddWindow(new WindowInt(STR_LOCOID, 10026, 1), pChoiceConfigLoco, nb);
						nb = pChoiceConfigLoco->AddChoice(STR_LOCOSTEPS);
							WindowChoice *pChoiceSteps = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOSTEPS, 3, false), pChoiceConfigLoco, nb);
								pChoiceSteps->AddChoice(STR_LOCOSTEPS14);
								pChoiceSteps->AddChoice(STR_LOCOSTEPS28);
								pChoiceSteps->AddChoice(STR_LOCOSTEPS128);
						for (int i = 0; i < Locomotive::FunctionNumber; i++)
						{
							nb = pChoiceConfigLoco->AddChoice(STR_FUNCTIONID, i+1);
								this->pUi->AddWindow(new WindowInt(STR_FUNCTIONID, 12, 1, i), pChoiceConfigLoco, nb);
						}
			}
		nb = pChoiceMain->AddChoice(STR_MODELOCOCTRL);
			this->pUi->AddWindow(new WindowLocoControl(STR_MODELOCOCTRL, this), pChoiceMain, nb); // run

		this->windowInterruptDcDcc = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_DCDCC, STR_DCDCC2))); // Dc/Dcc mode change
		this->windowInterruptEmergency = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_STOP, STR_STOP2))); // Emergency stop
	}
#endif
}

void Handle::EndSetup(bool inDcMode)
{
	this->SetSpeed(0);
	byte ControlIndex = 255;
	for (ControlIndex = 0; ControlIndex < this->pUi->GetWindowNumber(); ControlIndex++)
		if (this->pUi->GetWindow(ControlIndex)->GetType() == WINDOWTYPE_LOCOCONTROL)
			break;

	// If Dc goto to control directly
	if (inDcMode)
	{
		this->pUi->SetWindow(ControlIndex);
	}
	else
	// If Dcc
	//	If loco configurated, goto control
	//	else goto Config.
	{
		if (this->controled.GetSlotNumber() != 255)
			this->pUi->SetWindow(ControlIndex);
	}

}

void Handle::StartContent()
{
	this->controled.SetFunctionsSize(Locomotive::FunctionNumber);
#ifndef NANOCONTROLER
	this->edited.SetFunctionsSize(Locomotive::FunctionNumber);
#endif
}
	
void Handle::AddFunction(FunctionHandle *pFunction)
{
	CHECK(this->functionsAddCounter, "Handle::AddFunction");
	this->pFunctionHandleList[this->functionsAddCounter++] = pFunction;
}

//#define DEBUG_MODE

void Handle::SetControledLocomotive(const Locomotive &inLocomotive)
{
#ifdef DEBUG_MODE
	Serial.println(F("SetLocomotive"));
#endif
	this->controled.Copy(inLocomotive);
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
	if (this->pSpeedPotentiometer != 0)
		((ButtonsCommanderPotentiometer *) this->pSpeedPotentiometer)->SetMinMax(0, inLocomotive.GetSteps());
#endif
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

#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
	if (this->pSpeedPotentiometer != 0)
		if (this->pSpeedPotentiometer->IsSelectedLastLoop())
		{
			event = EVENT_MOVE;
			this->pSpeedPotentiometer->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Speed Potentiometer Moved"));
#endif
		}
#endif

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

	if (this->pCancelButton != 0)
		if (this->pCancelButton->IsSelectedLastLoop())
		{
			event = EVENT_CANCEL;
			this->pCancelButton->UnselectLastLoop();
#ifdef DEBUG_MODE
			Serial.println(F("Button Cancel pressed"));
#endif
		}

	for (int i = 0; i < this->GetFunctionNumber(); i++)
	{
		if (this->pFunctionHandleList[i]->Loop())
		{
			if (DDC.pControler->GetType() == Dcc)
			{
				this->ToggleFunction(i);
#ifdef DEBUG_MODE
				Serial.print(F("Function button "));
				Serial.print(i + 1);
				Serial.println(F(" pressed"));
#endif
			}
			else
			{
				if (i == 0)
				{
					Window *pCurrent = pUi->GetGlobalCurrentWindow();
					((ControlerDc *)DDC.pControler)->SetSlowMode(!((ControlerDc *)DDC.pControler)->IsSlowMode());
					if (((ControlerDc *)DDC.pControler)->IsSlowMode())
						this->MoreLessIncrement = 5;
					else
						this->MoreLessIncrement = 10;
					pCurrent->SetState(STATE_START);
				}
			}
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
			case STR_PWMFREQCFG:
				((WindowChooseDcFreq *)pCurrent)->SetCurrentChoice(((ControlerDc *)DDC.pControler)->GetFrequencyDivisor());
				break;
			case STR_HANDLECFGDIGITS:
				((WindowInt*)pCurrent)->SetValue(this->DccIdNbDigits);
				break;
			case STR_LONGADDRESS:
				((WindowInt*)pCurrent)->SetValue(loco.GetDccAddressKind());
				break;
			case STR_LOCOID:
				((WindowInt*)pCurrent)->SetValue(loco.GetDccId());
				break;
#ifndef NANOCONTROLER
			case STR_LOCONAME:
				((WindowText*)pCurrent)->SetValue(loco.GetName());
				break;
#endif
			case STR_LOCOSTEPS:
				int val;
				switch (loco.GetSteps())
				{
				case 14: val = STR_LOCOSTEPS14; break;
				case 28: val = STR_LOCOSTEPS28; break;
				default: val = STR_LOCOSTEPS128; break;
				}
				((WindowChoice*)pCurrent)->SetChoiceValue(val);
				break;
			case STR_FUNCTIONID:
				((WindowInt*)pCurrent)->SetValue(loco.GetFunctionFromIndex(pCurrent->GetTag()).GetDccId());
				break;
			}
			break;

		case STATE_CONFIRMED:
			switch (pUi->GetWindowId())
			{
			case STR_PWMFREQCFG:
				((ControlerDc *)DDC.pControler)->SetFrequencyDivisor(((WindowChooseDcFreq *)pCurrent)->GetChoiceIntValue());
				DDC.ConfigSave();
				break;

			case STR_HANDLECFGDIGITS:
				this->DccIdNbDigits = ((WindowInt*)pCurrent)->GetIntValue();
				this->ConfigSave();
				break;

			case STR_RESETCONFIG:
				DDC.ConfigReset();
				break;

#ifndef NANOCONTROLER
			case STR_MODELOCOCHANGE:
				DCCItemList.GetLoco(pCurrent->GetChoiceValue(), &this->controled);
				this->ConfigSave();
				break;

			case STR_LOCOSELECT:
				DCCItemList.GetLoco(pCurrent->GetChoiceValue(), &this->edited);
				this->ConfigSave();
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
#endif

			case STR_LONGADDRESS:
				loco.SetDccAddressKind(((WindowInt*)pCurrent)->GetIntValue());
#ifdef NANOCONTROLER
				this->ConfigSave();
#endif
				break;
			case STR_LOCOID:
				loco.SetDccId(((WindowInt*)pCurrent)->GetIntValue());
#ifdef NANOCONTROLER
				this->ConfigSave();
#endif
				break;
#ifndef NANOCONTROLER
			case STR_LOCONAME:
				loco.SetName(((WindowText*) pCurrent)->GetTextValue());
				break;
#endif
			case STR_LOCOSTEPS:
				int val;
				switch (pCurrent->GetChoiceValue())
				{
					case STR_LOCOSTEPS14: val = 14; this->MoreLessIncrement = 1; break;
					case STR_LOCOSTEPS28: val = 28; this->MoreLessIncrement = 1; break;
					default: val = 128; this->MoreLessIncrement = 10; break;
				}
				loco.SetSteps(val);
#ifdef NANOCONTROLER
				this->ConfigSave();
#endif
				break;
			case STR_FUNCTIONID:
				loco.GetFunctionFromIndex(pCurrent->GetTag()).SetDccId(((WindowInt*)pCurrent)->GetIntValue());
#ifdef NANOCONTROLER
				this->ConfigSave();
#endif
				break;

#ifndef NANOCONTROLER
			case STR_SAVELOCO:
				if (pCurrent->GetChoiceValue() == STR_YES)
				{
					DCCItemList.UpdateLoco(&this->edited);
					WindowChooseLoco::RebuildChoices();
				}
				break;
#endif
			}
			pUi->SetState(STATE_POSTCONFIRMED);
			break;

		}
	}

	return changed;
}

void Handle::SetSpeed(int inNewSpeed)
{
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->SetSpeed(inNewSpeed);
}

void Handle::SetDirection(bool inToLeft)
{
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->SetDirection(inToLeft);
}

void Handle::ToggleFunction(byte inFunctionNumber)
{
	DDC.pControler->SetControled(&this->controled);
	DDC.pControler->ToggleFunction(inFunctionNumber);
}

void Handle::ConfigLoad()
{
	byte i = DDC.IndexOf(this);
	int pos= EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	if (DDC.dcType == Dcc)
	{
		this->DccIdNbDigits = EEPROMextent.read(pos + 1);
		this->MoreLessIncrement = EEPROMextent.read(pos + 2);

		// Add here a new config
		byte controledSlot = 0;
#ifndef NANOCONTROLER
		controledSlot = EEPROMextent.read(pos + 3);
		if (controledSlot < DCCItemList.ListSize / DCCItemList.ItemSize)
#endif
		{
			DCCItemList.GetLoco(controledSlot, &this->controled);
			this->MoreLessIncrement = 1;
			if (this->controled.GetSteps() == 128)
				this->MoreLessIncrement = 10;
		}
	}
}

void Handle::ConfigSave()
{
	byte i = DDC.IndexOf(this);
	int pos = EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	if (DDC.dcType == Dcc)
	{
		EEPROMextent.write(pos, 0);
		EEPROMextent.write(pos + 1, this->DccIdNbDigits);
		EEPROMextent.write(pos + 2, this->MoreLessIncrement);
#ifdef NANOCONTROLER
		// Save/load only the current loco.
		DCCItemList.UpdateLoco(&this->controled);
#else
		EEPROMextent.write(pos + 3, this->controled.GetSlotNumber());
#endif
	}
}