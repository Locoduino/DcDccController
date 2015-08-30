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
		unsigned int DCFrequencyDivisor;

	public:
		inline ControlerDc() { this->dcPWMpin = 0; this->dcDirPin = 0; this->DCFrequencyDivisor = 64; this->dcType = Dc; }
		
	public:
		void Setup(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		unsigned int GetFrequencyDivisor() { return this->DCFrequencyDivisor; }
		void SetFrequencyDivisor(unsigned int inDivisor);
		void PanicStop(bool inStop);

	private:
		void SetFrequencyDivisorRaw(unsigned int inDivisor);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
