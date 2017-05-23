//-------------------------------------------------------------------
#ifndef __controlerDccpp_H__
#define __controlerDccpp_H__
//-------------------------------------------------------------------

#include "Controler.hpp"

#ifdef VISUALSTUDIO
#include "../VStudio/DCCpp.hpp"
#else
#include "DCCpp/DCCpp_Uno.h"
#include "DCCpp/PacketRegister.h"
#include "DCCpp/CurrentMonitor.h"
//#include "DCCpp/SerialCommand.h"
#include "DCCpp/Config.h"
//#include "DCCpp/Comm.h"
#endif

//-------------------------------------------------------------------

class ControlerDccpp : public Controler
{
	private:
		bool programMode;

		static CurrentMonitor *mMonitor;

		void SetSpeedRaw();
		void SetFunctionsRaw();
		void SetCv1Raw(int inId);

	public:
		static volatile RegisterList mainRegs, progRegs;

		ControlerDccpp();
		
	public:
		void begin();
		void beginMain(uint8_t OptionalDirectionMotor, uint8_t Dummy, uint8_t SignalEnablePin, uint8_t CurrentMonitor);
		void beginProg(uint8_t OptionalDirectionMotor, uint8_t Dummy, uint8_t SignalEnablePin, uint8_t CurrentMonitor);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void SetCv1(int inId);
		void ToggleFunction(byte inFunctionNumber);
		void PanicStop(bool inStop);
		void loop();
		void StartProgramMode();
		void EndProgramMode();

#ifdef DDC_DEBUG_MODE
		static void showConfiguration();
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
