#include "Looper\\Looper.h"
#include "DateInterface.h"
#include "Date.h"
#include "Utility.h"
#include "SegScreen\\SegScreen.h"
#include "TaskUtility.h"
#define DATEINTERFACE_LOOP_ZERONUM 8
#define DATEINTERFACE_STRING_LENGTH 10
#define DATEINTERFACE_LOOP_INTERVAL 50
#define MODE_RUNNING 0
#define MODE_EXIT -1

char dateInterface_View[8+5];
DATE dateInterface_Date;

#define VIEW_LOOP_INDEX TaskUtility_Public_Var_uChar1
#define VIEW_LOOP_TIMER TaskUtility_Public_Var_uChar2

unsigned char dateInterface_temp_i;
char dateInterface_Mode = MODE_EXIT;
void DateInterface_LooperUpdate(LOOPER *looper)
{
	char loopIndex = 0;
	if(dateInterface_Mode == MODE_EXIT)
	{
		return;
	}
	DATE_SET_YEAR(dateInterface_Date, 2019);
	DATE_SET_MONTH(dateInterface_Date, 9);
	DATE_SET_DAY(dateInterface_Date, 12);
	
	for(dateInterface_temp_i = 0; dateInterface_temp_i < 8; dateInterface_temp_i++)
	{
		loopIndex = (dateInterface_temp_i + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM);
		if((dateInterface_temp_i + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM) >= DATEINTERFACE_STRING_LENGTH)
		{
			dateInterface_View[dateInterface_temp_i] = ' ';
		}
		else
		{
			if(loopIndex == 0)
			{
				Int_To_Chars(DATE_GET_YEAR(dateInterface_Date), 4, &(dateInterface_View[dateInterface_temp_i]));
			}
			else if(loopIndex == 4)
			{
				dateInterface_View[dateInterface_temp_i] = '-';
			}
			else if(loopIndex == 5)
			{
				Int_To_Chars(DATE_GET_MONTH(dateInterface_Date), 2, &(dateInterface_View[dateInterface_temp_i]));
			}
			else if(loopIndex == 7)
			{
				dateInterface_View[dateInterface_temp_i] = '-';
			}
			else if(loopIndex == 8)
			{
				Int_To_Chars(DATE_GET_DAY(dateInterface_Date), 2, &(dateInterface_View[dateInterface_temp_i]));
			}
		}
	}
	
	dateInterface_View[8] = '\0';
	//dateInterface_View[0] = loopIndex + '0';
	SegScreen_Print_String(dateInterface_View);
	//for(dateInterface_temp_i = 0)
	/*
	Int_To_Chars(DATE_GET_YEAR(*date), 4, str);
	str[4] = '.';
	Int_To_Chars(DATE_GET_MONTH(*date), 2, &(str[5]));
	str[7] = '.';
	Int_To_Chars(DATE_GET_DAY(*date), 2, &(str[8]));
	str[10] = '\0';
	*/
	if(VIEW_LOOP_TIMER >= DATEINTERFACE_LOOP_INTERVAL)
	{
		VIEW_LOOP_TIMER = 0;
		VIEW_LOOP_INDEX = (1 + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM);
	}
	VIEW_LOOP_TIMER += looper->deltaTime;
	
	
}

void DateInterface_EnterTask()
{
	VIEW_LOOP_INDEX = 0;
	VIEW_LOOP_TIMER = 0;
	dateInterface_Mode = MODE_RUNNING;
}

void DateInterface_ExitTask()
{
	dateInterface_Mode = MODE_EXIT;
}
