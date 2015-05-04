//-------------------------------------------------------------------
#ifndef __function_H__
#define __function_H__
//-------------------------------------------------------------------

#define FUNCTION_TYPE				'F'

enum FunctionType : byte
{
	Unknown = 0,
	Lights = 1,
	FrontLight = 2,
	RearLight = 3,
	CabinLight = 4,
	Sound1 = 11,
	Sound2 = 12,
	Sound3 = 13,
	Sound4 = 14,
	Sound5 = 15,
	Sound6 = 16,
	Sound7 = 17,
	Sound8 = 18,
	Sound9 = 19,
	Sound10 = 20,
};

class Function
{
	public:
		byte SlotNumber;	// Slot number in the EEPROM list, 255 (EEPROMLIST_EMPTY_SLOT) if not saved.
		int DccIdFunction;
		char name[12];
		FunctionType Type;

	public:
		Function() { this->DccIdFunction = 0; this->Type = Unknown; }
		Function(int DccId, char *inName, FunctionType inType);
		inline byte GetSlotNumber() const { return this->SlotNumber; }
		inline void SetSlotNumber(byte inSlotNumber) { this->SlotNumber = inSlotNumber; }

		inline const char *GetName() const { return this->name; }
		inline void SetName(const char *inNewName) { STRCPY(this->name, inNewName); }
		inline uint16_t GetDccId() const { return this->DccIdFunction; }
		inline void SetDccId(uint16_t inDccId) { this->DccIdFunction = inDccId; }
		inline FunctionType GetType() const { return this->Type; }
		inline void SetType(FunctionType inType) { this->Type = inType; }

		void Load(int inStartPos);
		void Save(int inStartPos);
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
