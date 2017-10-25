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
#include "WindowLocoControl.hpp"
#include "WindowFunctions.hpp"
#include "WindowCv.hpp"
#ifndef NANOCONTROLER
#include "WindowChooseLoco.hpp"
#include "WindowChooseSlot.hpp"
#endif
//-------------------------------------------------------------------

#define EEPROM_DDC_HANDLE_CONFIG_SIZE	16

class Handle
{
	public:
		// Handle definition
		byte id;

#ifndef NANOCONTROLER
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
						Send
		28					winChooseSlotWorkshop
						Repared
		29					winRemoveWorkshop	Confirm
						Repair
		30					winWSRepairId			EditInt	1-10126
		31					winWSRepairName			EditText
		32					winChoiceWSRepairSteps	Choice 14/28/128
		33					winWSRepairFunctions	EditInt	0-28
		34					winWSRepairProgramCV	WindowCv

		35	Stop						Interrupt
		*/

		Choice choiceMain;
		Choice choiceConfig;
		Choice choiceRollingStock;
		Choice choiceEngineShed;
		Choice choiceWorkshop;
		Choice choiceStepsRolling;
		Choice choiceStepsWorkshop;
		Choice choiceStepsEngineShedAdd;
		Choice choiceStepsEngineShedLog;
		bool confirmed;
		byte locoChosen;
		byte slotChosen;
		byte locoWorkshop;

		WindowSequence winLocoInput;

		WindowInt winLocoAddId;
		WindowText winLocoAddName;
		WindowChoice winChoiceLocoAddSteps;
		WindowFunctions winLocoAddFunctions;

		// Master version
		WindowSplash winSplash;
		WindowConfirm winStart;
		WindowChoice winChoiceMain;
			WindowChoice winChoiceConfigDDC;
				WindowChooseDcFreq winFreq;
				WindowConfirm winResetConfig;
			WindowChoice winChoiceEngineShed;
				// Add
				//WindowSequence winLocoInput;
				/*WindowInt winLocoAddId;
				WindowText winLocoAddName;
				WindowChoice winChoiceLocoAddSteps;
				WindowFunctions winLocoAddFunctions;*/
				// Remove
				WindowChooseLoco winChooseLocoRemove;
				WindowConfirm winRemoveLoco;
				// Edit
				//WindowSequence winLocoInput;
				/*WindowInt winLocoLogId;
				WindowText winLocoLogName;
				WindowChoice winChoiceLogSteps;
				WindowFunctions winLogFunctions;*/
			WindowChoice winChoiceRollingStock;
				// Add
				WindowChooseLoco winChooseSlotLoco;
				// Remove
				WindowChooseSlot winChooseSlotRemove;
				WindowConfirm winRemoveSlot;
				// Maintain
				WindowChooseSlot winChooseSlotMaintain;
				//WindowSequence winLocoInput;
				/*WindowInt winSlotLocoId;
				WindowText winSlotLocoName;
				WindowChoice winSlotChoiceSteps;
				WindowFunctions winSlotFunctions;*/
			WindowChoice winChoiceWorkshop;
				// Add
				WindowChooseSlot winChooseSlotWorkshop;
				// Remove
				WindowConfirm winRemoveWorkshop;
				// Repair
				//WindowSequence winLocoInput;
				/*WindowInt winWSRepairId;
				WindowText winWSRepairName;
				WindowChoice winChoiceWSRepairSteps;
				WindowFunctions winWSRepairFunctions;*/
				WindowCv winWSRepairProgramCV;

			WindowCv winSlotCV;
			WindowLocoControl winLocoControl; // run
#else
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
		4				ID				EditInt	1-10126
		5				Steps			Choice 14/28/128
		6				Modify Cv		WindowCv
		7				Functions		EditInt	0-28
		8			LocoControl			WindowLocoControl
		9	Stop						Interrupt
		*/

		Choice choiceMain;
		Choice choiceConfig;
		Choice choiceConfigLoco;
		Choice choiceSteps;

		WindowSplash winSplash;
		WindowConfirm winStart;
		WindowChoice winChoiceMain;
		WindowChoice winChoiceConfigDDC;
		WindowChooseDcFreq winFreq;
		WindowInt winLocoId;
		WindowChoice winChoiceSteps;
		WindowFunctions winFunctions;
		WindowCv winProgramCV;
		WindowLocoControl winLocoControl;
#endif

		WindowInterrupt windowInterruptEmergency;
		int addressFunction[FUNCTION_NUMBER];

		LcdUi *pUi;

		// Handle configuration
		int MoreLessIncrement;

	private:
		// Handle interactive situation
		// In Mega version, because each handle moves its loco, RollingStock::Current cannot by used...
		Locomotive controled;

	public:
		Handle();
		
		void begin(byte inHandleId = 0);
		void EndSetup();
		void StartUI();
		void StartContent();
		void Clear();

		inline LcdUi *GetUI() const { return this->pUi; }
		void SetControledLocomotive(byte inRollingStockSlot);
		void SetControledLocomotive(Locomotive &Locomotive);
		inline const Locomotive &GetControledLocomotive() const { return this->controled; }

		inline bool IsLeftDir() const { return this->controled.GetDirectionToLeft(); }
		inline bool IsRightDir() const { return !IsLeftDir(); }

		void SetSpeed(int inNewSpeed);
		void SetDirection(bool inToLeft);
		void SetFunction(byte inFunctionNumber, bool inActivate);

		bool loop(unsigned long inEvent, int inData);

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
