#include "Looper\\Looper.h"
#include "DateInterface.h"
#include "Date.h"
#include "Utility.h"
#include "SegScreen\\SegScreen.h"
#include "TaskUtility.h"
#include "Input.h"
#define DATEINTERFACE_LOOP_ZERONUM 8
#define DATEINTERFACE_STRING_LENGTH 10
#define DATEINTERFACE_LOOP_INTERVAL 50

#define MODE_RUNNING 0
#define MODE_SETTING 1
#define MODE_EXIT -1

#define SPARKING_DURATION 300
#define SPARKING_STATE_SHOW 0
#define SPARKING_STATE_HIDE 1

#define SETTING_ITEM_YEAR 0
#define SETTING_ITEM_MONTH 1
#define SETTING_ITEM_DAY 2

#define KEYCODE_SETTING INPUT_TOKEYCODE(1,1)
#define KEYCODE_SETTING_SHIFT_ITEM INPUT_TOKEYCODE(1,2)
#define KEYCODE_SETTING_ADD INPUT_TOKEYCODE(1,3)
#define KEYCODE_SETTING_SUB INPUT_TOKEYCODE(2,3)
DATE dateInterface_Date;

#define VIEW_LOOP_INDEX TaskUtility_Public_Var_uChar1
#define VIEW_LOOP_TIMER TaskUtility_Public_Var_uChar2
#define SEG_VIEW_ARRAY TaskUtility_Public_Var_SegViewArr

#define SETTING_ITEM TaskUtility_Public_Var_uChar3
#define SETTING_ITEM_SPARK_TIMER TaskUtility_Public_Var_uChar4
#define SETTING_ITEM_SPARKSTATE TaskUtility_Public_Var_uChar5

