/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"
#include "Locomotive.hpp"

#ifdef VISUALSTUDIO
char DcDccControler::LastKeyPressed;
#endif

#ifdef DEBUG_MODE
void DcDccControler::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
	if (inIndex < 0 || inIndex > this->handleAddcounter)
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

//#define DEBUG_MODE

DcDccControler DcDccControler::DDc;

DcDccControler::DcDccControler()
{
	this->dcType = PanicStopped;
	this->pHandleList = 0;
	this->handleAddcounter = 0;
	this->pControler = 0;
}

void DcDccControler::AddHandle(Handle *inpHandle)
{
	Handle **pNewList = new Handle*[this->handleAddcounter + 1];

	int i = 0;
	if (this->pHandleList != 0)
		for (; i < this->handleAddcounter; i++)
			pNewList[i] = this->pHandleList[i];

	pNewList[i] = inpHandle;

	this->handleAddcounter++;
	delete this->pHandleList;
	this->pHandleList = pNewList;
}

void DcDccControler::StartSetup(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{
	Serial.begin(115200);
#ifdef DEBUG_MODE
#ifdef NANOCONTROLER
	Serial.println(F("Setup started (nano version)..."));
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

	LcdUi::StartSetup();

	this->dcPWMpin = inDcPWMpin;
	this->dcDirPin = inDcDirPin;

	// Just for let the time to the PIC to initialize internals...
	delay(500);

#ifdef DEBUG_MODE
	Serial.println(F(""));
	Serial.println(F("Dc/Dcc Controler"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
#endif
}

void DcDccControler::EndSetup()
{	
	LcdUi::EndSetup();

	if (this->handleAddcounter == 0)
	{
#ifdef DEBUG_MODE
		Serial.println(F("No handle declared !"));
#endif
		this->dcType = PanicStopped;
		return;
	}

#ifndef NANOCONTROLER
	WindowChooseLoco::ClearChoices();
#endif

#ifdef VISUALSTUDIO
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * this->handleAddcounter), 30, EEPROM_SIZE);
#else
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * this->handleAddcounter), 20, EEPROM_SIZE);
#endif

#ifndef NANOCONTROLER
	WindowChooseLoco::RebuildChoices();
#endif

	Screen::YesMsg = STR_YES;
	Screen::NoMsg = STR_NO;

	for (int i = 0; i < this->handleAddcounter; i++)
	{
		if (this->pHandleList[i]->GetFunctionNumber() < Locomotive::FunctionNumber)
			Locomotive::FunctionNumber = this->pHandleList[i]->GetFunctionNumber();
	}

#ifdef VISUALSTUDIO
	bool IsDc = true;
#else
	bool IsDc = true;
	if (this->pDcDccButton != 0)
		IsDc = ((ButtonsCommanderSwitch *)this->pDcDccButton)->GetLastButtonState();
#endif

	if (IsDc)
	{
#ifdef DEBUG_MODE
		Serial.println(F("Start in Dc mode"));
#endif
		DDC.dcTypeAtStart = Dc;
		DDC.pControler = new ControlerDc();
		// Force to use only the first handle...
		this->handleAddcounter = 1;
		// Affect a special loco to this handle.
		this->pHandleList[0]->SetControledLocomotive(Locomotive::AnalogLocomotive);
		this->pHandleList[0]->MoreLessIncrement = 10;
	}
	else
	{
#ifdef DEBUG_MODE
		Serial.println(F("Start in Dcc mode"));
#endif
		DDC.dcTypeAtStart = Dcc;
		DDC.pControler = new ControlerDcc();
	}

	DDC.dcType = DDC.dcTypeAtStart;
	if (DDC.pControler == 0)
		return;

	ConfigLoad();

	DDC.pControler->Setup(this->dcPWMpin, this->dcDirPin);

	// Must be done only when the good value is in Locomotive::FunctionNumber...
	for (int i = 0; i < this->handleAddcounter; i++)
	{
		this->pHandleList[i]->StartUI();
		this->pHandleList[i]->StartContent();
	}

	for (int i = 0; i < this->handleAddcounter; i++)
		this->pHandleList[i]->EndSetup(DDC.dcType == Dc);

//#ifdef DEBUG_MODE
	Serial.print(F("*** Setup Finished."));
#ifndef VISUALSTUDIO
	extern uint8_t *__brkval;
	extern uint8_t *__heap_start;

	Serial.print(F("   Memory used = "));
	Serial.print((int)(__brkval == 0 ? (int)&__heap_start : (int)__brkval) - (int)&__heap_start);
	Serial.println(F(" bytes"));
#endif
//#endif
}

byte DcDccControler::IndexOf(Handle *inpHandle) const
{
	for (int i = 0; i < DDC.handleAddcounter; i++)
		if (DDC.pHandleList[i] == inpHandle)
			return i;

	return 255;
}

void DcDccControler::Loop()
{
	if (this->pControler != 0)
		this->pControler->Loop();

	bool updateStatus = false;

	if (this->dcType == PanicStopped)
	{
		// if panic stopped, just react to panic button !
		if (this->pPanicButton != 0 && !this->pPanicButton->IsSelectedLastLoop())
			return;

		if (this->dcType == PanicStopped)
		{
#ifdef DEBUG_MODE
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
		if (this->pPanicButton != 0)
			this->pPanicButton->UnselectLastLoop();
		updateStatus = true;
	}
	else
	{
		if (this->pDcDccButton != 0 && this->pDcDccButton->IsSelectedLastLoop())
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
#ifdef DEBUG_MODE
					Serial.println(F("Dc mode change"));
#endif
					this->dcType = DcChangeStopped;
					for (int i = 0; i < this->handleAddcounter; i++)
						this->pHandleList[i]->Interrupt(EVENT_DCDCC);
				}
				updateStatus = true;
			}

			if (this->pDcDccButton != 0)
				this->pDcDccButton->UnselectLastLoop();
		}

		if (this->pPanicButton != 0 && this->pPanicButton->IsSelectedLastLoop())
		{
#ifdef DEBUG_MODE
			Serial.println(F("Panic stopped"));
#endif
			this->dcType = PanicStopped;
			this->pPanicButton->UnselectLastLoop();
			if (this->pControler != 0)
				this->pControler->PanicStop(true);
			for (int i = 0; i < this->handleAddcounter; i++)
				this->pHandleList[i]->Interrupt(EVENT_EMERGENCY);
			updateStatus = true;
		}
	}

	for (int i = 0; i < this->handleAddcounter; i++)
	{
		if (this->pHandleList[i] != 0)
			this->pHandleList[i]->Loop();
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
		if (this->dcType == Dc)
		{
			uint16_t dCFrequencyDivisor;
			EEPROMextent.readAnything(3, dCFrequencyDivisor);
			// Set frequency, even if the pin is not correct at this moment !
			((ControlerDc *)DDC.pControler)->SetFrequencyDivisor(dCFrequencyDivisor);
		}

		// Must be done only when the good value is in Locomotive::FunctionNumber...
		for (int i = 0; i < this->handleAddcounter; i++)
		{
			this->pHandleList[i]->ConfigLoad();
		}
	}
}

int DcDccControler::ConfigSave()
{
	EEPROMextent.write(0, 'D');
	EEPROMextent.write(1, 'D');
	EEPROMextent.write(2, 'C');

	if (this->dcType == Dc)
		EEPROMextent.writeAnything(3, ((ControlerDc *)DDC.pControler)->GetFrequencyDivisor());
	return 0;
}

void DcDccControler::ConfigReset()
{
	for (int i = 0; i < EEPROM_SIZE; i++)
		EEPROMextent.update(i, 0);
}