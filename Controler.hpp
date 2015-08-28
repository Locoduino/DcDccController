//-------------------------------------------------------------------
#ifndef __controler_H__
#define __controler_H__
//-------------------------------------------------------------------

class Controler
{
	protected:
		Locomotive *pControled;
		bool panicStopped;

	public:
		inline Controler() { this->pControled = 0; this->panicStopped = false; }
		
	public:
		inline void SetControled(Locomotive *inpControled) { this->pControled = inpControled; }
		virtual void Setup(uint8_t inPwmPin, uint8_t inDirPin) { }
		virtual bool SetSpeed(int inNewSpeed) { return false; }
		virtual bool SetDirection(bool inToLeft) { return false; }
		virtual void ToggleFunction(byte inFunctionNumber) { }
		virtual void PanicStop(bool inStop) { }
		virtual void Loop() { }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
