#include <string.h>
#include <time.h>
#include <stdio.h>

#include "arduino.h"
#include "EEPROM.hpp"

//////////////////////////////////////////
// EEPROM

EEPROMClass EEPROMClass::EEPROMInstance;

#define EEPROM_FILE	"d:\\EEPROM.txt"

EEPROMClass::EEPROMClass()
{
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "r");

	if (!ptr_file)
	{
		memset(this->eeprom_image, 0, 4096);
		return;
	}

	fread(this->eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}

uint8_t EEPROMClass::read(int inAddress)
{ 
	return this->eeprom_image[inAddress];
}

void EEPROMClass::write(int inAddress, uint8_t inValue)
{
	this->eeprom_image[inAddress] = inValue;
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "w+");

	if (!ptr_file)
	{
		//ASSERT("EEPROM Error ");
		return;
	}

	fwrite(this->eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}
