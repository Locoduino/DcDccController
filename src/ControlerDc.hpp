//-------------------------------------------------------------------
#ifndef __controlerDc_H__
#define __controlerDc_H__
//-------------------------------------------------------------------

#include "Controler.hpp"

//-------------------------------------------------------------------

#define SLOWMODELIMIT	100

class ControlerDc : public Controler
{
	private:
		uint8_t dcPWMpin;
		uint8_t dcDirPin;
		unsigned int DCFrequencyDivisor;
		bool slowMode;

	public:
		inline ControlerDc() { this->dcPWMpin = 0; this->dcDirPin = 0; this->DCFrequencyDivisor = 64; this->slowMode = false; this->maxSpeed = 255; }
		
	public:
		void begin(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		inline unsigned int GetFrequencyDivisor() { return this->DCFrequencyDivisor; }
		void SetFrequencyDivisor(unsigned int inDivisor);
		void PanicStop(bool inStop);
		inline bool IsSlowMode() { return this->slowMode; }
		void SetSlowMode(bool inSlowMode);

	private:
		void SetFrequencyDivisorRaw(unsigned int inDivisor);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
