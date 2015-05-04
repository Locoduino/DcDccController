//-------------------------------------------------------------------
#ifndef __EEPROMM_H__
#define __EEPROMM_H__
//-------------------------------------------------------------------

#define EEPROM		EEPROMClass::EEPROMInstance

class EEPROMClass
{
private:
	byte eeprom_image[4096];

public:
	EEPROMClass();
	uint8_t read(int);
	void write(int, uint8_t);

	static EEPROMClass EEPROMInstance;
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------