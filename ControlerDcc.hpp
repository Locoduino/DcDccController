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
		void SetCv1Raw(int inId);

	public:
		inline ControlerDcc() { this->dcType = Dcc;  this->maxSpeed = 127; }
		
	public:
		void Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void SetCv1(int inId);
		void ToggleFunction(byte inFunctionNumber);
		void PanicStop(bool inStop);
		void Loop();
		void StartProgramMode();
		void EndProgramMode();
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
