//-------------------------------------------------------------------
#ifndef __handle_H__
#define __handle_H__
//-------------------------------------------------------------------

#include "arduino.h"

#ifndef __LcdUi_H__
#include "LcdUi.h"
#endif

#include "Locomotive.hpp"
#include "FunctionHandle.hpp"
#include "Controler.hpp"

class Handle;

#include "WindowChooseDcFreq.hpp"
#include "WindowChooseLoco.hpp"
#include "WindowFunction.hpp"
#include "WindowLocoControl.hpp"
//-------------------------------------------------------------------

#define EEPROM_DDC_HANDLE_CONFIG_SIZE	16

class Handle
{
	public:
		// Handle definition
		byte id;

#ifndef NANOCONTROLER
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
		WindowSplash winSplash;
		WindowChoice winChoiceMain;	// menu
		WindowChoice winChoiceConfig;	// config
		WindowInt winCfgDigit;;
		WindowChooseDcFreq winDcFreq;
		WindowConfirm winResetConfig;	// reset config
		WindowLocoControl winLocoControl; // run
		WindowChooseLoco winChooseLoco;
		WindowChoice winChoiceLocoEdit;
		//Window *pWinChooseLoco = this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOSELECT, this), pChoiceLocoEdit, 0);
		WindowInt winLocoId;
		WindowYesNo winLocoEdit;
		WindowText winLocoName;
		WindowChoice *winChoiceSteps;
		WindowFunction winFunction1;
		WindowFunction winFunction2;
		WindowFunction winFunction3;
		WindowFunction winFunction4;
		WindowFunction winFunction5;
		WindowFunction winFunction6;
		WindowFunction winFunction7;
		WindowFunction winFunction8;
		this->pUi->AddWindow(pWinLocoId, pChoiceLocoEdit, 1);
		this->pUi->AddWindow(pWinLocoAddress, pChoiceLocoEdit, 1);
		this->pUi->AddWindow(pWinLocoName, pChoiceLocoEdit, 1);
		this->pUi->AddWindow(pChoiceSteps, pChoiceLocoEdit, 1);
		for (int i = 0; i < Locomotive::FunctionNumber; i++)
			this->pUi->AddWindow(new WindowFunction(STR_FUNCTIONID, i), pChoiceLocoEdit, 1);
		this->pUi->AddWindow(new WindowChooseLoco(STR_LOCOREMOVE, this), pChoiceLocoEdit, 2);
		this->pUi->AddWindow(new WindowConfirm(STR_LOCOREMOVE, STR_CONFIRM), pChoiceLocoEdit, 2);

		this->windowInterruptDcDcc = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_DCDCC, STR_DCDCC2))); // Dc/Dcc mode change
		this->windowInterruptEmergency = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterrupt(STR_STOP, STR_STOP2))); // Emergency stop
		this->windowInterruptSaveLoco = this->pUi->GetWindowIndex(this->pUi->AddWindow(new WindowInterruptConfirm(STR_SAVELOCO, STR_CONFIRM))); // Save the loco after modif
#else
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
		9				Function n		EditInt	1-127 or 1-10126
		10				Function n+1	EditInt	1-127 or 1-10126
		11			LocoControl			WindowLocoControl
		12	Dc-Dcc						Interrupt
		13	Stop						Interrupt
		*/

		Choice choiceMain;
		Choice choiceConfig;
		byte Freq;
		Choice choiceConfigLoco;
		Choice choiceSteps;
		int addressFunction1;
		int addressFunction2;
		int cv1;

		WindowSplash winSplash;
		WindowChoice winChoiceMain;
		WindowChoice winChoiceConfig;
		WindowChooseDcFreq winFreq;
		WindowInt winConfigDigit;
		WindowConfirm winResetConfig;
		WindowChoice winChoiceConfigLoco;
		WindowInt winLocoId;
		WindowYesNo winLongAddress;
		WindowChoice winChoiceSteps;
		WindowFunction winFunction1;
		WindowFunction winFunction2;
		WindowInt winProgramCV1;
		WindowLocoControl winLocoControl;

		WindowInterrupt windowInterruptDcDcc;
		WindowInterrupt windowInterruptEmergency;
#endif
	
		LcdUi *pUi;

		// Handle configuration
		int MoreLessIncrement;
		int DccIdNbDigits;	// Only an int because the WindowInt cannot edit a byte !

	private:
		// Handle interactive situation
		Locomotive controled;
#ifdef NANOCONTROLER
#define edited	controled
#else
		Locomotive edited;
#endif
		FunctionHandle FunctionHandleList[2];

	public:
		Handle();
		
		void begin(byte inHandleId = 0);
		void EndSetup(bool inDcMode);
		void StartUI();
		void StartContent();
		void Clear();

		//void AddFunction(FunctionHandle *);
		//FunctionHandle *GetFunction(byte inFunctionNumber);

		inline LcdUi *GetUI() const { return this->pUi; }
		void SetControledLocomotive(Locomotive &Locomotive);
		inline const Locomotive &GetControledLocomotive() const { return this->controled; }
		inline const Locomotive &GetEditedLocomotive() const { return this->edited; }

		inline bool IsLeftDir() const { return this->controled.GetDirectionToLeft(); }
		inline bool IsRightDir() const { return !IsLeftDir(); }
		inline bool IsPanicStopped() const { return this->pUi->GetWindowInterrupt() == &this->windowInterruptEmergency; }

		void SetSpeed(int inNewSpeed);
		void SetDirection(bool inToLeft);
		void ToggleFunction(byte inFunctionNumber);

		bool loop(unsigned long inEvent);

		void ConfigLoad();
		void ConfigSave();

	private:
#ifdef DDC_DEBUG_MODE
		void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
