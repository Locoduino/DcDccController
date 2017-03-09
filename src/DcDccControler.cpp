/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"
#include "Locomotive.hpp"

uint8_t  DcDccControler::dcPWMpin;
uint8_t  DcDccControler::dcDirPin;
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

static void CheckPinNb(GPIO_pin_t inPin, const __FlashStringHelper *inFunc)
{
	return CheckPinNb(GPIO_to_Arduino_pin(inPin), inFunc);
}

void static CheckPinNb(int inPin, const __FlashStringHelper *inFunc)
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
	if (DcDccControler::pHandleList != 0)
		for (; i < DcDccControler::handleAddcounter; i++)
			pNewList[i] = DcDccControler::pHandleList[i];

	pNewList[i] = inpHandle;

	DcDccControler::handleAddcounter++;
	delete DcDccControler::pHandleList;
	DcDccControler::pHandleList = pNewList;
}

void DcDccControler::begin(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{
	Serial.begin(115200);

	DcDccControler::dcType = PanicStopped;
	DcDccControler::pHandleList = 0;
	DcDccControler::handleAddcounter = 0;
	DcDccControler::pControler = 0;
	DcDccControler::beginFinished = false;

#ifdef DDC_DEBUG_MODE
#ifdef NANOCONTROLER
	Serial.println(F("Setup started (Nano version)..."));
#else
	Serial.println(F("Setup started..."));
	if (inDcPWMpin != 3 && inDcPWMpin != 11)
	{
		Serial.println(F("ERROR: The PWM pin must be 3 or 11 for DC frequency adjustment !"));
		Serial.println(F("Any other pin can dramaticaly change the meaning of all time related functions (delay, millis...)"));
	}
#endif
#ifndef VISUALSTUDIO
	//while (!Serial);
#endif
#endif

	pinMode(inDcPWMpin, OUTPUT);
	pinMode(inDcDirPin, OUTPUT);
	analogWrite(inDcPWMpin, 0);

	DcDccControler::dcPWMpin = inDcPWMpin;
	DcDccControler::dcDirPin = inDcDirPin;

	// Just for let the time to the PIC to initialize internals...
	delay(500);

#ifdef DDC_DEBUG_MODE
	Serial.println(F(""));
	Serial.println(F("Dc/Dcc Controler"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif
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

#ifndef NANOCONTROLER
	WindowChooseLoco::ClearChoices();
#endif

#ifdef VISUALSTUDIO
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * DcDccControler::handleAddcounter), 30, EEPROM.length());
#else
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * this->handleAddcounter), 20, EEPROM_SIZE);
#endif

#ifndef NANOCONTROLER
	WindowChooseLoco::RebuildChoices();
#endif

	LcdScreen::YesMsg = STR_YES;
	LcdScreen::NoMsg = STR_NO;

#ifdef VISUALSTUDIO
	bool IsDc = true;
#else
	bool IsDc = true;
	if (this->pDcDccButton != 0)
		IsDc = ((ButtonsCommanderSwitch *)this->pDcDccButton)->GetLastButtonState();
#endif

	if (IsDc)
	{
#ifdef DDC_DEBUG_MODE
		Serial.println(F("Start in Dc mode"));
#endif
		DcDccControler::dcTypeAtStart = DcDcc::Dc;
		DcDccControler::pControler = new ControlerDc();
		// Force to use only the first handle...
		DcDccControler::handleAddcounter = 1;
		// Affect a special loco to this handle.
		DcDccControler::pHandleList[0]->SetControledLocomotive(Locomotive::AnalogLocomotive);
		DcDccControler::pHandleList[0]->MoreLessIncrement = 10;
	}
	else
	{
#ifdef DDC_DEBUG_MODE
		Serial.println(F("Start in Dcc mode"));
#endif
		DcDccControler::dcTypeAtStart = DcDcc::Dcc;
		DcDccControler::pControler = new ControlerDcc();
	}

	DcDccControler::dcType = DcDccControler::dcTypeAtStart;
	if (DcDccControler::pControler == 0)
		return;

	ConfigLoad();

	DcDccControler::pControler->begin(DcDccControler::dcPWMpin, DcDccControler::dcDirPin);

	// Must be done only when the good value is in Locomotive::FunctionNumber...
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
	{
		DcDccControler::pHandleList[i]->StartUI();
		DcDccControler::pHandleList[i]->StartContent();
	}

	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		DcDccControler::pHandleList[i]->EndSetup(DcDccControler::dcType == Dc);

#ifdef DDC_DEBUG_MODE
	Serial.print(F("*** Setup Finished."));
#ifndef VISUALSTUDIO
	extern uint8_t *__brkval;
	extern uint8_t *__heap_start;

	Serial.print(F("   Memory used = "));
	Serial.print((int)(__brkval == 0 ? (int)&__heap_start : (int)__brkval) - (int)&__heap_start);
	Serial.println(F(" bytes"));
