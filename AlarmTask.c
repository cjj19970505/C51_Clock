#include "Looper\\Looper.h"
#include "AlarmTask.h"
#include "Time.h"
#include "Input.h"
#include "SegScreen\\SegScreen.h"
#include "Date.h"
#include "TaskUtility.h"
#include "TaskSelector.h"
#include "ClockInterface.h"
#include "RingtoneManager.h"
#define IN_FOREGROUND (TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_ALARM)
#define KEYCODE_SETTING INPUT_TOKEYCODE(1,1)
#define KEYCODE_SETTING_SHIFT_ITEM INPUT_TOKEYCODE(1,2)
#define KEYCODE_SETTING_SETTING_ADD INPUT_TOKEYCODE(1,3)
#define KEYCODE_SETTING_SETTING_SUB INPUT_TOKEYCODE(2,3)
#define MODE_SETTING 1
#define MODE_RUNNING 0
#define SPARKING_DURATION 300
#define SPARKING_STATE_SHOW 0
#define SPARKING_STATE_HIDE 1
#define SETTING_ITEM_HOUR 0
#define SETTING_ITEM_MINUTE 1
TIME alarmTask_Time;
char alarmTask_PreClockHour;
char alarmTask_PreClockMinute;
char alarmTask_Mode = MODE_RUNNING;	//0:Run Setting
#define SETTING_ITEM TaskUtility_Public_Var_uChar1
#define SETTING_ITEM_SPARK_TIMER TaskUtility_Public_Var_uChar2
#define SETTING_ITEM_SPARKSTATE TaskUtility_Public_Var_uChar3
#define SEG_VIEW_ARRAY TaskUtility_Public_Var_SegViewArr
//char alarmTask_SettingItem = 0; //0:Hour 1:Minute 2:Second
//int SETTING_ITEM_SPARK_TIMER = 0;
//char SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
void AlarmTask_Init()
{
	alarmTask_Mode = MODE_RUNNING;
	TIME_SET_HOUR(alarmTask_Time, 1);
	TIME_SET_MINUTE(alarmTask_Time, 0);
	TIME_SET_SECOND(alarmTask_Time, 0);
}
void AlarmTask_LooperUpdate(LOOPER *looper)
{
	if(IN_FOREGROUND)
	{
		Time_ToString(&alarmTask_Time, '-', 0, SEG_VIEW_ARRAY);
		SEG_VIEW_ARRAY[5] = ' ';
		SEG_VIEW_ARRAY[6] = ' ';
		SEG_VIEW_ARRAY[7] = ' ';
	}
	if(alarmTask_Mode == MODE_RUNNING)
	{
		if(Input_GetKeyDown() == KEYCODE_SETTING && IN_FOREGROUND)
		{
			alarmTask_Mode = MODE_SETTING;
		}
		if(ClockInterface_InSettingMode())
		{
			
		}
		else if(TIME_GET_HOUR(alarmTask_Time) == TIME_GET_HOUR(clockInterface_Time) && TIME_GET_MINUTE(alarmTask_Time) == TIME_GET_MINUTE(clockInterface_Time) && alarmTask_PreClockMinute != TIME_GET_MINUTE(alarmTask_Time))
		{
			RingtoneManager_StartRing();
		}
	}
	else if(alarmTask_Mode == MODE_SETTING && IN_FOREGROUND)
	{
		SETTING_ITEM_SPARK_TIMER += looper->deltaTime;
		if(SETTING_ITEM_SPARK_TIMER >= SPARKING_DURATION/2)
		{
			SETTING_ITEM_SPARKSTATE = (SETTING_ITEM_SPARKSTATE == SPARKING_STATE_SHOW) ? SPARKING_STATE_HIDE : SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
			
		}
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			alarmTask_Mode = MODE_RUNNING;
			SETTING_ITEM = 0;
			SETTING_ITEM_SPARK_TIMER = 0;
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
		}
		else if(Input_GetKeyDown() == KEYCODE_SETTING_SHIFT_ITEM)
		{
			SETTING_ITEM++;
			SETTING_ITEM %= 2;
		}
		if(SETTING_ITEM == SETTING_ITEM_HOUR)
		{
			if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD || Input_GetKeyDown() == KEYCODE_SETTING_SETTING_SUB)
			{
				if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD)
				{
					TIME_SET_HOUR(alarmTask_Time, (TIME_GET_HOUR(alarmTask_Time) + 1)%24);
				}
				else
				{
					TIME_SET_HOUR(alarmTask_Time, TIME_GET_HOUR(alarmTask_Time) == 0 ? 23 : (TIME_GET_HOUR(alarmTask_Time) - 1));
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
					TIME_SET_MINUTE(alarmTask_Time, (TIME_GET_MINUTE(alarmTask_Time) + 1)%60);
				}
				else
				{
					TIME_SET_MINUTE(alarmTask_Time, TIME_GET_MINUTE(alarmTask_Time) == 0 ? 59 : TIME_GET_MINUTE(alarmTask_Time) - 1);
				}
				
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[3] = ' ';
				SEG_VIEW_ARRAY[4] = ' ';
			}
		}
	}
	if(IN_FOREGROUND)
	{
		SegScreen_Print_String(SEG_VIEW_ARRAY);
	}
	
	alarmTask_PreClockHour = TIME_GET_HOUR(clockInterface_Time);
	alarmTask_PreClockMinute = TIME_GET_MINUTE(clockInterface_Time);
	
	
}
void AlarmTask_EnterTask()
{
	alarmTask_Mode = MODE_RUNNING;
}
void AlarmTask_ExitTask()
{
	alarmTask_Mode = MODE_RUNNING;
	SETTING_ITEM = 0;
	SETTING_ITEM_SPARK_TIMER = 0;
	SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
}


