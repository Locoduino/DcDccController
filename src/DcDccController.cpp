/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"
#include "Locomotive.hpp"
#include "EEPROM.h"		// only for EEPROM.length() !

DcDcc  DcDccControler::dcType;
DcDcc  DcDccControler::dcTypeAtStart;
Handle ** DcDccControler::pHandleList;
int  DcDccControler::handleAddcounter;
Controler * DcDccControler::pControler;
bool DcDccControler::beginFinished;

#ifdef DDC_DEBUG_MODE
void DcDccControler::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
	if (inIndex < 0 || inIndex > DcDccControler::handleAddcounter)
	{
		Serial.print(F("Index error in "));
		Serial.println(inFunc);
	}
}

void CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *inFunc)
{
	return CheckPinNb(GPIO_to_Arduino_pin(inPin), inFunc);
}

void CheckPinNb(int inPin, const __FlashStringHelper *inFunc)
{
	if (inPin <= 0 || inPin >= NUM_DIGITAL_PINS)
	{
		Serial.print(F("Pin "));
		Serial.print(inPin);
		Serial.print(F(" error in "));
		Serial.println(inFunc);
	}
}
#endif

//#define DDC_DEBUG_MODE

void DcDccControler::AddHandle(Handle *inpHandle)
{
	Handle **pNewList = new Handle*[DcDccControler::handleAddcounter + 1];

	int i = 0;
	if (DcDccControler::pHandleList != NULL)
		for (; i < DcDccControler::handleAddcounter; i++)
			pNewList[i] = DcDccControler::pHandleList[i];

	pNewList[i] = inpHandle;

	DcDccControler::handleAddcounter++;
	delete DcDccControler::pHandleList;
	DcDccControler::pHandleList = pNewList;
}

