#include "Looper\\Looper.h"
#include "Time.h"
#include "CountdownTask.h"
#include "Input.h"
#include "SegScreen\\SegScreen.h"
#include "Date.h"
#include "TaskUtility.h"
#include "TaskSelector.h"
#include "RingtoneManager.h"
#define IN_FOREGROUND (TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_COUNTDOWN)

#define KEYCODE_SETTING INPUT_TOKEYCODE(1,1)
#define KEYCODE_SETTING_SHIFT_ITEM INPUT_TOKEYCODE(1,2)
#define KEYCODE_SETTING_SETTING_ADD INPUT_TOKEYCODE(1,3)
#define KEYCODE_SETTING_SETTING_SUB INPUT_TOKEYCODE(2,3)
#define KEYCODE_START_OR_STOP INPUT_TOKEYCODE(1,2)
#define MODE_SETTING 1
#define MODE_COUNTING 0
#define MODE_STOP 2
#define MODE_COMPLETE 3
#define SPARKING_DURATION 300
#define SPARKING_STATE_SHOW 0
#define SPARKING_STATE_HIDE 1
#define SETTING_ITEM_HOUR 0
#define SETTING_ITEM_MINUTE 1
#define SETTING_ITEM_SECOND 2
TIME countdownTask_Time;
char countdownTask_Mode = 0;	//0:Run Setting
#define SETTING_ITEM TaskUtility_Public_Var_uChar1
#define SETTING_ITEM_SPARK_TIMER TaskUtility_Public_Var_uChar2
#define SETTING_ITEM_SPARKSTATE TaskUtility_Public_Var_uChar3
#define SEG_VIEW_ARRAY TaskUtility_Public_Var_SegViewArr
//char countdownTask_SettingItem = 0; //0:Hour 1:Minute 2:Second
//int SETTING_ITEM_SPARK_TIMER = 0;
//char SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
void CountdownTask_Init()
{
	TIME_SET_HOUR(countdownTask_Time, 0);
	TIME_SET_MINUTE(countdownTask_Time, 0);
	TIME_SET_SECOND(countdownTask_Time, 0);
}
void CountdownTask_LooperUpdate(LOOPER *looper)
{
	
	if(IN_FOREGROUND)
	{
		Time_ToString(&countdownTask_Time, '_', 0, SEG_VIEW_ARRAY);
	}
	if(countdownTask_Mode == MODE_COMPLETE)
	{
		countdownTask_Mode = MODE_STOP;
		RingtoneManager_StartRing();
	}
	if(countdownTask_Mode == MODE_COUNTING || countdownTask_Mode == MODE_STOP)
	{
		if(Input_GetKeyDown() == KEYCODE_SETTING && IN_FOREGROUND)
		{
			countdownTask_Mode = MODE_SETTING;
		}
		else if(Input_GetKeyDown() == KEYCODE_START_OR_STOP && IN_FOREGROUND)
		{
			if(TIME_GET_SECOND(countdownTask_Time) == 0 && TIME_GET_MINUTE(countdownTask_Time) == 0 && TIME_GET_HOUR(countdownTask_Time) == 0)
			{
				countdownTask_Mode = MODE_STOP;
			}
			else
			{
				countdownTask_Mode = (countdownTask_Mode == MODE_COUNTING)?MODE_STOP:MODE_COUNTING;
			}
			
		}
	}
	else if(countdownTask_Mode == MODE_SETTING && IN_FOREGROUND)
	{
		SETTING_ITEM_SPARK_TIMER += looper->deltaTime;
		if(SETTING_ITEM_SPARK_TIMER >= SPARKING_DURATION/2)
		{
			SETTING_ITEM_SPARKSTATE = (SETTING_ITEM_SPARKSTATE == SPARKING_STATE_SHOW) ? SPARKING_STATE_HIDE : SPARKING_STATE_SHOW;
			SETTING_ITEM_SPARK_TIMER = 0;
			
		}
		if(Input_GetKeyDown() == KEYCODE_SETTING)
		{
			countdownTask_Mode = MODE_STOP;
			SETTING_ITEM = 0;
			SETTING_ITEM_SPARK_TIMER = 0;
			SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
			TIME_SET_DECISECOND(countdownTask_Time, 9);
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
					TIME_SET_HOUR(countdownTask_Time, (TIME_GET_HOUR(countdownTask_Time) + 1)%100);
				}
				else
				{
					TIME_SET_HOUR(countdownTask_Time, TIME_GET_HOUR(countdownTask_Time) == 0 ? 99 : (TIME_GET_HOUR(countdownTask_Time) - 1));
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
					TIME_SET_MINUTE(countdownTask_Time, (TIME_GET_MINUTE(countdownTask_Time) + 1)%60);
				}
				else
				{
					TIME_SET_MINUTE(countdownTask_Time, TIME_GET_MINUTE(countdownTask_Time) == 0 ? 59 : TIME_GET_MINUTE(countdownTask_Time) - 1);
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
				if(Input_GetKeyDown() == KEYCODE_SETTING_SETTING_ADD)
				{
					TIME_SET_SECOND(countdownTask_Time, (TIME_GET_SECOND(countdownTask_Time) + 1)%60);
				}
				else
				{
					TIME_SET_SECOND(countdownTask_Time, TIME_GET_SECOND(countdownTask_Time) == 0 ? 59 : TIME_GET_SECOND(countdownTask_Time) - 1);
				}
			}
			if(SETTING_ITEM_SPARKSTATE == SPARKING_STATE_HIDE)
			{
				SEG_VIEW_ARRAY[6] = ' ';
				SEG_VIEW_ARRAY[7] = ' ';
			}
		}
		
	}
	if(TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_COUNTDOWN)
	{
		SegScreen_Print_String(SEG_VIEW_ARRAY);
	}
	
}
void CountdownTask_OnDeciClockTrigger()
{
	if(countdownTask_Mode == MODE_COUNTING)
	{
		Time_Sub_1_Decisecond(&countdownTask_Time);
		if(TIME_GET_DECISECOND(countdownTask_Time) == 0 && TIME_GET_SECOND(countdownTask_Time) == 0 && TIME_GET_MINUTE(countdownTask_Time) == 0 && TIME_GET_HOUR(countdownTask_Time) == 0)
		{
			countdownTask_Mode = MODE_COMPLETE;
		}
	}
	
}
void CountdownTask_EnterTask()
{
	
}
void CountdownTask_ExitTask()
{
	SETTING_ITEM = 0;
	SETTING_ITEM_SPARK_TIMER = 0;
	SETTING_ITEM_SPARKSTATE = SPARKING_STATE_SHOW;
}

