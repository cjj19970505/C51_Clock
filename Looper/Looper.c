#include "Looper.h"
#include "..\\SegScreen\\SegScreen.h"
#include "..\\TaskSelector.h"
#include "..\\Time.h"
#include "..\\ClockInterface.h"
#include "..\\DateInterface.h"
#include "..\\StopwatchTask.h"
#include "..\\TestTask.h"
#include "..\\Input.h"
#include "..\\AlarmTask.h"
#include "..\\RingtoneManager.h"
#include "..\\CountdownTask.h"
char looper_temp_i;
void Looper_Init(LOOPER *looper)
{
	looper->inited = 1;
	ClockInterface_Init();
	DateInterface_Init();
	StopwatchTask_Init();
	AlarmTask_Init();
	CountdownTask_Init();
	RingtoneManager_Init();
	
}
void Looper_Update(LOOPER *looper, int deltaTime)
{
	looper->deltaTime = deltaTime;
	
	Input_LooperUpdate(looper);
	TaskSelector_LooperUpdate();
	ClockInterface_LooperUpdate(looper);
	DateInterface_LooperUpdate(looper);
	StopwatchTask_LooperUpdate(looper);
	AlarmTask_LooperUpdate(looper);
	CountdownTask_LooperUpdate(looper);
	TestTask_LooperUpdate(looper);
	RingtoneManager_LooperUpdate(looper);
	SegScreen_LooperUpdate(looper);
}
