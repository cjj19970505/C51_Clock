#include <REG51.H>
#include <absacc.h>
#include "Looper\\Looper.h"
#include "Input.h"
char input_PreviousKeycode = 0;
char input_LatestKeycode = 0;
char input_HasKeyDown = 0;
unsigned char Input_Getkeycode(void)
{
	unsigned char line=0x00;
	unsigned char col=0x00;
	unsigned char scancode=0x01;
	unsigned char keycode;

	XBYTE[0x8000]=0xff;
	col=XBYTE[0x8000]&0x0f;
	if (col==0x00)
		keycode=0x00;
	else
	{
		while((scancode&0x0f)!=0)
		{
			line=scancode;
			XBYTE[0x8000]=scancode;
			if((XBYTE[0x8000]&0x0f)==col)
			break; 
			scancode=scancode<<1;
		}
		col=col<<4;
		keycode=col|line;
	}
	return keycode;
}

void Input_LooperUpdate(LOOPER *looper)
{
	static int checkInputTimer = 0;
	input_HasKeyDown = 0;
	if(checkInputTimer >= INPUT_CHECKINPUT_INTERVAL_MS)
	{
		checkInputTimer = 0;
		XBYTE[0x9000] = 0x00;
		input_PreviousKeycode = input_LatestKeycode;
		input_LatestKeycode = Input_Getkeycode();
		if(input_LatestKeycode != input_PreviousKeycode)
		{
			input_HasKeyDown = 1;
		}
	}
	checkInputTimer += looper->deltaTime;
	
}
char Input_GetLatestKeyCode()
{
	return input_LatestKeycode;
}
char Input_GetKeyDown()
{
	if(input_HasKeyDown)
	{
		return input_LatestKeycode;
	}
	return 0;
}
