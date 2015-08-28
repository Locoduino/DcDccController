//-------------------------------------------------------------------
#ifndef __handle_H__
#define __handle_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
#include "VStudioLcdUi\arduino2.hpp"
#else
#include "arduino2.hpp"
#endif

#include "Locomotive.hpp"
#include "FunctionHandle.hpp"
//#include "TaggedStringList.hpp"
#include "Controler.hpp"
#ifndef __LcdUi_H__
#include "../LcdUi/LcdUi.h"
#endif
//-------------------------------------------------------------------

#define EEPROM_DDC_HANDLE_CONFIG_SIZE	16

class Handle
{
	public:
		// Handle definition
		//byte id;
		byte handleNumber;	// Handle index in the DcDccControler list.

		// Handle hardware
		ButtonsCommanderButton *pSpeedPotentiometer;
		ButtonsCommanderButton *pSpeedEncoder;
		ButtonsCommanderButton *pSpeedPushMore;
		ButtonsCommanderButton *pSpeedPushLess;
		ButtonsCommanderButton *pDirectionPush;
		ButtonsCommanderButton *pModeButton;	// If null, the mode will be LocomotiveControl at start

		LcdUi *pUi;
		byte windowInterruptDcDcc;
		byte windowInterruptEmergency;
		byte windowInterruptSaveLoco;

		// Handle control situation
		int MoreLessIncrement;

		// Handle configuration
		bool ConfigBacklight;
		byte DccIdNbDigits;

	private:
		// Handle interactive situation
		Locomotive controled;
#ifdef NANOCONTROLER
#define edited	controled
#else
		Locomotive edited;
#endif
		byte editedFunction;
		FunctionHandle* *pFunctionHandleList;
		int functionsSize;
		int functionsAddCounter;

	public:
		Handle();
		
		void Setup(int inNumberOfFunctions = 0);
		void Setup(int inNumberOfFunctions, FunctionHandle *inpFirstFunction, ...);
		void EndSetup(bool inDcMode);
		void StartUI();
		void StartContent();
		void Interrupt(int inEvent);
		void InterruptEnd();

		void AddFunction(FunctionHandle *);
		FunctionHandle *GetFunction(int inFunctionNumber);

		inline LcdUi *GetUI() const { return this->pUi; }
		void SetControledLocomotive(const Locomotive &Locomotive);
		inline const Locomotive &GetControledLocomotive() const { return this->controled; }
		inline const Locomotive &GetEditedLocomotive() const { return this->edited; }
		inline byte GetEditedFunction() const { return this->editedFunction; }
		inline byte GetFunctionNumber() const { return this->functionsAddCounter; }

		inline bool IsLeftDir() const { return this->controled.GetDirectionToLeft(); }
		inline bool IsRightDir() const { return !IsLeftDir(); }
		inline bool IsPanicStopped() const { return this->pUi->GetWindowInterrupt() == this->windowInterruptEmergency; }

		void SetSpeed(int inNewSpeed);
		void SetDirection(bool inToLeft);
		void ToggleFunction(int inFunctionNumber);

		bool Loop();

		void ConfigLoad();
		void ConfigSave();

	private:
#ifdef DEBUG_MODE
		void CheckIndex(int inIndex, const __FlashStringHelper *infunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
