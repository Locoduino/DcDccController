/*************************************************************
project: <Dc/Dcc Controler>
author: <Thierry PARIS>
description: <Text communication helper.>
*************************************************************/

#include "DcDccControler.h"
#include "TextCommunicationHelper.hpp"

#ifdef DEBUG_MODE
#define CHECK(text)	CheckSyntax(text)
#else
#define CHECK(text)
#endif

#ifdef DEBUG_MODE
//       01234567
// Check 0000_0_0 syntax
void TextCommunicationHelper::CheckSyntax(char *inpBuffer)
{
	int len = 0;
	for (int i = 0; i < 10; i++)
		if (inpBuffer[i] != 0)
			len++;

	bool good = true;

	if (len != 8)
		good = false;
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (i == 4 || i == 6)
			{
				if (inpBuffer[i] != ' ' && inpBuffer[i] != '_')
					good = false;
				continue;
			}

			if (inpBuffer[i] < '0' || inpBuffer[i] > '9')
				good = false;
		}
	}

	Serial.print(F("String "));
	Serial.print(inpBuffer);
	Serial.println(F(" has the wrong format and should be AAAA_D_E."));
}
#endif

char TextCommunicationHelper::buffer[9];

char *TextCommunicationHelper::Encode(int inDccId, byte inDccIdAccessory, bool inEnable)
{
	buffer[0] = inDccId / 1000 + '0';
	buffer[1] = (inDccId % 1000) / 100 + '0';
	buffer[2] = (inDccId % 100) / 10 + '0';
	buffer[3] = (inDccId % 10) + '0';
	buffer[4] = ' ';
	buffer[5] = inDccIdAccessory + '0';
	buffer[6] = ' ';
	buffer[7] = inEnable ? '1' : '0';

	return buffer;
}

int TextCommunicationHelper::DecodeID(char *inpBuffer)
{
	CHECK(inpBuffer);

	return ((inpBuffer[0] - '0') * 1000) + ((inpBuffer[1] - '0') * 100) + ((inpBuffer[2] - '0') * 10) + ((inpBuffer[3] - '0'));
}

byte TextCommunicationHelper::DecodeIDAccessory(char *inpBuffer)
{
	CHECK(inpBuffer);

	return ((inpBuffer[5] - '0'));
}

bool TextCommunicationHelper::DecodeEnable(char *inpBuffer)
{
	CHECK(inpBuffer);

	return inpBuffer[7] == '1';
}
