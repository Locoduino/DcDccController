/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Class for one handle>
*************************************************************/

#include "DcDccControler.h"
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "Commanders.h"
#endif

#include <stdarg.h>

Handle::Handle()
{
	this->id = 0;
	this->MoreLessIncrement = 10;
	this->pUi = NULL;
	this->DccIdNbDigits = 4;
}

void Handle::begin(byte inHandleId)
{
	this->id = inHandleId;
	if (this->pUi == 0)
		this->pUi = new LcdUi();
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
						ID					EditInt	1-127 or 1-10126
						Address long		YesNo
						Name				EditText
						Steps				Choice 14/28/128
						Function n			Int
						Function n+1        Int
				Add
						ID					EditInt	1-127 or 1-10126
						Address long		YesNo
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
		Window *pWinLocoId = this->pUi->AddWindow(new WindowInt(STR_LOCOID, 10026, 1), pChoiceLocoEdit, 0);
		Window *pWinLocoAddress = this->pUi->AddWindow(new WindowYesNo(STR_LONGADDRESS), pChoiceLocoEdit, 0);
		Window *pWinLocoName = this->pUi->AddWindow(new WindowText(STR_LOCONAME), pChoiceLocoEdit, 0);
		WindowChoice *pChoiceSteps = (WindowChoice *)this->pUi->AddWindow(new WindowChoice(STR_LOCOSTEPS, 3, false), pChoiceLocoEdit, 0);
		pChoiceSteps->AddChoice(STR_LOCOSTEPS14);
		pChoiceSteps->AddChoice(STR_LOCOSTEPS28);
		pChoiceSteps->AddChoice(STR_LOCOSTEPS128);
		for (int i = 0; i < Locomotive::FunctionNumber; i++)
			this->pUi->AddWindow(new WindowFunction(STR_FUNCTIONID, i), pChoiceLocoEdit, 0);
		pChoiceLocoEdit->AddChoice(STR_LOCONEW);
		this->pUi->AddWindow(pWinLocoId, pChoiceLocoEdit, 1);
		this->pUi->AddWindow(pWinLocoAddress, pChoiceLocoEdit, 1);
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
		6				ID				EditInt	1-127 or 1-10126
		7				Address long	YesNo
		8				Steps			Choice 14/28/128
		9				Function 1		EditInt	1-127 or 1-10126
		10				Function 2		EditInt	1-127 or 1-10126
		11			LocoControl			WindowLocoControl
		12	Dc-Dcc						Interrupt
		13	Stop						Interrupt
		*/

		// BEGINS !
		winSplash.begin(STR_TITLE, STR_COPYRIGHT);	// splash
		winChoiceMain.begin(STR_MODEMODECHOICE, &choiceMain);	// menu dc
		winChoiceConfig.begin(STR_MODECONFIG, &choiceConfig);
		if (DcDccControler::dcType == Dc)
		{
			winFreq.begin(STR_PWMFREQCFG, NULL);
		}
		else
		{
			winConfigDigit.begin(STR_HANDLECFGDIGITS, &(this->DccIdNbDigits), 2, 4);
			winResetConfig.begin(STR_RESETCONFIG, STR_CONFIRM);
			winChoiceConfigLoco.begin(STR_LOCOEDIT, &choiceConfigLoco);
			winLocoId.begin(STR_LOCOID, NULL, 1, 10026);
			winLongAddress.begin(STR_LONGADDRESS, NULL);
			winChoiceSteps.begin(STR_LOCOSTEPS, &choiceSteps);
			winFunction1.begin(STR_FUNCTIONID1, &addressFunction1);
			winFunction2.begin(STR_FUNCTIONID2, &addressFunction2);
			winProgramCV1.begin(STR_PROGRAMCV1, NULL, 1, 10026);
		}

		winLocoControl.begin(1000, this);
		this->windowInterruptDcDcc.begin(STR_DCDCC, STR_DCDCC2, EVENT_DCDCC); // Dc/Dcc mode change
		this->windowInterruptEmergency.begin(STR_STOP, STR_STOP2, EVENT_EMERGENCY); // Emergency stop

		// WINDOWS in the list
		this->pUi->AddWindow(&winSplash);
		this->pUi->AddWindow(&winChoiceMain);
		this->pUi->AddWindow(&winChoiceConfig);
		if (DcDccControler::dcType == Dc)
		{
			this->pUi->AddWindow(&winFreq);
		}
		else
		{
			this->pUi->AddWindow(&winConfigDigit);
			this->pUi->AddWindow(&winResetConfig);
			this->pUi->AddWindow(&winChoiceConfigLoco);
			this->pUi->AddWindow(&winLocoId);
			this->pUi->AddWindow(&winLongAddress);
			this->pUi->AddWindow(&winChoiceSteps);
			this->pUi->AddWindow(&winFunction1);
			this->pUi->AddWindow(&winFunction2);
			this->pUi->AddWindow(&winProgramCV1);
		}

		this->pUi->AddWindow(&winLocoControl);
		this->pUi->AddWindow(&this->windowInterruptDcDcc);
		this->pUi->AddWindow(&this->windowInterruptEmergency);

		// CHOICES
		winChoiceMain.AddChoice(STR_MODECONFIG);
		winChoiceConfig.SetFather(&winChoiceMain, STR_MODECONFIG);	// config DDC
		if (DcDccControler::dcType == Dc)
		{
			winChoiceConfig.AddChoice(STR_PWMFREQCFG);
			winFreq.SetFather(&winChoiceConfig, STR_PWMFREQCFG);	// DC Freq
			winFreq.SetValueAddress(&(((ControlerDc *)DcDccControler::pControler)->DCFrequencyDivisorIndex));
		}
		else
		{
			winChoiceConfig.AddChoice(STR_HANDLECFGDIGITS);
				winConfigDigit.SetFather(&winChoiceConfig, STR_HANDLECFGDIGITS);
			winChoiceConfig.AddChoice(STR_RESETCONFIG);
				winResetConfig.SetFather(&winChoiceConfig, STR_RESETCONFIG);
			winChoiceMain.AddChoice(STR_LOCOEDIT);
				winChoiceConfigLoco.SetFather(&winChoiceMain, STR_LOCOEDIT);
					winChoiceConfigLoco.AddChoice(STR_LOCOID);
						winLocoId.SetFather(&winChoiceConfigLoco, STR_LOCOID);
					winChoiceConfigLoco.AddChoice(STR_LONGADDRESS);
						winLongAddress.SetFather(&winChoiceConfigLoco, STR_LONGADDRESS);
					winChoiceConfigLoco.AddChoice(STR_LOCOSTEPS);
						winChoiceSteps.SetFather(&winChoiceConfigLoco, STR_LOCOSTEPS);
							winChoiceSteps.AddChoice(STR_LOCOSTEPS14);
							winChoiceSteps.AddChoice(STR_LOCOSTEPS28);
							winChoiceSteps.AddChoice(STR_LOCOSTEPS128);
					winChoiceConfigLoco.AddChoice(STR_FUNCTIONID1);
						winFunction1.SetFather(&winChoiceConfigLoco, STR_FUNCTIONID1);
					winChoiceConfigLoco.AddChoice(STR_FUNCTIONID2);
						winFunction2.SetFather(&winChoiceConfigLoco, STR_FUNCTIONID2);
			winChoiceMain.AddChoice(STR_PROGRAMCV1);
				winProgramCV1.SetFather(&winChoiceMain, STR_PROGRAMCV1);
		}
		winChoiceMain.AddChoice(STR_MODELOCOCTRL);
			winLocoControl.SetFather(&winChoiceMain, STR_MODELOCOCTRL); // run
	}
