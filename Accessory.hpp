//-------------------------------------------------------------------
#ifndef __accessory_H__
#define __accessory_H__
//-------------------------------------------------------------------

enum AccessoryType
{
	motor = 1,
	light = 2
};

class Accessory
{
private:
	int DccId;
	char Name[12];
	int duration;

	// Global list of all accessories
	static Accessory* *pAllAccessories;
	static int allSize;
	static int allAddCounter;

protected:

	int dccIdCounter;

public:
	Accessory() { this->DccId = 0; this->duration = 0; }
	Accessory(int inDccId, char *inName, int duration = 0);

	inline const int GetDccId() const { return this->DccId; }
	inline const int GetDuration() const { return this->duration; }

#ifdef DEBUG_MODE
	void CheckIndex(int inIndex, const __FlashStringHelper *inFunc);
#endif

	// Global list of all accessories.

public:
	static void AllSetup(int inNumberOfAccessories);
	static void AllSetup(int inNumberOfAccessories, Accessory *pFirstAccessory, ...);
	static void AllSet(unsigned char inIndex, Accessory *inpAccessory);
	static unsigned char AllAdd(Accessory *inpAccessory);
	static void Load() {}
	static void Save() {}

private:
#ifdef DEBUG_MODE
	static void AllCheckIndex(int inIndex, const __FlashStringHelper *inFunc);
#endif

};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
