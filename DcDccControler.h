//-------------------------------------------------------------------
#ifndef __dcDccControler_H__
#define __dcDccControler_H__
//-------------------------------------------------------------------

////////////////////////////////////////////////////////
// Add a '//' at the beginning of the line to be in 
// release mode.
//#define DEBUG_MODE

///////////////////////////////////////////////////////
// Verbose mode lets you see all actions done by the 
// library, but with a real flood of text to console...
// Has no effect if DEBUG_MODE is not activated.
//#define DEBUG_VERBOSE_MODE

#define GPIO2_PREFER_SPEED    1

#ifdef VISUALSTUDIO
#include "arduino.h"
#include "Serial.hpp"
#include "VStudioLcdUI\arduino2.hpp"
#else
#include "arduino2.hpp"
#endif

#ifndef __LcdUi_H__
#include "../LcdUi/LcdUI.h"
#endif

#ifndef __EEPROMextent_h__
#include "../EEPROMextent/EEPROMextent.h"
#endif

#ifdef DEBUG_MODE
static void CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *inFunc);
static void CheckPinNb(int inPin, const __FlashStringHelper *inFunc);

#define CHECKPIN(val, text)		CheckPinNb(cval, text)
#else
#define CHECKPIN(val, text)
#endif

//extern const char str_special_rst[] PROGMEM;
extern const char * const DDC_config_table[] PROGMEM;
extern const char * const string_table[] PROGMEM;

#if defined(__DDCLanguage_H__)

// Unlocalized special strings
const char str_special_rst[] PROGMEM = "$rst";

const char * const string_table[] PROGMEM
{
	str_title,
	str_copyright,
	str_dc,
	str_dcc,
	str_cv,
	str_stop,
	str_stop2,
	str_dcdcc,
	str_dcdcc2,
	str_modemodechoice,
	str_modelococtrl,
	str_modelocoedit,
	str_modelocoprogram,
	str_modeacccontrol,
	str_modeaccedit,
	str_modeconfig,
	str_locoselect,
	str_resetconfig,
	str_yes,
	str_no,
	str_confirm,
	str_bkltconfig,
	str_loconew,
	str_locodel,
	str_locoedit,
	str_locochange,
	str_longaddress,
	str_locoId,
	str_locoName,
	str_locoSteps,
	str_locoSteps14,
	str_locoSteps28,
	str_locoSteps128,
	str_handleCfgDigits,
	str_functionnew,
	str_functiondel,
	str_functionedit,
	str_functionsel,
	str_locofunctions,
	str_functionId,
	str_saveLoco,
	str_special_rst
};

// List of available main config options

const char * const DDC_config_table[] PROGMEM =
{
	str_special_rst	// option to fully reset the configuration.
};
#endif

#define STR_TITLE		0
#define STR_COPYRIGHT	1
#define STR_DC			2
#define STR_DCC			3
#define STR_CV			4
#define STR_STOP		5
#define STR_STOP2		6
#define STR_DCDCC		7
#define STR_DCDCC2		8
#define STR_MODEMODECHOICE	9
#define STR_MODELOCOCTRL	10
#define STR_MODELOCOEDIT	11
#define STR_MODELOCOPROGRAM	12
#define STR_MODEACCCONTROL	13
#define STR_MODEACCEDIT		14
#define STR_MODECONFIG		15
#define STR_LOCOSELECT		16
#define STR_RESETCONFIG		17
#define STR_YES				18
#define STR_NO				19
#define STR_CONFIRM			20
#define STR_BACKLIGHTCFG	21
#define STR_LOCONEW			22
#define STR_LOCOREMOVE		23
#define STR_LOCOEDIT		24
#define STR_MODELOCOCHANGE	25
#define STR_LONGADDRESS		26
#define STR_LOCOID			27
#define STR_LOCONAME		28
#define STR_LOCOSTEPS		29
#define STR_LOCOSTEPS14		30
#define STR_LOCOSTEPS28		31
#define STR_LOCOSTEPS128	32
#define STR_HANDLECFGDIGITS	33
#define STR_FUNCTIONNEW		34
#define STR_FUNCTIONREMOVE	35
#define STR_FUNCTIONEDIT	36
#define STR_FUNCTIONSELECT	37
#define STR_LOCOFUNCTIONS	38
#define STR_FUNCTIONID		39
#define STR_SAVELOCO		40

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
//NO_BUTTONSCOMMANDER
//	ButtonsCommander.cpp
//	ButtonsCommander.hpp
//	ButtonsCommanderAnalogPushes.cpp
//	ButtonsCommanderAnalogPushes.hpp
//	ButtonsCommanderAnalogPushesItem.cpp
//	ButtonsCommanderAnalogPushesItem.hpp
//	ButtonsCommanderButtons.cpp
//	ButtonsCommanderButtons.hpp
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp
//
//NO_BUTTONSCOMMANDERPUSH
//	ButtonsCommanderPush.cpp
//	ButtonsCommanderPush.hpp
//
//NO_BUTTONSCOMMANDERENCODER
//	ButtonsCommanderEncoder.cpp
//	ButtonsCommanderEncoder.hpp
//
//NO_BUTTONSCOMMANDERANALOGPUSH
//	ButtonsCommanderAnalogPushes.cpp
//	ButtonsCommanderAnalogPushes.hpp
//	ButtonsCommanderAnalogPushesItem.cpp
//	ButtonsCommanderAnalogPushesItem.hpp
//
//NO_BUTTONSCOMMANDERSWITCH
//	ButtonsCommanderSwitch.cpp
//	ButtonsCommanderSwitch.hpp
//
//NO_BUTTONSCOMMANDERPOTENTIOMETER
//	ButtonsCommanderPotentiometer.cpp
//	ButtonsCommanderPotentiometer.hpp

