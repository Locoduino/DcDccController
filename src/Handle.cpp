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

	for (int i = 0; i < FUNCTION_NUMBER; i++)
		this->addressFunction[i] = 0;
}

void Handle::begin(byte inHandleId)
{
	this->id = inHandleId;
	if (this->pUi == 0)
		this->pUi = new LcdUi();
}

void Handle::StartUI()
{
#ifndef VISUALSTUDIO
	extern uint8_t *__brkval;
	extern uint8_t *__heap_start;
#endif

	int v;

	if (this->pUi != 0)
#ifndef NANOCONTROLER
	{
		/*
		In Dc :
		0	Splash
		1	Start : Confirm dc
		2		DDC menu				Choice
		3			Config DDC			Choice
		4				PWM freq		WindowChooseDcFreq
		5			LocoControl			WindowLocoControl
		6	Stop						Interrupt

		In Dcc :
		0	Splash
		1	Start : Confirm dcc
		2		DDC menu				Choice
		3			Config DDC			Choice
		4				Reset			Confirm
		5			LocoControl			WindowLocoControl
		6			RollingStock		winChoiceRollingStock
						PowerUp
		7					winChooseLoco
						Garage
		8					winChooseSlotRemove
		9					winRemoveSlot	Confirm
		Maintenance	(if only one loco in the stock, and no prog track)
		10					winChooseSlotMaintain
		11					winSlotLocoId		EditInt	1-10126
		12					winSlotLocoName		EditText
		13					winSlotChoiceSteps	Choice 14/28/128
		14					winSlotFunctions	EditInt	0-28
		15					winSlotCV			WindowCv
		16			EngineShed			winChoiceEngineShed
						Delivery
		17					winLocoAddId			EditInt	1-10126
		18					winLocoAddName			EditText
		19					winChoiceLocoAddSteps	Choice 14/28/128
		20					winLocoAddFunctions		EditInt	0-28
						Deregistration
		21					winChooseLocoRemove
		22					winRemoveLoco	Confirm
						Log book
		23					winLocoLogId			EditInt	1-10126
		24					winLocoLogName			EditText
		25					winChoiceLogSteps		Choice 14/28/128
		26					winLogFunctions			EditInt	0-28
		27			Workshop		Choice  (only if a prog track is declared)
						Move to
		28					winChooseSlotWorkshop
						Back To Stock
		29					winRemoveWorkshop	Confirm
						Repair
		30					winWSRepairId			EditInt	1-10126
		31					winWSRepairName			EditText
		32					winChoiceWSRepairSteps	Choice 14/28/128
		33					winWSRepairFunctions	EditInt	0-28
		34					winWSRepairProgramCV	WindowCv

		35	Stop						Interrupt
		*/
		this->locoChosen = 255;
		this->slotChosen = 255;
		this->locoWorkshop = 255;

		// BEGINS !
		winSplash.begin(STR_TITLE, STR_COPYRIGHT);
		winStart.begin(DcDccControler::dcType == Dc ? STR_DC : STR_DCC, STR_CONFIRM, NULL);
		winChoiceMain.begin(STR_MODEMODECHOICE, &choiceMain);	// menu
		winChoiceConfigDDC.begin(STR_MODECONFIG, &choiceConfig);
		if (DcDccControler::dcType == Dc)
		{
			winFreq.begin(STR_PWMFREQCFG, NULL);
		}
		else
		{
			winLocoInput.begin(1);
			winLocoInput.AddWindow(&winLocoAddId);
			winLocoInput.AddWindow(&winLocoAddName);
			winLocoInput.AddWindow(&winChoiceLocoAddSteps);
			winLocoInput.AddWindow(&winLocoAddFunctions);
			winLocoAddId.begin(STR_LOCOID, EngineShed::Current.GetDccIdAddress(), 1, 10026);
			winLocoAddName.begin(STR_LOCONAME, EngineShed::Current.GetNameAddress(), DCC_LOCONAME_LENGTH);
			winChoiceLocoAddSteps.begin(STR_LOCOSTEPS, &choiceStepsEngineShedAdd);
			winLocoAddFunctions.begin(STR_FUNCTIONS);

			winResetConfig.begin(STR_RESETCONFIG, STR_CONFIRM, &this->confirmed);
			winChoiceEngineShed.begin(STR_ENGINESHED, &this->choiceEngineShed);
				// Add
				//winLocoInput.begin(1);
/*				winLocoAddId.begin(STR_LOCOID, EngineShed::Current.GetDccIdAddress(), 1, 10026);
				winLocoAddName.begin(STR_LOCONAME, EngineShed::Current.GetNameAddress(), DCC_LOCONAME_LENGTH);
				winChoiceLocoAddSteps.begin(STR_LOCOSTEPS, &choiceStepsEngineShedAdd);
				winLocoAddFunctions.begin(STR_FUNCTIONS);*/
				// Remove
				winChooseLocoRemove.begin(STR_LOCOREMOVECONFIRM, &this->locoChosen, InRollingStock);
				winRemoveLoco.begin(STR_LOCOREMOVE, STR_CONFIRM, &this->confirmed);
				// Edit
				//winLocoInput.begin(1);
/*				winLocoLogId.begin(STR_LOCOID, EngineShed::Current.GetDccIdAddress(), 1, 10026);
				winLocoLogName.begin(STR_LOCONAME, EngineShed::Current.GetNameAddress(), DCC_LOCONAME_LENGTH);
				winChoiceLogSteps.begin(STR_LOCOSTEPS, &choiceStepsEngineShedAdd);
				winLogFunctions.begin(STR_FUNCTIONS);*/
			winChoiceRollingStock.begin(STR_ROLLINGSTOCK, &this->choiceRollingStock);
				// Add
				winChooseSlotLoco.begin(STR_SLOTADD, &this->locoChosen, NotInRollingStock);
				// Remove
				winChooseSlotRemove.begin(STR_SLOTREMOVECONFIRM, &this->slotChosen, UsedRollingStockSlot);
				winRemoveSlot.begin(STR_SLOTREMOVE, STR_CONFIRM, &this->confirmed);
				// Maintain
				winChooseSlotMaintain.begin(STR_SLOTMAINTAIN, &this->slotChosen, UsedRollingStockSlot);
				//winLocoInput.begin(1);
/*				winSlotLocoId.begin(STR_LOCOID, EngineShed::Current.GetDccIdAddress(), 1, 10026);
				winSlotLocoName.begin(STR_LOCONAME, EngineShed::Current.GetNameAddress(), DCC_LOCONAME_LENGTH);
				winSlotChoiceSteps.begin(STR_LOCOSTEPS, &choiceStepsEngineShedAdd);
				winSlotFunctions.begin(STR_FUNCTIONS);*/
			winChoiceWorkshop.begin(STR_WORKSHOP, &this->choiceWorkshop);
				// Add
				winChooseSlotWorkshop.begin(STR_WORKSHOPMOVE, &this->locoWorkshop, UsedRollingStockSlot);
				// Remove
				winRemoveWorkshop.begin(STR_WORKSHOPBACK, STR_CONFIRM, &this->confirmed);
				// Repair
				//winLocoInput.begin(1);
/*				winWSRepairId.begin(STR_LOCOID, EngineShed::Current.GetDccIdAddress(), 1, 10026);
				winWSRepairName.begin(STR_LOCONAME, EngineShed::Current.GetNameAddress(), DCC_LOCONAME_LENGTH);
				winChoiceWSRepairSteps.begin(STR_LOCOSTEPS, &choiceStepsEngineShedAdd);
				winWSRepairFunctions.begin(STR_FUNCTIONS);*/
				winWSRepairProgramCV.begin(STR_MODIFYCV);
		}

		winSlotCV.begin(STR_MODIFYCV);
		winLocoControl.begin(254, this);
		this->windowInterruptEmergency.begin(STR_STOP, STR_STOP2, EVENT_EMERGENCY); // Emergency stop

		// WINDOWS in the list
		this->pUi->AddWindow(&winSplash);
		this->pUi->AddWindow(&winStart);
		this->pUi->AddWindow(&winChoiceMain);
		this->pUi->AddWindow(&winChoiceConfigDDC);
		if (DcDccControler::dcType == Dc)
		{
			this->pUi->AddWindow(&winFreq);
		}
		else
		{
			this->pUi->AddWindow(&winResetConfig);
			this->pUi->AddWindow(&winChoiceEngineShed);
				// Add
				this->pUi->AddWindow(&winLocoInput);
/*				this->pUi->AddWindow(&winLocoAddId);
				this->pUi->AddWindow(&winLocoAddName);
				this->pUi->AddWindow(&winChoiceLocoAddSteps);
				this->pUi->AddWindow(&winLocoAddFunctions);*/
				// Remove
				this->pUi->AddWindow(&winChooseLocoRemove);
				this->pUi->AddWindow(&winRemoveLoco);
				// Edit
				this->pUi->AddWindow(&winLocoInput);
/*				this->pUi->AddWindow(&winLocoLogId);
				this->pUi->AddWindow(&winLocoLogName);
				this->pUi->AddWindow(&winChoiceLogSteps);
				this->pUi->AddWindow(&winLogFunctions);*/
			this->pUi->AddWindow(&winChoiceRollingStock);
				// Add
				this->pUi->AddWindow(&winChooseSlotLoco);
				// Remove
				this->pUi->AddWindow(&winChooseSlotRemove);
				this->pUi->AddWindow(&winRemoveSlot);
				// Maintain
				this->pUi->AddWindow(&winChooseSlotMaintain);
				this->pUi->AddWindow(&winLocoInput);
/*				this->pUi->AddWindow(&winSlotLocoId);
				this->pUi->AddWindow(&winSlotLocoName);
				this->pUi->AddWindow(&winSlotChoiceSteps);
				this->pUi->AddWindow(&winSlotFunctions);*/
			this->pUi->AddWindow(&winChoiceWorkshop);
				// Add
				this->pUi->AddWindow(&winChooseSlotWorkshop);
				// Remove
				this->pUi->AddWindow(&winRemoveWorkshop);
				// Repair
				this->pUi->AddWindow(&winLocoInput);
/*				this->pUi->AddWindow(&winWSRepairId);
				this->pUi->AddWindow(&winWSRepairName);
				this->pUi->AddWindow(&winChoiceWSRepairSteps);
				this->pUi->AddWindow(&winWSRepairFunctions);*/
				this->pUi->AddWindow(&winWSRepairProgramCV);
		}

		this->pUi->AddWindow(&winLocoControl);
		this->pUi->AddWindow(&this->windowInterruptEmergency);

		// CHOICES
		winChoiceMain.AddChoice(STR_MODELOCOCTRL, &winLocoControl); // run
		winChoiceMain.AddChoice(STR_MODECONFIG, &winChoiceConfigDDC);
		if (DcDccControler::dcType == Dc)
		{
			winChoiceConfigDDC.AddChoice(STR_PWMFREQCFG, &winFreq);	// DC Freq
			winFreq.SetValueAddress(&(((ControlerDc *)DcDccControler::pControler)->DCFrequencyDivisorIndex));
		}
		else
		{
			winChoiceConfigDDC.AddChoice(STR_RESETCONFIG, &winResetConfig);

			winChoiceMain.AddChoice(STR_ENGINESHED, &winChoiceEngineShed);
				winChoiceEngineShed.AddChoice(STR_LOCOADD);
					winLocoInput.AddFather(STR_ENGINESHED, STR_LOCOADD);
/*					winLocoAddName.SetFather(STR_ENGINESHED, STR_LOCOADD);
					winChoiceLocoAddSteps.SetFather(STR_ENGINESHED, STR_LOCOADD);
						winChoiceLocoAddSteps.AddChoice(STR_LOCOSTEPS14);
						winChoiceLocoAddSteps.AddChoice(STR_LOCOSTEPS28);
						winChoiceLocoAddSteps.AddChoice(STR_LOCOSTEPS128);
					winLocoAddFunctions.SetFather(STR_ENGINESHED, STR_LOCOADD);*/
				winChoiceEngineShed.AddChoice(STR_LOCOREMOVE);
					winChooseLocoRemove.SetFather(STR_ENGINESHED, STR_LOCOREMOVE);
					winRemoveLoco.SetFather(STR_ENGINESHED, STR_LOCOREMOVE);
				winChoiceEngineShed.AddChoice(STR_LOCOEDIT);
					winLocoInput.AddFather(STR_ENGINESHED, STR_LOCOEDIT);
					/*winLocoLogId.SetFather(STR_ENGINESHED, STR_LOCOEDIT);
					winLocoLogName.SetFather(STR_ENGINESHED, STR_LOCOEDIT);
					winChoiceLogSteps.SetFather(STR_ENGINESHED, STR_LOCOEDIT);
						winChoiceLogSteps.AddChoice(STR_LOCOSTEPS14);
						winChoiceLogSteps.AddChoice(STR_LOCOSTEPS28);
						winChoiceLogSteps.AddChoice(STR_LOCOSTEPS128);
					winLogFunctions.SetFather(STR_ENGINESHED, STR_LOCOEDIT);*/

			winChoiceMain.AddChoice(STR_ROLLINGSTOCK, &winChoiceRollingStock);
				winChoiceRollingStock.AddChoice(STR_SLOTADD, &winChooseSlotLoco);
				winChoiceRollingStock.AddChoice(STR_SLOTREMOVE, &winRemoveSlot);
					winChooseSlotRemove.SetFather(STR_ROLLINGSTOCK, STR_SLOTREMOVE);
					winRemoveSlot.SetFather(STR_ROLLINGSTOCK, STR_SLOTREMOVE);
				winChoiceRollingStock.AddChoice(STR_SLOTMAINTAIN);
					winChooseSlotMaintain.SetFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);
					winLocoInput.AddFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);
/*					winSlotLocoId.SetFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);
					winSlotLocoName.SetFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);
					winSlotChoiceSteps.SetFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);
						winSlotChoiceSteps.AddChoice(STR_LOCOSTEPS14);
						winSlotChoiceSteps.AddChoice(STR_LOCOSTEPS28);
						winSlotChoiceSteps.AddChoice(STR_LOCOSTEPS128);
					winSlotFunctions.SetFather(STR_ROLLINGSTOCK, STR_SLOTMAINTAIN);*/

			winChoiceMain.AddChoice(STR_WORKSHOP, &winChoiceWorkshop);
				winChoiceWorkshop.AddChoice(STR_WORKSHOPMOVE, &winChooseSlotWorkshop);
				winChoiceWorkshop.AddChoice(STR_WORKSHOPBACK, &winRemoveWorkshop);
				winChoiceWorkshop.AddChoice(STR_WORKSHOPREPAIR);
					winLocoInput.AddFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);