unsigned char dateInterface_temp_i;
char dateInterface_Mode = MODE_EXIT;
void DateInterface_Init()
{
	DATE_SET_YEAR(dateInterface_Date, 2019);
	DATE_SET_MONTH(dateInterface_Date, 9);
	DATE_SET_DAY(dateInterface_Date, 12);
}
void DateInterface_LooperUpdate(LOOPER *looper)
{
	char loopIndex = 0;
	if(dateInterface_Mode == MODE_EXIT)
	{
		return;
	}
	
	for(dateInterface_temp_i = 0; dateInterface_temp_i < 8; dateInterface_temp_i++)
	{
		loopIndex = (dateInterface_temp_i + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM);
		if((dateInterface_temp_i + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM) >= DATEINTERFACE_STRING_LENGTH)
		{
			SEG_VIEW_ARRAY[dateInterface_temp_i] = ' ';
		}
		else
		{
			if(loopIndex == 0)
			{
				Int_To_Chars(DATE_GET_YEAR(dateInterface_Date), 4, &(SEG_VIEW_ARRAY[dateInterface_temp_i]));
			}
			else if(loopIndex == 4)
			{
				SEG_VIEW_ARRAY[dateInterface_temp_i] = '-';
			}
			else if(loopIndex == 5)
			{
				Int_To_Chars(DATE_GET_MONTH(dateInterface_Date), 2, &(SEG_VIEW_ARRAY[dateInterface_temp_i]));
			}
			else if(loopIndex == 7)
			{
				SEG_VIEW_ARRAY[dateInterface_temp_i] = '-';
			}
			else if(loopIndex == 8)
			{
				Int_To_Chars(DATE_GET_DAY(dateInterface_Date), 2, &(SEG_VIEW_ARRAY[dateInterface_temp_i]));
			}
		}
	}
	SEG_VIEW_ARRAY[8] = '\0';
	
	if(dateInterface_Mode == MODE_RUNNING)
	{
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			dateInterface_Mode = MODE_SETTING;
			SETTING_ITEM = SETTING_ITEM_YEAR;
			VIEW_LOOP_INDEX = 0;
		}
	}
	else if(dateInterface_Mode == MODE_SETTING)
	{
		
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			dateInterface_Mode = MODE_RUNNING;
			VIEW_LOOP_INDEX = 0;
			SETTING_ITEM = SETTING_ITEM_YEAR;
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
		}
		else if(Input_GetKeyDown() == KEYCODE_SETTING_SHIFT_ITEM)
		{
			SETTING_ITEM = (SETTING_ITEM + 1) % 3;
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
		}
		
		SETTING_ITEM_SPARK_TIMER += looper->deltaTime;
		if(SETTING_ITEM_SPARK_TIMER >= SPARKING_DURATION/2)
		{
			SETTING_ITEM_SPARKSTATE = (SETTING_ITEM_SPARKSTATE == SPARKING_STATE_SHOW) ? SPARKING_STATE_HIDE : SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
		}
		
		if(Input_GetKeyDown() == KEYCODE_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SUB)
		{
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
		}
		
		if(SETTING_ITEM == SETTING_ITEM_YEAR)
		{
			VIEW_LOOP_INDEX = 0;
			if(Input_GetKeyDown() == KEYCODE_SETTING_ADD)
			{
				if(DATE_GET_YEAR(dateInterface_Date)+1 >= 2100)
				{
					DATE_SET_YEAR(dateInterface_Date, 2000);
				}
				else
				{
					DATE_SET_YEAR(dateInterface_Date, DATE_GET_YEAR(dateInterface_Date)+1);
				}
			}
			else if(Input_GetKeyDown() == KEYCODE_SETTING_SUB)
			{
				if(DATE_GET_YEAR(dateInterface_Date)-1 <= 1999)
				{
					DATE_SET_YEAR(dateInterface_Date, 2099);
				}
				else
				{
					DATE_SET_YEAR(dateInterface_Date, DATE_GET_YEAR(dateInterface_Date)-1);
				}
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[0] = ' ';
				SEG_VIEW_ARRAY[1] = ' ';
				SEG_VIEW_ARRAY[2] = ' ';
				SEG_VIEW_ARRAY[3] = ' ';
			}
		}
		else if(SETTING_ITEM == SETTING_ITEM_MONTH)
		{
			VIEW_LOOP_INDEX = 0;
			if(Input_GetKeyDown() == KEYCODE_SETTING_ADD)
			{
				if(DATE_GET_MONTH(dateInterface_Date)+1 >= 13)
				{
					DATE_SET_MONTH(dateInterface_Date, 1);
				}
				else
				{
					DATE_SET_MONTH(dateInterface_Date, DATE_GET_MONTH(dateInterface_Date)+1);
				}
			}
			else if(Input_GetKeyDown() == KEYCODE_SETTING_SUB)
			{
				if(DATE_GET_MONTH(dateInterface_Date)-1 <= 0)
				{
					DATE_SET_MONTH(dateInterface_Date, 12);
				}
				else
				{
					DATE_SET_MONTH(dateInterface_Date, DATE_GET_MONTH(dateInterface_Date)-1);
				}
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[5] = ' ';
				SEG_VIEW_ARRAY[6] = ' ';
			}
		}
		else if(SETTING_ITEM == SETTING_ITEM_DAY)
		{
			VIEW_LOOP_INDEX = 2;
			if(Input_GetKeyDown() == KEYCODE_SETTING_ADD)
			{
				if(DATE_GET_DAY(dateInterface_Date)+1 >= 32)
				{
					DATE_SET_DAY(dateInterface_Date, 1);
				}
				else
				{
					DATE_SET_DAY(dateInterface_Date, DATE_GET_DAY(dateInterface_Date)+1);
				}
			}
			else if(Input_GetKeyDown() == KEYCODE_SETTING_SUB)
			{
				if(DATE_GET_DAY(dateInterface_Date)-1 <= 0)
				{
					DATE_SET_DAY(dateInterface_Date, 31);
				}
				else
				{
					DATE_SET_DAY(dateInterface_Date, DATE_GET_DAY(dateInterface_Date)-1);
				}
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[6] = ' ';
				SEG_VIEW_ARRAY[7] = ' ';
			}
		}
	}
	
	SegScreen_Print_String(SEG_VIEW_ARRAY);
	
	if(dateInterface_Mode == MODE_RUNNING)
	{
		if(VIEW_LOOP_TIMER >= DATEINTERFACE_LOOP_INTERVAL)
		{
			VIEW_LOOP_TIMER = 0;
			VIEW_LOOP_INDEX = (1 + VIEW_LOOP_INDEX) % (DATEINTERFACE_STRING_LENGTH + DATEINTERFACE_LOOP_ZERONUM);
		}
		VIEW_LOOP_TIMER += looper->deltaTime;
	}
	
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
