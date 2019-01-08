#include "Time.h"
int time_temp_i;
void Int_To_Chars(int value, int decDigitCount, char *chars);
void Time_ToString(TIME *time, char *str)
{
	Int_To_Chars(TIME_GET_HOUR(*time), 2, str);
	str[2] = '.';
	Int_To_Chars(TIME_GET_MINUTE(*time), 2, &(str[3]));
	str[5] = '.';
	Int_To_Chars(TIME_GET_SECOND(*time), 2, &(str[6]));
	str[8] = '\0';
}

void Int_To_Chars(int value, int decDigitCount, char *chars)
{
	int i = decDigitCount - 1;
	int left = value;
	int curr;
	while(i >= 0)
	{
		curr = left % 10;
		left = left / 10;
		chars[i] = curr + '0';
		i--;
	}
}