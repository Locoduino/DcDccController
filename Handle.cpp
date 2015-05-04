/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one handle>
*************************************************************/

#include "DcDccControler.h"
#include "ButtonsCommanderPotentiometer.hpp"
//#include "TaggedStringList.hpp"
#include "WindowLocoControl.hpp"
#include "WindowChooseLoco.hpp"
#include "WindowChooseFunction.hpp"

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
	this->pWindowInterruptDcDcc = 0;
	this->pWindowInterruptEmergency = 0;
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
	this->pUi = new LcdUi();
}

void Handle::Setup(int inNumberOfFunctions)
{
	this->pFunctionHandleList = new FunctionHandle*[inNumberOfFunctions];
	this->functionsSize = inNumberOfFunctions;
	this->functionsAddCounter = 0;

	Screen::YesMsg = 16;
	Screen::NoMsg = 17;
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
		this->pUi->SetWindowsNumber(32);

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
						Functions			Choice
							Edit			ChooseFunction
								ID			EditInt	1-127 or 1-10126
								Name		EditText
								Type		Choice (Front/Rear/Sound...)
							Add
								ID			EditInt	1-127 or 1-10126
								Name		EditText
								Type		Choice (Front/Rear/Sound...)
							Remove			ChooseFunction
								Confirm		Confirm
				Add							
						Address long		YesNo
						ID					EditInt	1-127 or 1-10126
						Name				EditText
						Steps				Choose 14/28/128
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
						WindowChoice *pChoiceFunctions = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOFUNCTIONS), pChoiceLocoEdit, 0);
							pChoiceFunctions->AddChoice(STR_FUNCTIONEDIT);
								Window *pWinChooseFunction = this->pUi->AddWindow(new WindowChooseFunction(STR_FUNCTIONSELECT, this), pChoiceFunctions, 0);
								Window *pWinFunctionId = this->pUi->AddWindow(new WindowInt(STR_FUNCTIONID, 10026, 1), pChoiceFunctions, 0);
								Window *pWinFunctionName = this->pUi->AddWindow(new WindowText(STR_FUNCTIONNAME), pChoiceFunctions, 0);
								WindowChoice *pChoiceType = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_FUNCTIONTYPE), pChoiceFunctions, 0);
									pChoiceType->AddChoice(STR_FUNCTIONLIGHTS);
									pChoiceType->AddChoice(STR_FUNCTIONFRONTLIGHTS);
									pChoiceType->AddChoice(STR_FUNCTIONREARLIGHTS);
									pChoiceType->AddChoice(STR_FUNCTIONCABINLIGHTS);
							pChoiceFunctions->AddChoice(STR_FUNCTIONNEW);
								this->pUi->AddWindow(pWinFunctionId, pChoiceFunctions, 1);
								this->pUi->AddWindow(pWinFunctionName, pChoiceFunctions, 1);
								this->pUi->AddWindow(pChoiceType, pChoiceFunctions, 1);
							pChoiceFunctions->AddChoice(STR_FUNCTIONREMOVE);
								this->pUi->AddWindow(new WindowChooseFunction(STR_FUNCTIONSELECT, this), pChoiceFunctions, 2);
								this->pUi->AddWindow(new WindowConfirm(STR_FUNCTIONREMOVE, STR_CONFIRM), pChoiceFunctions, 2);
					pChoiceLocoEdit->AddChoice(STR_LOCONEW);
						this->pUi->AddWindow(pWinLocoAddress, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pWinLocoId, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pWinLocoName, pChoiceLocoEdit, 1);
						this->pUi->AddWindow(pChoiceSteps, pChoiceLocoEdit, 1);
					pChoiceLocoEdit->AddChoice(STR_LOCOREMOVE);
						this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOREMOVE, this), pChoiceLocoEdit, 2);
						this->pUi->AddWindow(new WindowConfirm(STR_LOCOREMOVE, STR_CONFIRM), pChoiceLocoEdit, 2);

		this->pWindowInterruptDcDcc = (WindowInterrupt *) this->pUi->AddWindow(new WindowInterrupt(STR_DCDCC, STR_DCDCC2)); // Dc/Dcc mode change
		this->pWindowInterruptEmergency = (WindowInterrupt *) this->pUi->AddWindow(new WindowInterrupt(STR_STOP, STR_STOP2)); // Emergency stop
		this->pWindowInterruptSaveLoco = (WindowInterruptConfirm *) this->pUi->AddWindow(new WindowInterruptConfirm(STR_SAVELOCO, STR_CONFIRM)); // Save the loco after modif

