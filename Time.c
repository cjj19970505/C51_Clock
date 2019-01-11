#include "Time.h"
#include "BinHelper.h"
#include "Utility.h"

int time_temp_i;
void Int_To_Chars(int value, int decDigitCount, char *chars);

void Time_ToString(TIME *time, char hyphen, char hasDeciSecond, char *str)
{
	Int_To_Chars(TIME_GET_HOUR(*time), 2, str);
	str[2] = hyphen;
	Int_To_Chars(TIME_GET_MINUTE(*time), 2, &(str[3]));
	str[5] = hyphen;
	Int_To_Chars(TIME_GET_SECOND(*time), 2, &(str[6]));
	if(hasDeciSecond)
	{
		str[8] = hyphen;
		Int_To_Chars(TIME_GET_DECISECOND(*time), 2, &(str[9]));
		str[11] = '\0';
	}
	else
	{
		str[8] = '\0';
	}
}

void Time_Add_1_Decisecond(TIME *time)
{
	TIME_SET_DECISECOND(*time ,TIME_GET_DECISECOND(*time)+1);
	if(TIME_GET_DECISECOND(*time) >= 10)
	{
		TIME_SET_DECISECOND(*time, 0);
		TIME_SET_SECOND(*time, TIME_GET_SECOND(*time)+1);
	}
	if(TIME_GET_SECOND(*time) >= 60)
	{
		TIME_SET_SECOND(*time, 0);
		TIME_SET_MINUTE(*time, TIME_GET_MINUTE(*time)+1);
	}
	if(TIME_GET_MINUTE(*time) >= 60)
	{
		TIME_SET_MINUTE(*time, 0);
		TIME_SET_HOUR(*time, TIME_GET_HOUR(*time)+1);
	}
}

void Time_Sub_1_Decisecond(TIME *time)
{
	TIME_SET_DECISECOND(*time ,TIME_GET_DECISECOND(*time)-1);
	if(TIME_GET_DECISECOND(*time) < 0)
	{
		TIME_SET_DECISECOND(*time, 9);
		TIME_SET_SECOND(*time, TIME_GET_SECOND(*time)-1);
	}
	if(TIME_GET_SECOND(*time) < 0)
	{
		TIME_SET_SECOND(*time, 59);
		TIME_SET_MINUTE(*time, TIME_GET_MINUTE(*time)-1);
	}
	if(TIME_GET_MINUTE(*time) < 0)
	{
		TIME_SET_MINUTE(*time, 59);
		TIME_SET_HOUR(*time, TIME_GET_HOUR(*time)-1);
	}
	if(TIME_GET_HOUR(*time) < 0 )
	{
		TIME_SET_HOUR(*time, 99);
	}
}

