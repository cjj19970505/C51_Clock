#include "Looper\\Looper.h"
#include "ClockInterface.h"
#include "Time.h"
#include "Input.h"
#include "SegScreen\\SegScreen.h"
#include "Date.h"
#define KEYCODE_SETTING INPUT_TOKEYCODE(1,1)
#define KEYCODE_SETTING_SHIFT_ITEM INPUT_TOKEYCODE(1,2)
#define KEYCODE_SETTING_SETTING_ADD INPUT_TOKEYCODE(1,3)
#define KEYCODE_SETTING_SETTING_SUB INPUT_TOKEYCODE(2,1)
#define MODE_SETTING 1
#define MODE_RUNNING 0
#define SPARKING_DURATION 300
#define SPARKING_STATE_SHOW 0
#define SPARKING_STATE_HIDE 1
#define SETTING_ITEM_HOUR 0
#define SETTING_ITEM_MINUTE 1
#define SETTING_ITEM_SECOND 2
TIME clockInterface_Time;
char clockInterface_View[16];
char clockInterface_Mode = 0;	//0:Run Setting
char clockInterface_SettingItem = 0; //0:Hour 1:Minute 2:Second
int clockInterface_SettingItem_Spark_Timer = 0;
char clockInterface_SettingItem_SparkState = SPARKING_STATE_SHOW;
void ClockInterface_Init()
{
	TIME_SET_HOUR(clockInterface_Time, 0);
	TIME_SET_MINUTE(clockInterface_Time, 0);
	TIME_SET_SECOND(clockInterface_Time, 0);
}
void ClockInterface_LooperUpdate(LOOPER *looper)
{
	//clockInterface_Timer += looper->deltaTime;
	Time_ToString(&clockInterface_Time, clockInterface_View);
	if(clockInterface_Mode == MODE_RUNNING)
	{
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			clockInterface_Mode = MODE_SETTING;
		}
	}
	else if(clockInterface_Mode == MODE_SETTING)
	{
		clockInterface_SettingItem_Spark_Timer += looper->deltaTime;
		if(clockInterface_SettingItem_Spark_Timer >= SPARKING_DURATION/2)
		{
			clockInterface_SettingItem_SparkState = (clockInterface_SettingItem_SparkState == SPARKING_STATE_SHOW) ? SPARKING_STATE_HIDE : SPARKING_STATE_SHOW;
			clockInterface_SettingItem_Spark_Timer = 0;
			
		}
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			clockInterface_Mode = MODE_RUNNING;
			clockInterface_SettingItem = 0;
			clockInterface_SettingItem_Spark_Timer = 0;
			clockInterface_SettingItem_SparkState = SPARKING_STATE_SHOW;
		}
		else if(Input_GetKeyDown() == KEYCODE_SETTING_SHIFT_ITEM)
		{
			clockInterface_SettingItem++;
			clockInterface_SettingItem %= 3;
		}
		if(clockInterface_SettingItem == SETTING_ITEM_HOUR)
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
				clockInterface_SettingItem_SparkState = SPARKING_STATE_SHOW;
				clockInterface_SettingItem_Spark_Timer = 0;
				
			}
			if(clockInterface_SettingItem_SparkState == SPARKING_STATE_HIDE)
			{
				clockInterface_View[0] = ' ';
				clockInterface_View[1] = ' ';
			}
		}
		else if(clockInterface_SettingItem == SETTING_ITEM_MINUTE)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				clockInterface_SettingItem_SparkState = SPARKING_STATE_SHOW;
				clockInterface_SettingItem_Spark_Timer = 0;
				if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD)
				{
					TIME_SET_MINUTE(clockInterface_Time, (TIME_GET_MINUTE(clockInterface_Time) + 1)%60);
				}
				else
				{
					TIME_SET_MINUTE(clockInterface_Time, TIME_GET_MINUTE(clockInterface_Time) == 0 ? 59 : TIME_GET_MINUTE(clockInterface_Time) - 1);
				}
				
				
			}
			if(clockInterface_SettingItem_SparkState == SPARKING_STATE_HIDE)
			{
				clockInterface_View[3] = ' ';
				clockInterface_View[4] = ' ';
			}
		}
		else if(clockInterface_SettingItem == SETTING_ITEM_SECOND)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				clockInterface_SettingItem_SparkState = SPARKING_STATE_SHOW;
				clockInterface_SettingItem_Spark_Timer = 0;
				TIME_SET_SECOND(clockInterface_Time, 0);
			}
			if(clockInterface_SettingItem_SparkState == SPARKING_STATE_HIDE)
			{
				clockInterface_View[6] = ' ';
				clockInterface_View[7] = ' ';
			}
		}
		
	}
	SegScreen_Print_String(clockInterface_View);
}

void ClockInterface_OnDeciClockTrigger()
{
	Time_Add_1_Decisecond(&clockInterface_Time);
}