/*					winWSRepairId.SetFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);
					winWSRepairName.SetFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);
					winChoiceWSRepairSteps.SetFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);
						winChoiceWSRepairSteps.AddChoice(STR_LOCOSTEPS14);
						winChoiceWSRepairSteps.AddChoice(STR_LOCOSTEPS28);
						winChoiceWSRepairSteps.AddChoice(STR_LOCOSTEPS128);
					winWSRepairFunctions.SetFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);*/
					winWSRepairProgramCV.SetFather(STR_WORKSHOP, STR_WORKSHOPREPAIR);
		}
	}
#else
	{
		/*
		In Dc :
		0	Splash
		1	Start : Confirm dc
		2		Mode Choice				Choice
		3			Config DDC			Choice
		4				PWM freq		WindowChooseDcFreq
		5			LocoControl			WindowLocoControl
		6	Stop						Interrupt

		In Dcc :
		0	Splash
		1	Start : Confirm dcc
		2		DDC menu				Choice
		3			Config DDC			Choice
		4				ID				EditInt	1-127 or 1-10126
		6				Steps			Choice 14/28/128
		7				Modify Cv		WindowCv
		8				Function n		EditInt	1-127 or 1-10126
		9				Function n+1	EditInt	1-127 or 1-10126
		10			LocoControl			WindowLocoControl
		11	Stop						Interrupt
		*/

		// BEGINS !
		winSplash.begin(STR_TITLE, STR_COPYRIGHT);
		winStart.begin(DcDccControler::dcType == Dc?STR_DC:STR_DCC, STR_CONFIRM, NULL);
		winChoiceMain.begin(STR_MODEMODECHOICE, &choiceMain);	// menu
		winChoiceConfigDDC.begin(STR_MODECONFIG, &choiceConfig);
		if (DcDccControler::dcType == Dc)
		{
			winFreq.begin(STR_PWMFREQCFG, NULL);
		}
		else
		{
			winLocoId.begin(STR_LOCOID, NULL, 1, 10026);
			winChoiceSteps.begin(STR_LOCOSTEPS, &choiceSteps);
			winFunctions.begin(STR_FUNCTIONS);
			winProgramCV.begin(STR_MODIFYCV);
		}

		winLocoControl.begin(254, this);
		this->windowInterruptEmergency.begin(STR_STOP, STR_STOP2, EVENT_EMERGENCY); // Emergency stop

		// WINDOWS in the list
		this->pUi->AddWindow(&winSplash);
		this->pUi->AddWindow(&winStart);
		this->pUi->AddWindow(&winChoiceMain);
		this->pUi->AddWindow(&winChoiceConfigDDC);
		if (DcDccControler::dcType == Dc)
		{
			this->pUi->AddWindow(&winFreq);
		}
		else
		{
			this->pUi->AddWindow(&winLocoLogId);
			this->pUi->AddWindow(&winChoiceSteps);
			this->pUi->AddWindow(&winFunctions);
			this->pUi->AddWindow(&winProgramCV);
		}

		this->pUi->AddWindow(&winLocoControl);
		this->pUi->AddWindow(&this->windowInterruptEmergency);

		// CHOICES
		winChoiceMain.AddChoice(STR_MODECONFIG, &winChoiceConfigDDC);
		if (DcDccControler::dcType == Dc)
		{
			winChoiceConfigDDC.AddChoice(STR_PWMFREQCFG, &winFreq);	// DC Freq
			winFreq.SetValueAddress(&(((ControlerDc *)DcDccControler::pControler)->DCFrequencyDivisorIndex));
		}
		else
		{
				winChoiceConfigDDC.AddChoice(STR_LOCOID, &winLocoLogId);
				winChoiceConfigDDC.AddChoice(STR_LOCOSTEPS, &winChoiceSteps);
					winChoiceSteps.AddChoice(STR_LOCOSTEPS14);
					winChoiceSteps.AddChoice(STR_LOCOSTEPS28);
					winChoiceSteps.AddChoice(STR_LOCOSTEPS128);
				winChoiceConfigDDC.AddChoice(STR_FUNCTIONS, &winFunctions);
				winChoiceConfigDDC.AddChoice(STR_MODIFYCV, &winProgramCV);
		}
		winChoiceMain.AddChoice(STR_MODELOCOCTRL, &winLocoControl); // run
	}