void DcDccControler::begin(uint8_t inDcDccSelectPin)
{
#ifdef DDC_DEBUG_MODE
	Serial.println(F(""));
	Serial.println(F("Dc/Dcc Controler"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif

	DcDccControler::pHandleList = NULL;
	DcDccControler::handleAddcounter = 0;
	DcDccControler::pControler = NULL;
	DcDccControler::beginFinished = false;

#ifdef DDC_DEBUG_MODE
#ifdef NANOCONTROLER
	Serial.println(F("Setup started (Nano version)..."));
#else
	Serial.println(F("Setup started..."));
	/*if (inDcPWMpin != 3 && inDcPWMpin != 11)
	{
		Serial.println(F("ERROR: The PWM pin must be 3 or 11 for DC frequency adjustment !"));
		Serial.println(F("Any other pin can dramaticaly change the meaning of all time related functions (delay, millis...)"));
	} */
#endif
#ifndef VISUALSTUDIO
	//while (!Serial);
#endif
#endif

	DcDccControler::dcType = Dc;
	if (inDcDccSelectPin == 255)
		DcDccControler::dcType = Dcc;
	if (inDcDccSelectPin > 0 && inDcDccSelectPin < 255)
	{
		pinMode(inDcDccSelectPin, INPUT_PULLUP);
		if (digitalRead(inDcDccSelectPin) == HIGH)
			DcDccControler::dcType = Dcc;
	}

	// Just for let the time to the microC to initialize internals...
	delay(500);

	LcdScreen::YesMsg = STR_YES;
	LcdScreen::NoMsg = STR_NO;

#ifndef NANOCONTROLER
	LcdScreen::BackspaceMsg	= STR_BACK;
#endif

	if (DcDccControler::dcType == Dc)
	{
#ifdef DDC_DEBUG_MODE
		Serial.println(F("Start in Dc mode"));
#endif
		DcDccControler::pControler = new ControlerDc();
	}
	else
	{
#ifdef DDC_DEBUG_MODE
		Serial.println(F("Start in Dcc mode"));
#endif
		DcDccControler::pControler = new ControlerDccpp();
	}

	DcDccControler::pControler->begin();
}

void DcDccControler::beginMain(uint8_t DirectionMotor, uint8_t SignalPin, uint8_t SignalEnablePin, uint8_t CurrentMonitor)
{
	DcDccControler::pControler->beginMain(DirectionMotor, SignalPin, SignalEnablePin, CurrentMonitor);
}

void DcDccControler::beginProg(uint8_t DirectionMotor, uint8_t SignalPin, uint8_t SignalEnablePin, uint8_t CurrentMonitor)
{
	DcDccControler::pControler->beginProg(DirectionMotor, SignalPin, SignalEnablePin, CurrentMonitor);
}

void DcDccControler::beforeFirstLoop()
{
	if (DcDccControler::handleAddcounter == 0)
	{
#ifdef DDC_DEBUG_MODE
		Serial.println(F("No handle declared !"));
#endif
		DcDccControler::dcType = PanicStopped;
		return;
	}

	if (DcDccControler::dcType == Dc)
	{
		// Force to use only the first handle...
		DcDccControler::handleAddcounter = 1;
		// Affect a special loco to this handle.
		DcDccControler::pHandleList[0]->SetControledLocomotive(Locomotive::AnalogLocomotive);
		DcDccControler::pHandleList[0]->MoreLessIncrement = 10;
	}
	else
	{
#ifndef NANOCONTROLER
		EngineShed::begin();
		RollingStock::begin();
#endif
	}

	ConfigLoad();

	// Must be done only when the good value is in Locomotive::FunctionNumber...
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
	{
		DcDccControler::pHandleList[i]->StartUI();
		DcDccControler::pHandleList[i]->StartContent();
	}

	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		DcDccControler::pHandleList[i]->EndSetup();

#ifdef DDC_DEBUG_MODE
	Serial.print(F("*** Setup Finished."));
#ifndef VISUALSTUDIO
	extern uint8_t *__brkval;
	extern uint8_t *__heap_start;

	Serial.print(F("   Memory used = "));
	Serial.print((int)(__brkval == 0 ? (int)&__heap_start : (int)__brkval) - (int)&__heap_start);
	Serial.println(F(" bytes"));
#endif
	Serial.println("");
#endif
}

byte DcDccControler::IndexOf(Handle *inpHandle)
{
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		if (DcDccControler::pHandleList[i] == inpHandle)
			return i;

	return 255;
}

void DcDccControler::loop(unsigned long inEvent, int inData)
{
	if (!DcDccControler::beginFinished)
	{
		DcDccControler::beforeFirstLoop();
		DcDccControler::beginFinished = true;
	}

	if (DcDccControler::pControler != 0)
		DcDccControler::pControler->loop();

	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
	{
		if (DcDccControler::pHandleList[i] != 0)
			DcDccControler::pHandleList[i]->loop(inEvent, inData);
	}
}

void DcDccControler::ConfigLoad()
{
	// The first three bytes must be 'DDC' to assume that there is already a saved configuration
	// in the EEPROM. If not, reset all the EEPROM to 0, and save the 'DDC' identifier.
	if (EEPROMextent.readByte(0) != 'D' || EEPROMextent.readByte(1) != 'D' || EEPROMextent.readByte(2) != 'C')
	{
		ConfigReset();
		ConfigSave();
	}
	else
	{
		if (DcDccControler::dcType == Dc)
		{
			byte dCFrequencyDivisorIndex = EEPROMextent.readByte(3);

			if (dCFrequencyDivisorIndex < NB_PWM_FREQ_11_3)
				// Set frequency, even if the pin is not correct at this moment !
				((ControlerDc *)DcDccControler::pControler)->DCFrequencyDivisorIndex = dCFrequencyDivisorIndex;
		}
		else
		{
			int pos = 4;
#ifndef NANOCONTROLER
			pos = EngineShed::ConfigLoad(pos);
			pos = RollingStock::ConfigLoad(pos);
#endif
		}

		// Must be done only when the good value is in Locomotive::FunctionNumber...
		for (int i = 0; i < DcDccControler::handleAddcounter; i++)
			DcDccControler::pHandleList[i]->ConfigLoad();
	}
}

int DcDccControler::ConfigSave()
{
	EEPROMextent.writeByte(0, 'D');
	EEPROMextent.writeByte(1, 'D');
	EEPROMextent.writeByte(2, 'C');

	int pos = 4;
	if (DcDccControler::dcType == Dc)
		EEPROMextent.writeByte(3, ((ControlerDc *)DcDccControler::pControler)->DCFrequencyDivisorIndex);
	else
	{
#ifndef NANOCONTROLER
		pos = EngineShed::ConfigSave(pos);
		pos = RollingStock::ConfigSave(pos);
#endif
	}

	// Must be done only when the good value is in Locomotive::FunctionNumber...
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		DcDccControler::pHandleList[i]->ConfigSave();

	return pos; // Pos of next writing...
}

void DcDccControler::ConfigReset()
{
#ifndef NANOCONTROLER
	EngineShed::Clear();
	RollingStock::Clear();
#endif

	for (uint16_t i = 0; i < EEPROM.length(); i++)
		EEPROMextent.updateByte(i, 0);

#ifndef NANOCONTROLER
	// For test only
	// For test : populate locos of Engine Shed

	EngineShed::Current.SetDccId(3);
	EngineShed::Current.SetName("Test Loco3");
	EngineShed::Current.SetSteps(128);
	EngineShed::Current.Functions[0].SetDccId(0);
	EngineShed::Current.Functions[1].SetDccId(10);
	EngineShed::Current.Functions[2].SetDccId(255);
	EngineShed::Current.Functions[3].SetDccId(255);
	EngineShed::Current.Functions[4].SetDccId(255);
	EngineShed::Current.Functions[5].SetDccId(255);
	EngineShed::Current.Functions[6].SetDccId(255);
	EngineShed::Current.Functions[7].SetDccId(255);
	DCCItemList.AddLoco(&EngineShed::Current);

	EngineShed::Current.SetDccId(47);
	EngineShed::Current.SetName("X73914");
	EngineShed::Current.SetSteps(128);
	EngineShed::Current.Functions[0].SetDccId(0);
	EngineShed::Current.Functions[1].SetDccId(1);
	DCCItemList.AddLoco(&EngineShed::Current);

	EngineShed::Current.SetDccId(41);
	EngineShed::Current.SetName("ABJ-1");
	EngineShed::Current.SetSteps(128);
	EngineShed::Current.Functions[0].SetDccId(0);
	EngineShed::Current.Functions[1].SetDccId(1);
	DCCItemList.AddLoco(&EngineShed::Current);

	EngineShed::Current.SetDccId(46);
	EngineShed::Current.SetName("BB66000");
	EngineShed::Current.SetSteps(128);
	EngineShed::Current.Functions[0].SetDccId(0);
	EngineShed::Current.Functions[1].SetDccId(255);
	DCCItemList.AddLoco(&EngineShed::Current);

	EngineShed::Current.SetDccId(48);
	EngineShed::Current.SetName("ALCO S4");
	EngineShed::Current.SetSteps(128);
	EngineShed::Current.Functions[0].SetDccId(0);
	EngineShed::Current.Functions[1].SetDccId(1);
	DCCItemList.AddLoco(&EngineShed::Current);
#endif
	EngineShed::RebuildLocos();
}