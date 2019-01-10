#include "Looper\\Looper.h"
#include "ClockInterface.h"
#include "Time.h"
#include "Input.h"
#include "SegScreen\\SegScreen.h"
#include "Date.h"
#include "TaskUtility.h"
#include "TaskSelector.h"
#define KEYCODE_SETTING INPUT_TOKEYCODE(1,1)
#define KEYCODE_SETTING_SHIFT_ITEM INPUT_TOKEYCODE(1,2)
#define KEYCODE_SETTING_SETTING_ADD INPUT_TOKEYCODE(1,3)
#define KEYCODE_SETTING_SETTING_SUB INPUT_TOKEYCODE(2,3)
#define MODE_SETTING 1
#define MODE_RUNNING 0
#define MODE_EXIT 2
#define SPARKING_DURATION 300
#define SPARKING_STATE_SHOW 0
#define SPARKING_STATE_HIDE 1
#define SETTING_ITEM_HOUR 0
#define SETTING_ITEM_MINUTE 1
#define SETTING_ITEM_SECOND 2
TIME clockInterface_Time;
char clockInterface_Mode = 0;	//0:Run Setting
#define SETTING_ITEM TaskUtility_Public_Var_uChar1
#define SETTING_ITEM_SPARK_TIMER TaskUtility_Public_Var_uChar2
#define SETTING_ITEM_SPARKSTATE TaskUtility_Public_Var_uChar3
#define SEG_VIEW_ARRAY TaskUtility_Public_Var_SegViewArr
//char clockInterface_SettingItem = 0; //0:Hour 1:Minute 2:Second
//int SETTING_ITEM_SPARK_TIMER = 0;
//char SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
void ClockInterface_Init()
{
	TIME_SET_HOUR(clockInterface_Time, 0);
	TIME_SET_MINUTE(clockInterface_Time, 0);
	TIME_SET_SECOND(clockInterface_Time, 0);
}
void ClockInterface_LooperUpdate(LOOPER *looper)
{
	
	if(TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_CLOCK)
	{
		Time_ToString(&clockInterface_Time, '.', 0, SEG_VIEW_ARRAY);
	}
	if(clockInterface_Mode == MODE_RUNNING)
	{
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			clockInterface_Mode = MODE_SETTING;
		}
	}
	else if(clockInterface_Mode == MODE_SETTING)
	{
		SETTING_ITEM_SPARK_TIMER += looper->deltaTime;
		if(SETTING_ITEM_SPARK_TIMER >= SPARKING_DURATION/2)
		{
			SETTING_ITEM_SPARKSTATE = (SETTING_ITEM_SPARKSTATE == SPARKING_STATE_SHOW) ? SPARKING_STATE_HIDE : SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
			
		}
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			clockInterface_Mode = MODE_RUNNING;
			SETTING_ITEM = 0;
			SETTING_ITEM_SPARK_TIMER = 0;
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
		}
		else if(Input_GetKeyDown() == KEYCODE_SETTING_SHIFT_ITEM)
		{
			SETTING_ITEM++;
			SETTING_ITEM %= 3;
		}
		if(SETTING_ITEM == SETTING_ITEM_HOUR)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD)
				{
					TIME_SET_HOUR(clockInterface_Time, (TIME_GET_HOUR(clockInterface_Time) + 1)%24);
				}
				else
				{
					TIME_SET_HOUR(clockInterface_Time, TIME_GET_HOUR(clockInterface_Time) == 0 ? 23 : (TIME_GET_HOUR(clockInterface_Time) - 1));
				}
				SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
				SETTING_ITEM_SPARK_TIMER = 0;
				
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[0] = ' ';
				SEG_VIEW_ARRAY[1] = ' ';
			}
		}
		else if(SETTING_ITEM == SETTING_ITEM_MINUTE)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
				SETTING_ITEM_SPARK_TIMER = 0;
				if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD)
				{
					TIME_SET_MINUTE(clockInterface_Time, (TIME_GET_MINUTE(clockInterface_Time) + 1)%60);
				}
				else
				{
					TIME_SET_MINUTE(clockInterface_Time, TIME_GET_MINUTE(clockInterface_Time) == 0 ? 59 : TIME_GET_MINUTE(clockInterface_Time) - 1);
				}
				
				
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[3] = ' ';
				SEG_VIEW_ARRAY[4] = ' ';
			}
		}
		else if(SETTING_ITEM == SETTING_ITEM_SECOND)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
				SETTING_ITEM_SPARK_TIMER = 0;
				TIME_SET_SECOND(clockInterface_Time, 0);
				TIME_SET_DECISECOND(clockInterface_Time, 0);
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[6] = ' ';
				SEG_VIEW_ARRAY[7] = ' ';
			}
		}
		
	}
	if(TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_CLOCK)
	{
		SegScreen_Print_String(SEG_VIEW_ARRAY);
	}
	
}

void ClockInterface_OnDeciClockTrigger()
{
	Time_Add_1_Decisecond(&clockInterface_Time);
}
void ClockInterface_EnterTask()
{
	clockInterface_Mode = MODE_RUNNING;
}
void ClockInterface_ExitTask()
{
	clockInterface_Mode = MODE_EXIT;
	SETTING_ITEM = 0;
	SETTING_ITEM_SPARK_TIMER = 0;
	SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
}


