#define ARDUINO2_MAIN
#include <string.h>
#include <time.h>

#include "arduino.h"
#include "Serial.hpp"
#include "EEPROM.hpp"
#include "arduino2.hpp"

//////////////////////////////////////////
// Arduino

void pinMode(int, int)
{}

void digitalWrite(int, int)
{}

int digitalRead(int)
{
	return 0;
}

void analogWrite(int, int)
{}

int analogRead(int)
{
	return 0;
}

unsigned long millis()
{
	time_t t;
	time(&t);
	return (unsigned long)t;
}

unsigned long micros()
{
	time_t t;
	time(&t);
	return (unsigned long)t;
}

void delay(int)
{}

void interrupts()
{}

void noInterrupts()
{}

void attachInterrupt(uint8_t, void(*)(void), int mode)
{}

void detachInterrupt(uint8_t)
{}

double power(double val, int exp)
{
	double res = val;
	for (int i = 0; i < exp; i++)
		res *= val;

	return res;
}

//////////////////////////////////////////
// Memory

int freeMemory()
{
	return 20000;
}


int map(int, int, int, int, int)
{
	return 0;
}

byte pgm_read_byte(const char *str)
{
	return (byte)*str;
}

int pgm_read_word(const char *str)
{
	return (int)str;
}

//////////////////////////////////////////
// Serial

SerialClass SerialClass::SerialInstance;

String memo;

void SerialClass::print(const char *line)
{
	memo += line;
}

void SerialClass::print(int value, int i)
{
	int base = 10;
	switch (i)
	{
	case DEC:
		base = 10;
		break;
	case HEX:
		base = 16;
		break;
	case BIN:
		base = 2;
		break;
	}
	print(String(value, base));
}

void SerialClass::println(int value, int i)
{
	print(value, i);
	println("");
}

#undef INPUT
#undef OUTPUT
#undef HEX
#undef byte
#undef boolean

#include "LCDUI_VStudioMfcDlg.h"

void SerialClass::println(const char *line)
{
	memo += line;
	char buf[200];
	memo.toCharArray(buf, 200);
	CLCDUI_VStudioMfcDlg::pMainDialog->debug(buf);
	memo = "";
}

void SerialClass::println(const String &line)
{
	memo += line;
	char buf[200];
	memo.toCharArray(buf, 200);
	CLCDUI_VStudioMfcDlg::pMainDialog->debug(buf);
	memo = "";
}

//////////////////////////////////////////
// Arduino 2

int GPIO_to_Arduino_pin(GPIO_pin_t inPin)
{
	int i;
	for (i = 0; i < GPIO_PINS_NUMBER; i++)
	{
		if (inPin == gpio_pins_progmem[i])
			return i;
	}

	return -1;
}

GPIO_pin_t Arduino_to_GPIO_pin(int inPin)
{
	return gpio_pins_progmem[inPin];
}

//////////////////////////////////////////
// EEPROM

EEPROMClass EEPROMClass::EEPROMInstance;

#define EEPROM_FILE	"d:\\EEPROM.txt"
static byte eeprom_image[4096];

EEPROMClass::EEPROMClass()
{
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "r");

	if (!ptr_file)
	{
		memset(eeprom_image, 0, 4096);
		return;
	}

	fread(eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}

uint8_t EEPROMClass::read(int inAddress)
{ 
	return eeprom_image[inAddress]; 
}

void EEPROMClass::write(int inAddress, uint8_t inValue)
{
	eeprom_image[inAddress] = inValue;
	FILE *ptr_file;
	fopen_s(&ptr_file, EEPROM_FILE, "w+");

	if (!ptr_file)
	{
		ASSERT("EEPROM Error ");
		return;
	}

	fwrite(eeprom_image, 1, 4096, ptr_file);
	fclose(ptr_file);
}