#endif

	winLocoId.SetValueAddress(this->controled.GetDccIdAddress());
	winLongAddress.SetValueAddress(this->controled.GetDccIdAddressKindAddress());
	winProgramCV1.SetValueAddress(this->controled.GetDccIdAddress());
}

void Handle::EndSetup()
{
	this->SetSpeed(0);
	Window *pLoco = this->pUi->GetWindowById(1000);

	// If Dc goto to control directly
	if (DcDccControler::dcType == Dc)
	{
		this->pUi->MoveToWindow(pLoco);
	}
	else
	// If Dcc
	//	If loco configurated, goto control
	{
		if (this->controled.GetSlotNumber() != 255)
			this->pUi->MoveToWindow(pLoco);
	}
}

void Handle::StartContent()
{
}
	
void Handle::Clear()
{
	this->MoreLessIncrement = 10;
	this->DccIdNbDigits = 4;
}

//#define DDC_DEBUG_MODE

void Handle::SetControledLocomotive(Locomotive &inLocomotive)
{
#ifdef DDC_DEBUG_MODE
	Serial.println(F("SetLocomotive"));
#endif
	this->controled.Copy(inLocomotive);
}

bool Handle::loop(unsigned long inEvent)
{
	bool changed = false;

	int event;

	if (inEvent <= LAST_HANDLE_EVENT)
	{	// Five handles events
		if (inEvent / 50 != this->id)
			return false;

		event = inEvent % 50;

		// Handle Function events
		if (event >= LCD1_EVENT_FUNCTION1)
		{
			byte function = event - LCD1_EVENT_FUNCTION1;

			if (DcDccControler::dcType == Dcc)
			{
				this->ToggleFunction(function);
#ifdef DDC_DEBUG_MODE
				Serial.print(F("Function button "));
				Serial.print(function + 1);
				Serial.println(F(" pressed"));
#endif
			}
			else
			{
				if (function == 0)
				{
					Window *pCurrent = pUi->GetGlobalCurrentWindow();
					((ControlerDc *)DcDccControler::pControler)->SetSlowMode(!((ControlerDc *)DcDccControler::pControler)->IsSlowMode());
					if (((ControlerDc *)DcDccControler::pControler)->IsSlowMode())
						this->MoreLessIncrement = 5;
					else
						this->MoreLessIncrement = 10;
					pCurrent->SetState(STATE_START);
				}
			}
		}

	}
	else
		// Other event (emergency and others...)
		event = inEvent;

	if (pUi->loop(event))
	{
		Window *pCurrent = pUi->GetGlobalCurrentWindow();
		Locomotive &loco = this->edited;

#ifdef DDC_DEBUG_MODE
		//LcdUi::printEvent(event, F("pUi->Loop"));
		//Window::printState(pUi->GetState(), F("pUi->Loop"));
#endif

		switch (pUi->GetState())
		{
		case STATE_INITIALIZE:
			switch (pUi->GetWindowId())
			{
			case STR_STOP:
				DcDccControler::dcTypeAtStart = DcDccControler::dcType;
				DcDccControler::pControler->PanicStop(true);
				break;
			case STR_DCDCC:
				if (DcDccControler::dcType == DcChangeStopped)
					break;

				DcDccControler::dcTypeAtStart = DcDccControler::dcType;
				DcDccControler::dcType = DcChangeStopped;
#ifdef DDC_DEBUG_MODE
				Serial.println(F("Dc/Dcc mode change"));
#endif
				break;

			case STR_PROGRAMCV1:
				((ControlerDcc *)DcDccControler::pControler)->StartProgramMode();
				break;
#ifndef NANOCONTROLER
			case STR_LOCONAME:
				strncpy(this->name, loco.GetName(), 20);
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

				winChoiceSteps.SetCurrentChoiceById(choiceSteps.id);
				break;
			}
			break;

		case STATE_CONFIRMED:
			switch (pUi->GetWindowId())
			{
			case STR_STOP:
				DcDccControler::pControler->PanicStop(false);
				DcDccControler::dcType = DcDccControler::dcTypeAtStart;
				break;
			case STR_DCDCC:
				DcDccControler::dcType = DcDccControler::dcTypeAtStart;
#ifdef DDC_DEBUG_MODE
				Serial.println(F("Dc/Dcc mode change canceled"));
#endif
				break;
			case STR_PWMFREQCFG:
//				((ControlerDc *)DcDccControler::pControler)->SetFrequencyDivisor(((WindowChooseDcFreq *)pCurrent)->GetChoiceIntValue());
				DcDccControler::ConfigSave();
				break;

			case STR_HANDLECFGDIGITS:
				this->ConfigSave();
				break;

			case STR_RESETCONFIG:
				DcDccControler::ConfigReset();
				this->Clear();
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

#ifdef NANOCONTROLER
			case STR_LONGADDRESS:
			case STR_LOCOID:
				this->ConfigSave();
				break;
#endif

			case STR_PROGRAMCV1:
				{
					((ControlerDcc *)DcDccControler::pControler)->SetCv1(this->cv1);
					loco.SetDccId(this->cv1);
					this->SetSpeed(0);
#ifdef NANOCONTROLER
					this->ConfigSave();
#endif
				}

				((ControlerDcc *)DcDccControler::pControler)->EndProgramMode();
				break;
#ifndef NANOCONTROLER
			case STR_LOCONAME:
				loco.SetName(((WindowText*) pCurrent)->GetTextValue());
				break;
#endif
			case STR_LOCOSTEPS:
				int val;
				switch (this->choiceSteps.id)
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
			case STR_FUNCTIONID1:
				loco.Functions[0].SetDccId(addressFunction1);
#ifdef NANOCONTROLER
				this->ConfigSave();
#endif
				break;

			case STR_FUNCTIONID2:
				loco.Functions[1].SetDccId(addressFunction2);
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
	DcDccControler::pControler->SetControled(&this->controled);
	DcDccControler::pControler->SetSpeed(inNewSpeed);
}

void Handle::SetDirection(bool inToLeft)
{
	DcDccControler::pControler->SetControled(&this->controled);
	DcDccControler::pControler->SetDirection(inToLeft);
}

void Handle::ToggleFunction(byte inFunctionNumber)
{
	DcDccControler::pControler->SetControled(&this->controled);
	DcDccControler::pControler->ToggleFunction(inFunctionNumber);
}

void Handle::ConfigLoad()
{
	byte i = DcDccControler::IndexOf(this);
	int pos= EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	if (DcDccControler::dcType == Dcc)
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
	byte i = DcDccControler::IndexOf(this);
	int pos = EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	if (DcDccControler::dcType == Dcc)
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