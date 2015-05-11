//-------------------------------------------------------------------
#ifndef __locomotive_H__
#define __locomotive_H__
//-------------------------------------------------------------------
#include "Function.hpp"
//-------------------------------------------------------------------

#define DCC_SHORT_ADDRESS           0x00
#define DCC_LONG_ADDRESS            0x01

#define LOCOMOTIVE_TYPE				'L'

class Locomotive
{
private:
	byte SlotNumber;	// Slot number in the EEPROM list, 255 (EEPROMLIST_EMPTY_SLOT) if not saved.
	uint16_t DccId;
	uint8_t addressKind;
	char Name[12];
	uint8_t steps;
	Function* *pFunctions;
	byte functionsAddCounter;
	byte size;

	int mappedSpeed;
	bool directionToLeft;

public:
	static Locomotive AnalogLocomotive;

public:
	Locomotive();
	Locomotive(uint16_t inDccId, uint8_t adressKind, char *inName, uint8_t inSteps = 0);

	void Setup(byte inNumberOfFunctions);
	void Setup(byte inNumberOfFunctions, Function *inpFirstFunction, ...);
	void AddFunction(Function *);
	Function *GetFunction(int inDccIdFunction) const;
	void SetFunctionsSize(byte size);
	inline Function *GetFunctionFromIndex(byte inIndex)	const { return this->pFunctions == 0 ? 0 : this->pFunctions[inIndex]; }
	byte GetFunctionIndex(Function *inpRef) const;
	void Copy(const Locomotive &inLocomotive);
	inline const char *GetName() const { return this->Name; }
	inline void SetName(const char *inNewName) { STRCPY(this->Name, inNewName); }
	inline uint16_t GetDccId() const { return this->DccId; }
	inline void SetDccId(uint16_t inDccId) { this->DccId = inDccId; }
	inline uint8_t GetDccAddressKind() const { return this->addressKind; }
	inline void SetDccAddressKind(uint8_t inAddressKind) { this->addressKind = inAddressKind; }
	inline byte GetFunctionsSize() const { return this->size; }
	inline byte GetFunctionNumber() const { return this->functionsAddCounter; }
	inline uint8_t GetSteps() const { return this->steps; }
	inline void SetSteps(uint8_t inSteps) { this->steps = inSteps; }
	inline int GetMappedSpeed() const { return  this->mappedSpeed; }
	inline bool GetDirectionToLeft() const { return  this->directionToLeft; }
	inline void SetMappedSpeed(int inSpeed) { this->mappedSpeed = inSpeed; }
	inline void SetDirectionToLeft(bool inDirectionToLeft) { this->directionToLeft = inDirectionToLeft; }
	inline byte GetSlotNumber() const { return this->SlotNumber; }
	inline void SetSlotNumber(byte inSlotNumber) { this->SlotNumber = inSlotNumber; }

	void Load(int inStartPos);
	static void LoadName(int inStartPos, char *outpName);
	void Save(int inStartPos);
	void Clear();

#ifdef DEBUG_MODE
	void CheckIndex(byte inIndex, const __FlashStringHelper *inFunc);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
