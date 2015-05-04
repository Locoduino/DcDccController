//-------------------------------------------------------------------
#ifndef __controlerDc_H__
#define __controlerDc_H__
//-------------------------------------------------------------------

#include "Controler.hpp"

//-------------------------------------------------------------------

class ControlerDc : public Controler
{
	private:
		uint8_t dcPWMpin;
		uint8_t dcDirPin;

	public:
		inline ControlerDc() { this->dcPWMpin = 0; this->dcDirPin = 0; }
		
	public:
		void Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void PanicStop(bool inStop);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
