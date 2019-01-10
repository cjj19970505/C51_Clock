void Int_To_Chars(int value, int decDigitCount, char *chars)
{
	char i = decDigitCount - 1;
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