#endif

	winLocoLogId.SetValueAddress(this->controled.GetDccIdAddress());

//	Serial.print((int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval));
//	Serial.println(F(" bytes"));

//	PRINT_WINDOWS(this->pUi);
}

void Handle::EndSetup()
{
	this->SetSpeed(0);
	/*Window *pLoco = &winLocoControl;
	
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
	*/
}

void Handle::StartContent()
{
}
	
void Handle::Clear()
{
	this->MoreLessIncrement = 10;
}

//#define DDC_DEBUG_MODE

void Handle::SetControledLocomotive(byte inRollingStockSlot)
{
#ifdef DDC_DEBUG_MODE
	Serial.println(F("SetLocomotive"));
#endif
	RollingStock::SetCurrent(inRollingStockSlot);
	this->controled.Copy(RollingStock::Current);
}

void Handle::SetControledLocomotive(Locomotive &inLocomotive)
{
#ifdef DDC_DEBUG_MODE
	Serial.println(F("SetLocomotive"));
#endif
	this->controled.Copy(inLocomotive);
}

bool Handle::loop(unsigned long inEvent, int inData)
{
	bool saveConfig = false;

	int event;

	if (inEvent <= LAST_HANDLED_EVENT)
	{	// Five handles events
		if (inEvent / 50 != this->id)
			return false;

		event = inEvent % 50;

		if (event == LCD1_EVENT_ENCODER)
		{
			if (inData == +1)
				event = EVENT_MORE;
			if (inData == -1)
				event = EVENT_LESS;
		}

		// Handle Function events
		if (event >= LCD1_EVENT_FUNCTION0)
		{
			byte function = event - LCD1_EVENT_FUNCTION0;
			bool activate = inData == COMMANDERS_MOVE_ON;

			if (DcDccControler::dcType == Dcc)
			{
				this->SetFunction(function, activate);
#ifdef DDC_DEBUG_MODE
				Serial.print(F("Function F"));
				Serial.print(function);
				Serial.println(activate ? F(" On") : F(" Off"));
#endif
			}
			else
			{
				if (function == 0)
				{
					Window *pCurrent = pUi->GetGlobalCurrentWindow();
					((ControlerDc *)DcDccControler::pControler)->SetSlowMode(activate);
					int steps = ((ControlerDc *)DcDccControler::pControler)->GetMaxSpeed();
					this->MoreLessIncrement = steps / (pUi->GetScreen()->GetSizeX() - 2);
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
		Locomotive &loco = this->controled;

#ifdef DDC_DEBUG_MODE
		//LcdUi::printEvent(event, F("pUi->Loop"));
		//Window::printState(pUi->GetState(), F("pUi->Loop"));
#endif

		switch (pUi->GetState())
		{
		case STATE_INITIALIZE:
			int val;
			switch (pUi->GetWindowId())
			{
			case STR_STOP:
				DcDccControler::dcTypeAtStart = DcDccControler::dcType;
				DcDccControler::pControler->PanicStop(true);
				break;
			case STR_LOCOID:
				// Gets the current id of the loco...
				val = ((ControlerDccpp *)DcDccControler::pControler)->ReadCv(1);

				if (val < 0)
					val = 3;
				this->controled.SetDccId(val);
				break;
			case STR_LOCOSTEPS:
				switch (loco.GetSteps())
				{
				case 14: val = STR_LOCOSTEPS14; break;
				case 28: val = STR_LOCOSTEPS28; break;
				default: val = STR_LOCOSTEPS128; break;
				}

				winChoiceLogSteps.SetCurrentChoiceById(val);
				break;
			}
			break;

		case STATE_CONFIRMED:
			switch (pUi->GetWindowId())
			{
			case STR_DC:
			case STR_DCC:
				DcDccControler::pControler->PanicStop(false);
				pCurrent->SetActive(false);
				break;
			case STR_STOP:
				DcDccControler::pControler->PanicStop(false);
				DcDccControler::dcType = DcDccControler::dcTypeAtStart;
				break;
			case STR_PWMFREQCFG:
				{
					WindowChooseDcFreq *pWinFreq = (WindowChooseDcFreq *)pCurrent;
					((ControlerDc *)DcDccControler::pControler)->SetFrequencyDivisorIndex(pWinFreq->GetChoiceValue());
					DcDccControler::ConfigSave();
				}
				break;
			case STR_LOCOSTEPS:
				int val;
				switch (this->winChoiceLogSteps.GetFirstLine())
				{
				case STR_LOCOSTEPS14: val = 14; this->MoreLessIncrement = 1; break;
				case STR_LOCOSTEPS28: val = 28; this->MoreLessIncrement = 1; break;
				default: val = 128; this->MoreLessIncrement = 10; break;
				}
				loco.SetSteps(val);
				saveConfig = true;
				break;
			case STR_FUNCTIONS:
				saveConfig = true;
				break;

#ifndef NANOCONTROLER
			case STR_SLOTADD:
			{
				byte free = RollingStock::FindFreeSlot();
				if (free != 255)
					RollingStock::AddLoco(free, this->locoChosen); 
				DcDccControler::ConfigSave();
			}
				break;
			case STR_SLOTREMOVE:
				if (this->confirmed)
					RollingStock::Remove(this->slotChosen);
				DcDccControler::ConfigSave();
				break;
			case STR_RESETCONFIG:
				DcDccControler::ConfigReset();
				DcDccControler::ConfigSave();
				this->Clear();
				break;
#else
			case STR_LOCOID:
				saveConfig = true;
				break;
#endif
			}
			pUi->SetState(STATE_POSTCONFIRMED);
			break;
		}

		if (saveConfig)
			this->ConfigSave();
	}

	return saveConfig;
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

void Handle::SetFunction(byte inFunctionNumber, bool inActivate)
{
	DcDccControler::pControler->SetControled(&this->controled);
	DcDccControler::pControler->SetFunction(inFunctionNumber, inActivate);
}

void Handle::ConfigLoad()
{
	byte i = DcDccControler::IndexOf(this);
	int pos= EEPROM_DDC_CONFIG_SIZE + (i * EEPROM_DDC_HANDLE_CONFIG_SIZE);

	if (DcDccControler::dcType == Dcc)
	{
		this->MoreLessIncrement = EEPROMextent.readByte(pos + 1);

		// Add here a new config
		byte controledSlot = 0;
#ifndef NANOCONTROLER
		controledSlot = EEPROMextent.readByte(pos + 3);
		if (controledSlot < DCCItemList.listSize / DCCItemList.itemSize)
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
		EEPROMextent.writeByte(pos, 0);		// Data version
		EEPROMextent.writeByte(pos+1, this->MoreLessIncrement);		// Data version
#ifdef NANOCONTROLER
		// Save/load only the current loco.
		DCCItemList.UpdateLoco(&this->controled);
#else
		EEPROMextent.writeByte(pos + 3, this->controled.GetSlotNumber());
#endif
	}
}