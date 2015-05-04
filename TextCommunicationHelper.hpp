//-------------------------------------------------------------------
#ifndef __textCommunicationHelper_H__
#define __textCommunicationHelper_H__
//-------------------------------------------------------------------

// Helper functions to encode/decode string communication.
// A string to communicate have the form 'AAAA_D_E' 
// where AAAA is a Decoder Dcc ID on four digits with leading zeros,
// D is the accessory number from 0 to 7,
// E is 0 or 1 to activate/unactivate the accessory.

class TextCommunicationHelper
{
	private:
		static char buffer[9];

	public:
		static char *Encode(int inDccId, byte inDccIdAccessory, bool inEnable);
		static int DecodeID(char *inpBuffer);
		static byte DecodeIDAccessory(char *inpBuffer);
		static bool DecodeEnable(char *inpBuffer);

#ifdef DEBUG_MODE
		static void CheckSyntax(char *inpBuffer);
#endif
};

//-------------------------------------------------------------------
#endif
//-------------------------------------------------------------------
