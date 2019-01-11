#include "..\\Looper\\Looper.h"
#include <REG51.H>
#include <absacc.h>
#include "SegScreen.h"

//From Right To Left--0 to 7
int segScreen_CurrShiftMask = 0x01;
int segScreen_shiftTimer = 0;
unsigned char code segScreen_led_table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x79	};
unsigned char segScreen_led[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
char SegScreen_ShiftMaskToIndex(char segMask);
int segScreen_temp_i;
void SegScreen_LooperUpdate(LOOPER *looper)
{
	segScreen_shiftTimer += looper->deltaTime;
	XBYTE[0x9000]= segScreen_led[SegScreen_ShiftMaskToIndex(segScreen_CurrShiftMask)];
	XBYTE[0x8000]= segScreen_CurrShiftMask;
	if(segScreen_shiftTimer >= SEGSCREEN_SHIFT_INTERVAL_MS)
	{
		segScreen_shiftTimer = 0;
		segScreen_CurrShiftMask = 0xFF & (segScreen_CurrShiftMask << 1);
		if(segScreen_CurrShiftMask == 0)
		{
			segScreen_CurrShiftMask = 0x01;
		}
	}
}
/*
void SegScreen_SetLed(int index, char ledMask)
{
	segScreen_led[index] = ledMask;
}
*/
/*
void SegScreen_Print_Int(int value)
{
	int temp;
	for(segScreen_temp_i = 0; segScreen_temp_i < 8; segScreen_temp_i++)
	{
		segScreen_led[segScreen_temp_i] = 0x00;
	}
	temp = value < 0 ? -value : value;
	segScreen_temp_i = 0;
	if(value == 0)
	{
		segScreen_led[0] = segScreen_led_table[0];
	}
	else
	{
		while(temp != 0 && segScreen_temp_i < 8)
		{
			segScreen_led[segScreen_temp_i] = segScreen_led_table[temp % 10];
			temp = temp / 10;
			segScreen_temp_i++;
		}
	}
	
	if(segScreen_temp_i < 8 && value < 0)
	{
		segScreen_led[segScreen_temp_i] = 0x40;
	}
}
void SegScreen_Print_Bin(int value)
{
	for(segScreen_temp_i == 0; segScreen_temp_i < 8; segScreen_temp_i++)
	{
		segScreen_led[segScreen_temp_i] = segScreen_led_table[(value & 1<<segScreen_temp_i) == 0 ? 0: 1];
	}
}
*/
//Can only show dot(.) and number(0..9)
void SegScreen_Print_String(char *str)
{
	int segIndex = 7;
	for(segScreen_temp_i = 0; segScreen_temp_i < 8; segScreen_temp_i++)
	{
		segScreen_led[segScreen_temp_i] = 0x00;
	}
	for(segScreen_temp_i = 0; str[segScreen_temp_i] != '\0' && segIndex >= 0; segScreen_temp_i++)
	{
		if(str[segScreen_temp_i] == '.')
		{
			segScreen_led[segIndex+1] = segScreen_led[segIndex+1] | 0x80;
		}
		else
		{
			if(str[segScreen_temp_i] == ' ')
			{
				segScreen_led[segIndex] = 0;
			}
			else if(str[segScreen_temp_i] == '-')
			{
				segScreen_led[segIndex] = 0x40;
			}
			else if(str[segScreen_temp_i] == '_')
			{
				segScreen_led[segIndex] = 0x08;
			}
			else
			{
				segScreen_led[segIndex] = segScreen_led_table[str[segScreen_temp_i] - '0'];
			}
			
			segIndex--;
		}
	}
}
char SegScreen_ShiftMaskToIndex(char segMask){
	for(segScreen_temp_i = 0; segScreen_temp_i < 8; segScreen_temp_i++)
	{
		if((segMask & (1 << segScreen_temp_i)) != 0)
		{
			return segScreen_temp_i;
		}
	}
	return -1;
}