//-------------------------------------------------------------------
#ifndef __controler_H__
#define __controler_H__
//-------------------------------------------------------------------

class Controler
{
	protected:
		Locomotive *pControled;
		bool panicStopped;
		DcDcc dcType;

	public:
		inline Controler() { this->pControled = 0; this->panicStopped = false; }
		
	public:
		inline void SetControled(Locomotive *inpControled) { this->pControled = inpControled; }
		inline virtual void Setup(uint8_t inPwmPin, uint8_t inDirPin) { }
		inline virtual bool SetSpeed(int inNewSpeed) { return false; }
		inline virtual bool SetDirection(bool inToLeft) { return false; }
		inline virtual void ToggleFunction(byte inFunctionNumber) { }
		inline virtual void PanicStop(bool inStop) { }
		inline virtual void Loop() { }

		inline DcDcc GetType() { return this->dcType; }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