#endif
#endif
}

byte DcDccControler::IndexOf(Handle *inpHandle)
{
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		if (DcDccControler::pHandleList[i] == inpHandle)
			return i;

	return 255;
}

void DcDccControler::loop(unsigned long inEvent)
{
	if (!DcDccControler::beginFinished)
	{
		DcDccControler::beforeFirstLoop();
		DcDccControler::beginFinished = true;
	}

	if (DcDccControler::pControler != 0)
		DcDccControler::pControler->loop();

	bool updateStatus = false;

#if 0
	if (this->dcType == PanicStopped)
	{
		// if panic stopped, just react to panic button !
		/*if (this->pPanicButton != 0 && !this->pPanicButton->IsSelectedLastLoop())
			return;*/

		if (this->dcType == PanicStopped)
		{
#ifdef DDC_DEBUG_MODE
			Serial.println(F("Panic canceled"));
#endif
			// return to starting normal mode.
			this->dcType = this->dcTypeAtStart;
			if (this->pControler != 0)
				this->pControler->PanicStop(false);
			for (int i = 0; i < this->handleAddcounter; i++)
				this->pHandleList[i]->InterruptEnd();
		}
		else
		{
			this->dcType = PanicStopped;
			if (this->pControler != 0)
				this->pControler->PanicStop(true);
			for (int i = 0; i < this->handleAddcounter; i++)
				this->pHandleList[i]->Interrupt(EVENT_EMERGENCY);
		}
		/*if (this->pPanicButton != 0)
			this->pPanicButton->UnselectLastLoop();*/
		updateStatus = true;
	}
	else
	{
		if (this->pDcDccButton != 0/* && this->pDcDccButton->IsSelectedLastLoop()*/)	
		{
			if (this->dcType != PanicStopped)
			{
				if (this->dcType == DcChangeStopped)
				{
					this->dcType = this->dcTypeAtStart;
					for (int i = 0; i < this->handleAddcounter; i++)
						this->pHandleList[i]->InterruptEnd();
				}
				else
				{
#ifdef DDC_DEBUG_MODE
					Serial.println(F("Dc mode change"));
#endif
					this->dcType = DcChangeStopped;
					for (int i = 0; i < this->handleAddcounter; i++)
						this->pHandleList[i]->Interrupt(EVENT_DCDCC);
				}
				updateStatus = true;
			}

			/*if (this->pDcDccButton != 0)
				this->pDcDccButton->UnselectLastLoop();*/
		}

		if (this->pPanicButton != 0/* && this->pPanicButton->IsSelectedLastLoop()*/)
		{
#ifdef DDC_DEBUG_MODE
			Serial.println(F("Panic stopped"));
#endif
			this->dcType = PanicStopped;
			//this->pPanicButton->UnselectLastLoop();
			if (this->pControler != 0)
				this->pControler->PanicStop(true);
			for (int i = 0; i < this->handleAddcounter; i++)
				this->pHandleList[i]->Interrupt(EVENT_EMERGENCY);
			updateStatus = true;
		}
	}
#endif
	for (int i = 0; i < DcDccControler::handleAddcounter; i++)
	{
		if (DcDccControler::pHandleList[i] != 0)
			DcDccControler::pHandleList[i]->loop(inEvent);
	}
}

void DcDccControler::ConfigLoad()
{
	// The first three bytes must be 'DDC' to assume that there is already a saved configuration
	// in the EEPROM. If not, reset all the EEPROM to 0, and save the 'DDC' identifier.
	if (EEPROMextent.read(0) != 'D' || EEPROMextent.read(1) != 'D' || EEPROMextent.read(2) != 'C')
	{
		ConfigReset();
		ConfigSave();
	}
	else
	{
		if (DcDccControler::dcType == Dc)
		{
			uint16_t dCFrequencyDivisor;
			EEPROMextent.readAnything(3, dCFrequencyDivisor);
			// Set frequency, even if the pin is not correct at this moment !
			((ControlerDc *)DcDccControler::pControler)->SetFrequencyDivisor(dCFrequencyDivisor);
		}

		// Must be done only when the good value is in Locomotive::FunctionNumber...
		for (int i = 0; i < DcDccControler::handleAddcounter; i++)
		{
			DcDccControler::pHandleList[i]->ConfigLoad();
		}
	}
}

int DcDccControler::ConfigSave()
{
/*	EEPROMextent.write(0, 'D');
	EEPROMextent.write(1, 'D');
	EEPROMextent.write(2, 'C');

	if (DcDccControler::dcType == Dc)
		EEPROMextent.writeAnything(3, ((ControlerDc *)DcDccControler::pControler)->GetFrequencyDivisor());*/
	return 0;
}

void DcDccControler::ConfigReset()
{
	for (int i = 0; i < EEPROM.length(); i++)
		EEPROMextent.update(i, 0);
}