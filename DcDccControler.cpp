/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Base functions of the library>
*************************************************************/

#include "DcDccControler.h"
#include "WindowChooseLoco.hpp"
#include "Locomotive.hpp"

#ifndef VISUALSTUDIO
#include "MemoryFree.hpp"
#else
char DcDccControler::LastKeyPressed;
#endif

static int freemem = 0;

#ifdef DEBUG_MODE
void DcDccControler::CheckIndex(unsigned char inIndex, const __FlashStringHelper *inFunc)
{
	if (inIndex < 0 || inIndex > this->handleAddcounter)
	{
		Serial.print(F("Index error in "));
		Serial.println(inFunc);
	}
}
#endif
#ifdef DEBUG_MODE
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

DcDccControler DcDccControler::DDc;
ControlerDc DcDccControler::DcControler;
ControlerDcc DcDccControler::DccControler;

char DcDccControler::buffer[80];

DcDccControler::DcDccControler()
{
	this->dcType = PanicStopped;
	this->pHandleList = 0;
	this->handleAddcounter = 0;
}

char *DcDccControler::GetString(int inString)
{
#ifdef VISUALSTUDIO
	strcpy_s(buffer, 80, string_table[inString]);
#else
	strcpy_P(buffer, (char*)pgm_read_word(&string_table[inString]));
#endif

	return buffer;
}

void DcDccControler::AddHandle(Handle *inpHandle)
{
	Handle **pNewList = new Handle*[this->handleAddcounter + 1];

	int i = 0;
	if (this->pHandleList != 0)
		for (; i < this->handleAddcounter; i++)
			pNewList[i] = this->pHandleList[i];

	pNewList[i] = inpHandle;
	inpHandle->handleNumber = this->handleAddcounter;

	this->handleAddcounter++;
	delete this->pHandleList;
	this->pHandleList = pNewList;
}

void DcDccControler::StartSetup(uint8_t inDcPWMpin, uint8_t inDcDirPin)
{
#ifdef DEBUG_MODE
	Serial.begin(115200);
	Serial.println("Setup started...");
#ifndef VISUALSTUDIO
	//while (!Serial);
#endif

	LcdUi::StartSetup();

	//this->EEPROMversion = EEPROM.read(0);

	this->dcPWMpin = inDcPWMpin;
	this->dcDirPin = inDcDirPin;

	// Just for let the time to the PIC to initialize internals...
	delay(500);

	Serial.println(F(""));
	Serial.println(F("Dc/Dcc Controler"));
	Serial.println(F("Developed by Thierry Paris."));
	Serial.println(F(""));

	Serial.println(F("*** Setup started."));
	freemem = freeMemory();
#endif
}

void DcDccControler::EndSetup()
{	
	LcdUi::EndSetup();

	if (this->handleAddcounter == 0)
	{
		Serial.println(F("No handle declared !"));
		this->dcType = PanicStopped;
		return;
	}

#ifdef VISUALSTUDIO
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * this->handleAddcounter), 30, EEPROM_SIZE);
#else
	DCCItemList.Setup(EEPROM_DDC_CONFIG_SIZE + (EEPROM_DDC_HANDLE_CONFIG_SIZE * this->handleAddcounter), 20, EEPROM_SIZE);
#endif

	WindowChooseLoco::ClearChoices();
	WindowChooseLoco::RebuildChoices();

	Screen::YesMsg = STR_YES;
	Screen::NoMsg = STR_NO;

	for (int i = 0; i < this->handleAddcounter; i++)
	{
		this->pHandleList[i]->ConfigLoad();
		this->pHandleList[i]->StartUI();
	}

#ifdef VISUALSTUDIO
	bool IsDc = false;
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
		DDC.pControler = &DcControler;
		// Force to use only the first handle...
		this->handleAddcounter = 1;
		// Affect a special loco to this handle.
		this->pHandleList[0]->SetLocomotive(Locomotive::AnalogLocomotive);
	}
	else
	{
#ifdef DEBUG_MODE
		Serial.println(F("Start in Dcc mode"));
#endif
		DDC.dcTypeAtStart = Dcc;
		DDC.pControler = &DccControler;

	/*
		for (int i = 0; i < this->handleAddcounter; i++)
			this->pHandleList[i]->mode = LocomotiveEdit;
		if (Locomotive::GetLocomotivesSize() > 0)
		{
			Locomotive *pLoco = Locomotive::GetLocomotive(0);
			if (pLoco != 0)
			{
				this->pHandleList[0]->SetLocomotive(*pLoco);
				DDC.mode = LocomotiveControl;
			}
		}*/
	}

	DDC.dcType = DDC.dcTypeAtStart;
	DDC.pControler->Setup(this->dcPWMpin, this->dcDirPin);

#ifdef DEBUG_MODE
	Serial.print(F("*** Setup Finished."));
	Serial.print(F("   Memory used = "));
	Serial.print(freemem - freeMemory());
	Serial.println(F(" bytes"));
#endif
}

void DcDccControler::Loop()
{
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
			this->dcType = DcChangeStopped;
			// return to starting normal mode.
			this->dcType = this->dcTypeAtStart;
			this->pControler->PanicStop(false);
			for (int i = 0; i < this->handleAddcounter; i++)
				this->pHandleList[i]->InterruptEnd();
		}
		else
		{
			this->dcType = PanicStopped;
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

void DcDccControler::LoadConfig()
{

}

int DcDccControler::SaveConfig()
{

	return 0;
}

void DcDccControler::ResetConfig()
{
	for (int i = 0; i < EEPROM_SIZE; i++)
		EEPROMextent.update(i, 0);
}