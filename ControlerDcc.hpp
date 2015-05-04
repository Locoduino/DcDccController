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

	public:
		inline ControlerDcc() { }
		
	public:
		void Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void PanicStop(bool inStop);
		inline void Loop() { dps.update();	}
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
