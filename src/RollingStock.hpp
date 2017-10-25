//-------------------------------------------------------------------
#ifndef __rollingStock_H__
#define __rollingStock_H__
//-------------------------------------------------------------------

#ifndef NANOCONTROLER
class RollingStock
{
public:
	static byte rollingStock[ROLLINGSTOCK_NUMBER];
	static Locomotive Current;

	static void Clear() { for (int i = 0; i < ROLLINGSTOCK_NUMBER; i++) rollingStock[i] = 255; }
	static void begin();
	static void SetCurrent(byte inSlotNumber);
	static byte FindFreeSlot();
	static bool IsInStock(byte inLocoNumber);
	static void AddLoco(byte inSlotPosition, byte inLocoNumber);
	static void Remove(byte inSlotNumber);
	static int ConfigSave(int inPosition);
	static int ConfigLoad(int inPosition);
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