//#define NO_ACCESSORY
//#define NO_LOCOMOTIVE
//#define NO_SCREEN

//#define NO_COMMANDER
//#define NO_BUTTONSCOMMANDER
//#define NO_BUTTONSCOMMANDERENCODER
//#define NO_BUTTONSCOMMANDERPUSH
#define NO_BUTTONSCOMMANDERANALOGPUSH
//#define NO_BUTTONSCOMMANDERSWITCH
#define NO_BUTTONSCOMMANDERPOTENTIOMETER

#ifdef NO_COMMANDER
#ifndef NO_BUTTONSCOMMANDER
#define NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER
#define NO_BUTTONSCOMMANDERENCODER
#endif
#ifndef NO_BUTTONSCOMMANDERPUSH
#define NO_BUTTONSCOMMANDERPUSH
#endif
#ifndef NO_BUTTONSCOMMANDERANALOGPUSH
#define NO_BUTTONSCOMMANDERANALOGPUSH
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#define NO_BUTTONSCOMMANDERSWITCH
#endif
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#define NO_BUTTONSCOMMANDERPOTENTIOMETER
#endif
#endif
#endif

/////////////////////////////////////

#include "Handle.hpp"
#ifndef NO_ACCESSORY
#include "Accessory.hpp"
#endif
#ifndef NO_COMMANDER
#include "Commander.hpp"
#endif

#ifndef NO_COMMANDER
#ifndef NO_BUTTONSCOMMANDER
#ifndef NO_BUTTONSCOMMANDERENCODER
#include "ButtonsCommanderEncoder.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPUSH
#include "ButtonsCommanderPush.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERANALOGPUSH
#include "ButtonsCommanderAnalogPushes.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERSWITCH
#include "ButtonsCommanderSwitch.hpp"
#endif
#ifndef NO_BUTTONSCOMMANDERPOTENTIOMETER
#include "ButtonsCommanderPotentiometer.hpp"
#endif
#endif

#ifndef NO_BUTTONSCOMMANDER
#include "ButtonsCommander.hpp"
#endif
#endif

#include "ControlerDc.hpp"
#include "ControlerDcc.hpp"
#include "DCCItemList.hpp"

// Copy from cmdrarduino ...
#define DCC_SHORT_ADDRESS           0x00
#define DCC_LONG_ADDRESS            0x01

// LcdUI defines
#define EVENT_DCDCC		100
#define EVENT_EMERGENCY	101

#define WINDOWTYPE_INTERRUPT_DCDCC		100
#define WINDOWTYPE_INTERRUPT_EMERGENCY	101
#define EEPROM_DDC_CONFIG_SIZE			64

enum DcDcc
{
	DcChangeStopped,
	PanicStopped,
	Dc,
	Dcc
};

class DcDccControler
{
protected:
	uint8_t dcPWMpin;
	uint8_t dcDirPin;

public:
	DcDcc dcType;
	DcDcc dcTypeAtStart;
	Handle **pHandleList;
	int handleAddcounter;
	Controler *pControler;

#ifdef VISUALSTUDIO
	static char LastKeyPressed;
#endif

private:
	ButtonsCommanderButton *pDcDccButton;	// If null, always Dcc...
	ButtonsCommanderButton *pPanicButton;	// If null, no emergency !

public:
	static DcDccControler DDc;
	static ControlerDc DcControler;
	static ControlerDcc DccControler;

public:
	DcDccControler();

	void StartSetup(uint8_t inDcPWMpin = 0, uint8_t inDcDirPin = 0);
	void EndSetup();
	void AddHandle(Handle *pHandle);
	byte IndexOf(Handle *inpHandle) const;
	inline void SetDcDccButton(ButtonsCommanderButton *inpDcDccButton) { this->pDcDccButton = inpDcDccButton; }
	inline void SetPanicButton(ButtonsCommanderButton *inpPanicButton) { this->pPanicButton = inpPanicButton; }

	void Loop();

	void LoadConfig();
	int SaveConfig();
	void ResetConfig();

#ifdef DEBUG_MODE
	void CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc);
#endif
};

#define DDC DcDccControler::DDc
#endif