//-------------------------------------------------------------------
#ifndef __controlerDcc_H__
#define __controlerDcc_H__
//-------------------------------------------------------------------

#include "Controler.hpp"

#ifdef VISUALSTUDIO
#include "cmdrarduino.hpp"
#else
#include "DCCPacket.hpp"
#include "DCCPacketQueue.hpp"
#include "DCCPacketScheduler.hpp"
#endif

//-------------------------------------------------------------------

class ControlerDcc : public Controler
{
	private:
		DCCPacketScheduler dps;
		void SetSpeedRaw();
		void SetFunctionsRaw();

	public:
		inline ControlerDcc() { this->dcType = Dcc; }
		
	public:
		void Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void ToggleFunction(byte inFunctionNumber);
		void PanicStop(bool inStop);
		void Loop();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
