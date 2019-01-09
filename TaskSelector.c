#include "Looper\\Looper.h"
#include "DateInterface.h"
#include "ClockInterface.h"
#include "Input.h"
#define TASKSELECTOR_MODE_CLOCK 0
#define TASKSELECTOR_MODE_DATE 1
#define TASKSELECTOR_KEYCODE INPUT_TOKEYCODE(4, 1)

char taskSelector_Mode = 0;
void TaskSelector_LooperUpdate(LOOPER *looper)
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
			ClockInterface_EnterTask();
			taskSelector_Mode = TASKSELECTOR_MODE_CLOCK;
		}
	}
}