/*		if (this->pUi->GetScreen()->GetSizeX() <= 16)
			this->pUi->SplashScreen(STR_DDCTITLE16, STR_DDCCOPYRIGHT16);
		else
			this->pUi->SplashScreen(STR_DDCTITLE, STR_DDCCOPYRIGHT);*/
	}
}

void Handle::AddFunction(FunctionHandle *pFunction)
{
	CHECK(this->functionsAddCounter, "Handle::AddFunction");
	this->pFunctionHandleList[this->functionsAddCounter++] = pFunction;
}

void Handle::SetLocomotive(const Locomotive &inLocomotive)
{
	this->controled.Copy(inLocomotive);
	if (this->pSpeedPotentiometer != 0)
		((ButtonsCommanderPotentiometer *) this->pSpeedPotentiometer)->SetMinMax(0, inLocomotive.GetSteps());
}

void Handle::Interrupt(int inEvent)
{
	switch(inEvent)
	{
	case EVENT_DCDCC:		this->pUi->Interrupt(this->pWindowInterruptDcDcc);		break;
	case EVENT_EMERGENCY:	this->pUi->Interrupt(this->pWindowInterruptEmergency);	break;
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
		}

	if (this->pSpeedPushMore != 0)
		if (this->pSpeedPushMore->IsSelectedLastLoop())
		{
			event = EVENT_MORE;
			this->pSpeedPushMore->UnselectLastLoop();
		}

	if (this->pSpeedPushLess != 0)
		if (this->pSpeedPushLess->IsSelectedLastLoop())
		{
			event = EVENT_LESS;
			this->pSpeedPushLess->UnselectLastLoop();
		}

	if (this->pDirectionPush != 0)
		if (this->pDirectionPush->IsSelectedLastLoop())
		{
			event = EVENT_SELECT;
			this->pDirectionPush->UnselectLastLoop();
		}

	if (this->pModeButton != 0)
		if (this->pModeButton->IsSelectedLastLoop())
		{
			event = EVENT_CANCEL;
			this->pModeButton->UnselectLastLoop();
		}

	if (event != EVENT_NONE || pUi->GetState() != STATE_NONE || pUi->GetCurrentWindow()->GetType() == WINDOWTYPE_SPLASH)
	{
		pUi->Loop(event);

		switch (pUi->GetState())
		{
		case STATE_INITIALIZE:
			switch (pUi->GetWindowId())
			{
			case STR_BACKLIGHTCFG:
				pUi->GetCurrentWindow()->SetChoiceValue(this->ConfigBacklight ? Screen::YesMsg : Screen::NoMsg);
				break;
			case STR_HANDLECFGDIGITS:
				pUi->GetCurrentWindow()->SetValue(this->DccIdNbDigits);
				break;
			case STR_LONGADDRESS:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetDccAddressKind());
				break;
			case STR_LOCOID:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetDccId());
				break;
			case STR_LOCONAME:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetName());
				break;
			case STR_LOCOSTEPS:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetSteps());
				break;
			case STR_FUNCTIONID:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetFunctionFromIndex(this->editedFunction)->GetDccId());
				break;
			case STR_FUNCTIONNAME:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetFunctionFromIndex(this->editedFunction)->GetName());
				break;
			case STR_FUNCTIONTYPE:
				pUi->GetCurrentWindow()->SetValue(this->edited.GetFunctionFromIndex(this->editedFunction)->GetType());
				break;
			}
			break;

		case STATE_CONFIRMED:
			switch (pUi->GetWindowId())
			{
			case STR_BACKLIGHTCFG:
				this->ConfigBacklight = pUi->GetCurrentWindow()->GetChoiceValue() == Screen::YesMsg;
				this->ConfigSave();
				break;

			case STR_HANDLECFGDIGITS:
				this->DccIdNbDigits = pUi->GetCurrentWindow()->GetIntValue();
				this->ConfigSave();
				break;

			case STR_RESETCONFIG:
				DDC.ResetConfig();
				break;

			case STR_MODELOCOCHANGE:
				DCCItemList.GetLoco(pUi->GetCurrentWindow()->GetChoiceValue(), &this->controled);
				this->ConfigSave();
				break;

			case STR_LOCOSELECT:
				DCCItemList.GetLoco(pUi->GetCurrentWindow()->GetChoiceValue(), &this->edited);
				break;

			case STR_LOCONEW:
				this->edited.Clear();
				break;

			case STR_LOCOREMOVE:
				if (pUi->GetCurrentWindow()->GetType() == WINDOWTYPE_CONFIRM)
				{
					WindowConfirm *pConfirm = (WindowConfirm *)pUi->GetCurrentWindow();

					if (pConfirm->GetChoiceValue() == STR_YES)
					{
						DCCItemList.FreeLoco(&this->edited);
						this->edited.Clear();
						WindowChooseLoco::SetSelectedLoco(255);
						WindowChooseLoco::RebuildChoices();
					}
				}
				else
					DCCItemList.GetLoco(pUi->GetCurrentWindow()->GetChoiceValue(), &this->edited);
				break;

			case STR_LONGADDRESS:
				this->edited.SetDccAddressKind(pUi->GetCurrentWindow()->GetIntValue());
				break;
			case STR_LOCOID:
				this->edited.SetDccId(pUi->GetCurrentWindow()->GetIntValue());
				break;
			case STR_LOCONAME:
				this->edited.SetName(pUi->GetCurrentWindow()->GetTextValue());
				break;
			case STR_LOCOSTEPS:
				this->edited.SetSteps(pUi->GetCurrentWindow()->GetChoiceValue());
				break;

			case STR_FUNCTIONSELECT:
			{
				WindowChooseFunction *pChoose = (WindowChooseFunction *)pUi->GetCurrentWindow();
				this->editedFunction = this->edited.GetFunctionIndex(pChoose->GetSelected());
			}
				break;

			case STR_FUNCTIONID:
				this->edited.GetFunctionFromIndex(this->editedFunction)->SetDccId(pUi->GetCurrentWindow()->GetIntValue());
				break;
			case STR_FUNCTIONNAME:
				this->edited.GetFunctionFromIndex(this->editedFunction)->SetName(pUi->GetCurrentWindow()->GetTextValue());
				break;
			case STR_FUNCTIONTYPE:
				this->edited.GetFunctionFromIndex(this->editedFunction)->SetType((FunctionType)(pUi->GetCurrentWindow()->GetIntValue()));
				break;

			case STR_SAVELOCO:
				if (this->pWindowInterruptSaveLoco->GetChoiceValue() == STR_YES)
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

void Handle::ConfigLoad()
{
	for (int i = 0; i < DDC.handleAddcounter; i++)
	{
		if (DDC.pHandleList[i] == this)
		{
			this->ConfigBacklight = EEPROMextent.read(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE)) > 0;
			this->DccIdNbDigits = EEPROMextent.read(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE) + 1);
			byte slot = EEPROMextent.read(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE) + 2);
			DCCItemList.GetLoco(slot, &this->controled);
			break;
		}
	}
}

void Handle::ConfigSave()
{
	for (int i = 0; i < DDC.handleAddcounter; i++)
	{
		if (DDC.pHandleList[i] == this)
		{
			EEPROMextent.write(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE), (byte) this->ConfigBacklight);
			EEPROMextent.write(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE) + 1, this->DccIdNbDigits);
			EEPROMextent.write(EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE) + 2, this->controled.GetSlotNumber());
			break;
		}
	}
}