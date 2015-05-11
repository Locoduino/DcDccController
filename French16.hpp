//-------------------------------------------------------------------
#ifndef __DDCLanguage_H__
#define __DDCLanguage_H__
//-------------------------------------------------------------------

#ifdef VISUALSTUDIO
#include "VStudioMfc/arduino.h"
#else
#include "arduino.h"
#include <avr/pgmspace.h>
#endif

// French language for 16 characters display.

										  // 0123456789012345
const char  str_title[] PROGMEM =			"Dc/Dcc Cont V0.1";
const char  str_copyright[] PROGMEM =		"Thierry Paris";
const char  str_dc[] PROGMEM=				"Dc";
const char  str_dcc[] PROGMEM=				"Dcc";
const char  str_cv[] PROGMEM=				"Cv";
const char  str_stop[] PROGMEM =			"Arret general";
const char  str_stop2[] PROGMEM =			"Appuyer encore";
const char  str_dcdcc[] PROGMEM =			"Change DC/DCC";
const char  str_dcdcc2[] PROGMEM =			"Redemarrer/annul";
const char  str_modemodechoice[] PROGMEM =	"Choix du mode:";
const char  str_modelococtrl[] PROGMEM =	 "Controle loco";
const char  str_modelocoedit[] PROGMEM =	 "Edition locos";
const char  str_modelocoprogram[] PROGMEM =	 "Program loco";
const char  str_modeacccontrol[] PROGMEM = "";//"Controle acces";
const char  str_modeaccedit[] PROGMEM = "";// "Edition access";
const char  str_modeconfig[] PROGMEM =		 "Config DDC";
const char  str_locoselect[] PROGMEM =		"Choix de loco";
const char  str_resetconfig[] PROGMEM =		 "Reset Config";
const char  str_yes[] PROGMEM =				"oui";
const char  str_no[] PROGMEM =				"non";
const char  str_confirm[] PROGMEM =			"Sur ?";
const char  str_bkltconfig[] PROGMEM =		 "Retro eclairage";
const char  str_loconew[] PROGMEM =			 "Nouvelle Loco";
const char  str_locodel[] PROGMEM =			 "Supprimer loco";
const char  str_locoedit[] PROGMEM =		 "Editer loco";
const char  str_locochange[] PROGMEM =		 "Changer loco";
const char  str_longaddress[] PROGMEM =		 "Adresse longue";
const char  str_locoId[] PROGMEM =			 "Adresse";
const char  str_locoName[] PROGMEM =		 "Nom";
const char  str_locoSteps[] PROGMEM =		 "Nb Pas vitesse";
const char  str_locoSteps14[] PROGMEM =		 "14";
const char  str_locoSteps28[] PROGMEM =		 "28";
const char  str_locoSteps128[] PROGMEM =	 "128";
const char  str_handleCfgDigits[] PROGMEM =  "digits adresse";
const char  str_functionnew[] PROGMEM =		 "Nouv fonction";
const char  str_functiondel[] PROGMEM =		 "Supp fonction";
const char  str_functionedit[] PROGMEM =	 "Edit fonction";
const char  str_functionsel[] PROGMEM =		"Choix fonction";
const char  str_locofunctions[] PROGMEM =	"Choix fonction";
const char  str_functionId[] PROGMEM =		"Adresse";
const char  str_functionName[] PROGMEM =	"Nom";
const char  str_functionType[] PROGMEM =	"Type";
const char  str_functionLights[] PROGMEM =	 "Eclairage";
const char str_functionFrontLights[] PROGMEM="Eclai. avant";
const char str_functionRearLights[] PROGMEM= "Eclai. arriere";
const char str_functionCabinLights[] PROGMEM = "Eclai. cabine";
const char str_saveLoco[] PROGMEM =			"Sauver la loco ?";
										//   0123456789012345


//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
