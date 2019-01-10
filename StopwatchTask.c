#include "Looper\\Looper.h"
#include "Time.h"
#include "Input.h"
#include "SegScreen\\SegScreen.h"
#include "TaskUtility.h"
#include "TaskSelector.h"

#define IN_FOREGROUND (TASKSELECTOR_CURRENT_MODE == TASKSELECTOR_MODE_STOPWATCH)

#define SEG_VIEW_ARRAY TaskUtility_Public_Var_SegViewArr

#define MODE_STOP 0
#define MODE_RUNNING 1

#define KEYCODE_RUN_OR_STOP INPUT_TOKEYCODE(1,2)
#define KEYCODE_INIT INPUT_TOKEYCODE(1,3)

TIME stopwatchTask_Time;
char stopwatchTask_Mode = MODE_STOP;
void StopwatchTask_Init()
{
	TIME_SET_HOUR(stopwatchTask_Time, 0);
	TIME_SET_MINUTE(stopwatchTask_Time, 0);
	TIME_SET_SECOND(stopwatchTask_Time, 0);
}
void StopwatchTask_LooperUpdate(LOOPER *looper)
{
	if(IN_FOREGROUND)
	{
		Time_ToString(&stopwatchTask_Time, '.', 1,SEG_VIEW_ARRAY);
	}
	if(stopwatchTask_Mode == MODE_STOP && IN_FOREGROUND)
	{
		if(Input_GetKeyDown() == KEYCODE_RUN_OR_STOP)
		{
			stopwatchTask_Mode = MODE_RUNNING;
		}
		else if(Input_GetKeyDown() == KEYCODE_INIT)
		{
			TIME_SET_HOUR(stopwatchTask_Time, 0);
			TIME_SET_MINUTE(stopwatchTask_Time, 0);
			TIME_SET_SECOND(stopwatchTask_Time, 0);
			TIME_SET_DECISECOND(stopwatchTask_Time, 0);
		}
	}
	else if(stopwatchTask_Mode == MODE_RUNNING && IN_FOREGROUND)
	{
		if(Input_GetKeyDown() == KEYCODE_RUN_OR_STOP)
		{
			stopwatchTask_Mode = MODE_STOP;
		}
	}
	
	if(IN_FOREGROUND)
	{
		SegScreen_Print_String(SEG_VIEW_ARRAY);
	}
	
}

void StopwatchTask_OnDeciClockTrigger()
{
	if(stopwatchTask_Mode == MODE_RUNNING)
	{
		Time_Add_1_Decisecond(&stopwatchTask_Time);
	}
}
void StopwatchTask_EnterTask()
{
}

void StopwatchTask_ExitTask()
{
}