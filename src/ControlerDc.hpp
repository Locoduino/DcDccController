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
		bool slowMode;

	public:
		byte DCFrequencyDivisorIndex;

		inline ControlerDc() { this->dcPWMpin = 0; this->dcDirPin = 0; this->DCFrequencyDivisorIndex = 3; this->slowMode = false; this->maxSpeed = 255; }
		
	public:
		void begin(uint8_t inDcPWMpin, uint8_t inDcDirPin);
		bool SetSpeed(int inNewSpeed);
		bool SetDirection(bool inToLeft);
		void PanicStop(bool inStop);
		inline bool IsSlowMode() { return this->slowMode; }
		void SetSlowMode(bool inSlowMode);

	private:
		void SetFrequencyDivisorRaw(unsigned int inDivisor);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
