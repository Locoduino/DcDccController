//-------------------------------------------------------------------
#ifndef __DDCLanguage_H__
#define __DDCLanguage_H__
//-------------------------------------------------------------------

#include "arduino.h"

// English language	for 16 characters display.

#define SHORTLENGTH_MSG
										//   0123456789012345
const char  str_dc[] PROGMEM =				"Dc";
const char  str_dcc[] PROGMEM=				"Dcc";
const char  str_stop[] PROGMEM=				"Stop Alim";
const char  str_stop2[] PROGMEM=			"Push cancel";
const char  str_modemodechoice[] PROGMEM =	"DDC menu:";
const char  str_modelococtrl[] PROGMEM =	"Control loco";
const char  str_modeconfig[] PROGMEM =		"Config DDC";
const char  str_yes[] PROGMEM =				"yes";
const char  str_no[] PROGMEM =				"no";
const char  str_confirm[] PROGMEM =			"Sure?";
const char  str_pwmfreqency[] PROGMEM =		"PWM Frequency";
const char  str_locoedit[] PROGMEM =		"Edit loco";
const char  str_longaddress[] PROGMEM =		"Long Address";
const char  str_locoId[] PROGMEM =			"Address";
const char  str_locoName[] PROGMEM =		"Name";
const char  str_locoSteps[] PROGMEM =		"Speed Steps";
const char  str_locoSteps14[] PROGMEM =		"14";
const char  str_locoSteps28[] PROGMEM =		"28";
const char  str_locoSteps128[] PROGMEM =	"128";
const char  str_functionId[] PROGMEM =		"Functions";
const char  str_function[] PROGMEM =		"Function";
const char  str_dcslow[] PROGMEM =			"Dc slow";
const char  str_programcv[] PROGMEM =		"Modify Cv";

// Extended part of messages (MEGA)

const char  str_saveLoco[] PROGMEM =		"Save the loco ?";
const char  str_modelocoedit[] PROGMEM =	"Edit locos";
const char  str_loconew[] PROGMEM =			"New Loco";
const char  str_locodel[] PROGMEM =			"Remove loco";
const char  str_locochange[] PROGMEM =		"Change loco";
const char  str_locoselect[] PROGMEM =		"Select loco";
const char  str_resetconfig[] PROGMEM =		"Reset Config";
										//   0123456789012345


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
																		 