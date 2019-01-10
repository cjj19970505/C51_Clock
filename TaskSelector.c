#include "Looper\\Looper.h"
#include "DateInterface.h"
#include "ClockInterface.h"
#include "StopwatchTask.h"
#include "Input.h"
#include "TaskSelector.h"
#include "AlarmTask.h"

#define TASKSELECTOR_KEYCODE INPUT_TOKEYCODE(4, 1)

char taskSelector_Mode = TASKSELECTOR_MODE_CLOCK;
void TaskSelector_LooperUpdate()
{
	if(Input_GetKeyDown() == TASKSELECTOR_KEYCODE)
	{
		if(taskSelector_Mode == TASKSELECTOR_MODE_CLOCK)
		{
			ClockInterface_ExitTask();
			DateInterface_EnterTask();
			taskSelector_Mode = TASKSELECTOR_MODE_DATE;
		}
		else if(taskSelector_Mode == TASKSELECTOR_MODE_DATE)
		{
			DateInterface_ExitTask();
			StopwatchTask_EnterTask();
			taskSelector_Mode = TASKSELECTOR_MODE_STOPWATCH;
		}
		else if(taskSelector_Mode == TASKSELECTOR_MODE_STOPWATCH)
		{
			StopwatchTask_ExitTask();
			AlarmTask_EnterTask();
			taskSelector_Mode = TASKSELECTOR_MODE_ALARM;
		}
		else if(taskSelector_Mode == TASKSELECTOR_MODE_ALARM)
		{
			AlarmTask_ExitTask();
			ClockInterface_EnterTask();
			taskSelector_Mode = TASKSELECTOR_MODE_CLOCK;
		}
	}
}