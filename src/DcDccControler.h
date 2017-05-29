//-------------------------------------------------------------------
#ifndef __dcDccControler_H__
#define __dcDccControler_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define DDC_DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

#define GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#define DDC_DEBUG_MODE
#define ARDUINO_AVR_NANO	// for test
#include "../../DIO2/VStudio/DIO2.h"
#else
#include "DIO2.h"
#endif

#if defined(ARDUINO_AVR_NANO) || defined(ARDUINO_AVR_UNO)
#define NANOCONTROLER
#endif

#ifndef __LcdUi_H__
#include "LcdUi.h"
#endif

#ifndef NANOCONTROLER
/*	#ifndef __EEPROMextent_h__
		#include "../EEPROMextent/src/EEPROMextent.h"
	#endif*/
	#define FUNCTION_NUMBER		8
#else
	#define FUNCTION_NUMBER		2
#endif

#ifdef DDC_DEBUG_MODE
static void CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *inFunc);
static void CheckPinNb(int inPin, const __FlashStringHelper *inFunc);

#define CHECKPIN(val, text)		CheckPinNb(cval, text)
#else
#define CHECKPIN(val, text)
#endif

extern const char * const DDC_config_table[] PROGMEM;
extern const char * const string_table[] PROGMEM;

#if defined(__DDCLanguage_H__)

#ifdef SHORTLENGTH_MSG
const char str_title[] PROGMEM = "Dc/Dcc Cont V0.7";
const char str_copyright[] PROGMEM = "By Thierry Paris";
#else
const char str_title[] PROGMEM = "Dc/Dcc Controler V0.7";
const char str_copyright[] PROGMEM = "By Thierry Paris";
#endif

const char * const string_table[] PROGMEM
{
	str_title,
	str_copyright,
	str_dc,
	str_dcc,
	str_stop,
	str_stop2,
	str_modemodechoice,
	str_modelococtrl,
	str_modeconfig,
	str_yes,
	str_no,
	str_confirm,
	str_pwmfreqency,
	str_locoedit,
	str_longaddress,
	str_locoId,
	str_locoName,
	str_locoSteps,
	str_locoSteps14,
	str_locoSteps28,
	str_locoSteps128,
	str_functionId,
	str_function,
	str_dcslow,
	str_programcv,

#if !defined(ARDUINO_AVR_NANO) && !defined(ARDUINO_AVR_UNO)
	str_saveLoco,
	str_modelocoedit,
	str_loconew,
	str_locodel,
	str_locochange,
	str_locoselect,
	str_resetconfig
#endif
};
#endif

#define STR_TITLE			0
#define STR_COPYRIGHT		1
#define STR_DC				2
#define STR_DCC				3
#define STR_STOP			4
#define STR_STOP2			5
#define STR_MODEMODECHOICE	6
#define STR_MODELOCOCTRL	7
#define STR_MODECONFIG		8
#define STR_YES				9
#define STR_NO				10
#define STR_CONFIRM			11
#define STR_PWMFREQCFG		12
#define STR_LOCOEDIT		13
#define STR_LONGADDRESS		14
#define STR_LOCOID			15
#define STR_LOCONAME		16
#define STR_LOCOSTEPS		17
#define STR_LOCOSTEPS14		18
#define STR_LOCOSTEPS28		19
#define STR_LOCOSTEPS128	20
#define STR_FUNCTIONID		21
#define STR_FUNCTION		22
#define STR_DCSLOW			23
#define STR_MODIFYCV		24

#define STR_SAVELOCO		25
#define STR_MODELOCOEDIT	26
#define STR_LOCONEW			27
#define STR_LOCOREMOVE		28
#define STR_LOCOCHANGE		29
#define STR_LOCOSELECT		30
#define STR_RESETCONFIG		31

// IDs for function windows not related to strings.

#define STR_FUNCTIONID0		40
#define STR_FUNCTIONID1		41
#define STR_FUNCTIONID2		42
#define STR_FUNCTIONID3		43
#define STR_FUNCTIONID4		44
#define STR_FUNCTIONID5		45
#define STR_FUNCTIONID6		46
#define STR_FUNCTIONID7		47

//////////////////////////////////////////
//  Exclusion area
//
// You can exclude some parts of library here, to avoid losing program and data memory
// on parts you don t use.
// For example, if you don t want buttons commander
// just uncomment the line #define NO_BUTTONSCOMMANDER by removing // at the beginning.
//
// Arduino IDE build of list of the files to compile from the content of the library directory
// in a makefile and give it to Gcc. So Gcc compiles all the files of the directory
// even if its content is not used. The result is, if an unused source file contains
// static declarations, these statics will be allocated a this memory will be lost.
// The only solution I have found is to rename the source files to something IDE and Gcc don t know...
//
// So if you want to lose less memory, you can rename the linked files
// from .cpp to .cpp.ori, and from .hpp to .hpp.ori.
// See below the file names related to each exclusion:
//
//#define NO_LOCOMOTIVE
//#define NO_SCREEN

/////////////////////////////////////

enum DcDcc
{
	DcChangeStopped,
	PanicStopped,
	ProgramMode,
	Dc,
	Dcc
};

#include "Locomotive.hpp"

#include "ControlerDc.hpp"
#include "ControlerDccpp.hpp"
#include "DCCItemList.hpp"

#include "Handle.hpp"

// Copy from cmdrarduino ...
#define DCC_SHORT_ADDRESS           0x00
#define DCC_LONG_ADDRESS            0x01

// LcdUi defines
#define WINDOWTYPE_INTERRUPT_DCDCC		10
#define WINDOWTYPE_INTERRUPT_EMERGENCY	11
#define EEPROM_DDC_CONFIG_SIZE			64

#include <LcdEvents.h>

class DcDccControler
{
protected:
	// Main
	static uint8_t DcDccSignalPinMain;	// To be able to change Dc frequency, the pin used must be 9 or 10 !
	static uint8_t SignalEnablePinMain;
	static uint8_t DirectionMotorA;
	static uint8_t CurrentMonitorMain;

	// all at 255 if prog track not used.
	static uint8_t DccSignalPinProg;
	static uint8_t SignalEnablePinProg;
	static uint8_t DirectionMotorB;
	static uint8_t CurrentMonitorProg;

public:
	static DcDcc dcType;
	static DcDcc dcTypeAtStart;
	static Handle **pHandleList;
	static int handleAddcounter;
	static Controler *pControler;

private:
	DcDccControler() {}

	static bool beginFinished;
	static void beforeFirstLoop();

public:

	static void begin(uint8_t inDcDccSelectPin = 255);
	static void beginMain(uint8_t DirectionMotor, uint8_t DccSignalPin, uint8_t SignalEnablePin, uint8_t CurrentMonitor);
	static void beginProg(uint8_t DirectionMotor, uint8_t DccSignalPin, uint8_t SignalEnablePin, uint8_t CurrentMonitor);
	static void AddHandle(Handle *pHandle);
	static byte IndexOf(Handle *inpHandle);

	static void loop(unsigned long inEvent, int inData);

	static void ConfigLoad();
	static int ConfigSave();
	static void ConfigReset();

#ifdef DDC_DEBUG_MODE
	static void CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc);
#endif
};
#endif
