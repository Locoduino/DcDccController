//-------------------------------------------------------------------
#ifndef __controler_H__
#define __controler_H__
//-------------------------------------------------------------------

class Controler
{
	protected:
		Locomotive *pControled;

	public:
		inline Controler() { this->pControled = 0; }
		
	public:
		inline void SetControled(Locomotive *inpControled) { this->pControled = inpControled; }
		virtual void Setup(uint8_t inPwmPin, uint8_t inDirPin) { }
		virtual bool SetSpeed(int inNewSpeed) { return false; }
		virtual bool SetDirection(bool inToLeft) { return false; }
		virtual void PanicStop(bool inStop) { }
		virtual void Loop() { }
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
