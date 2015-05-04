//-------------------------------------------------------------------
#ifndef __handle_H__
#define __handle_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
#include "VStudioLcdUI\arduino2.hpp"
#else
#include "arduino2.hpp"
#endif

#include "Locomotive.hpp"
#include "FunctionHandle.hpp"
//#include "TaggedStringList.hpp"
#include "Controler.hpp"
#ifndef __LcdUi_H__
#include "../LcdUi/LcdUI.h"
#endif
//-------------------------------------------------------------------

#define EEPROM_DDC_HANDLE_CONFIG_SIZE	16

class Handle
{
	public:
		// Handle definition
		byte id;
		byte handleNumber;	// Handle index in the DcDccControler list.

		// Handle hardware
		ButtonsCommanderButton *pSpeedPotentiometer;
		ButtonsCommanderButton *pSpeedEncoder;
		ButtonsCommanderButton *pSpeedPushMore;
		ButtonsCommanderButton *pSpeedPushLess;
		ButtonsCommanderButton *pDirectionPush;
		ButtonsCommanderButton *pModeButton;	// If null, the mode will be LocomotiveControl at start

		LcdUi *pUi;
		WindowInterrupt *pWindowInterruptDcDcc;
		WindowInterrupt *pWindowInterruptEmergency;
		WindowInterrupt *pWindowInterruptSaveLoco;

		// Handle control situation
		unsigned int Speed;
		int MoreLessIncrement;
		bool DirectionToLeft;

		// Handle configuration
		bool ConfigBacklight;
		byte DccIdNbDigits;

	private:
		// Handle interactive situation
		Locomotive controled;
		Locomotive edited;
		byte editedFunction;
		FunctionHandle* *pFunctionHandleList;
		int functionsSize;
		int functionsAddCounter;

	public:
		Handle();
		Handle(byte inId);
		
		void Setup();
		void Setup(int inNumberOfFunctions);
		void Setup(int inNumberOfFunctions, FunctionHandle *inpFirstFunction, ...);
		void StartUI();
		void Interrupt(int inEvent);
		void InterruptEnd();

		void AddFunction(FunctionHandle *);
		FunctionHandle *GetFunction(int inFunctionNumber);

		inline LcdUi *GetUI() { return this->pUi; }
		void SetLocomotive(const Locomotive &Locomotive);
		inline const Locomotive &GetLocomotive() const { return this->controled; }
		inline const Locomotive &GetEditedLocomotive() const { return this->edited; }
		inline byte GetEditedFunction() const { return this->editedFunction; }

		inline bool IsLeftDir() const { return this->DirectionToLeft; }
		inline bool IsRightDir() const { return !this->DirectionToLeft; }
		//inline bool IsStopped() const { return ports[inType].Get(inPort).IsStopped(); }

		void SetSpeed(int inNewSpeed);
		void SetDirection(bool inToLeft);

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
