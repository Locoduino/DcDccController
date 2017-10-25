//-------------------------------------------------------------------
#ifndef __engineShed_H__
#define __engineShed_H__
//-------------------------------------------------------------------

#ifndef NANOCONTROLER

class EngineShed
{
public:
	static byte LocoSlots[LOCO_MAXNUMBER];
	static Locomotive Current;
	static char name[DCC_LOCONAME_LENGTH];

	static void Clear() { for (int i = 0; i < LOCO_MAXNUMBER; i++) EngineShed::LocoSlots[i] = 255; }
	static void RebuildLocos();

	static void begin();
	static void SetCurrent(byte inSlotNumber);
	static byte AddCurrentLoco();
	static void Remove(byte inSlotNumber);
	static char *GetName(byte inSlotNumber);

	static int inline ConfigSave(int inPosition) {
		return inPosition + 5;	 // 5 bytes of security margin for future data save...
	}

	static int inline ConfigLoad(int inPosition) {
		return inPosition + 5;	 // 5 bytes of security margin for future data save...
	}
};

//-------------------------------------------------------------------
#endif
#endif
//-------------------------------------------------------------------
