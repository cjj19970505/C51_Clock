#include "Looper.h"

int looper_temp_i;
void Looper_Init(LOOPER *looper)
{
	looper->inited = 1;
	looper->taskCount = 0;
}
void Looper_AddTask(LOOPER *looper, void (*updateFunc)(LOOPER*))
{
	looper->updateFunctions[looper->taskCount] = updateFunc;
	looper->taskCount++;
}
void Looper_Update(LOOPER *looper, int deltaTimeInMS)
{
	looper->deltaTimeInMS = deltaTimeInMS;
	for(looper_temp_i = 0; looper_temp_i < looper->taskCount; looper_temp_i++)
	{
		looper->updateFunctions[looper_temp_i](looper);
	}
}
