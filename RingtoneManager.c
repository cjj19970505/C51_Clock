#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "RingtoneManager.h"
#include "BinHelper.h"
#include "TaskUtility.h"
#include "Input.h"
#define SPARK_DURATION 200
#define STATE_RINGING 1
#define STATE_NORINGING 0
#define GET_RING_STATE (BIN_GET_BIT(ringtoneManager_Control, 0))
#define SET_RING_STATE(state) (BIN_SET_BIT(ringtoneManager_Control, 0, (state)))
unsigned char ringtoneManager_Spark_Timer;
char ringtoneManager_Control;

void RingtoneManager_Init()
{
	SET_RING_STATE(STATE_NORINGING);
	ringtoneManager_Spark_Timer = 0;
}

void RingtoneManager_LooperUpdate(LOOPER *looper)
{
	if(GET_RING_STATE == STATE_NORINGING)
	{
		if(Input_GetKeyDown() == INPUT_TOKEYCODE(3,2))
		{
			RingtoneManager_StartRing();
		}
	}
	else
	{
		if(Input_GetKeyDown() != 0)
		{
			RingtoneManager_StopRing();
		}
			
		if(ringtoneManager_Spark_Timer >= SPARK_DURATION/2)
		{
			TaskUtility_Public_Var_SegViewArr[0] = '-';
			TaskUtility_Public_Var_SegViewArr[1] = '-';
			TaskUtility_Public_Var_SegViewArr[2] = '-';
			TaskUtility_Public_Var_SegViewArr[3] = '-';
			TaskUtility_Public_Var_SegViewArr[4] = '-';
			TaskUtility_Public_Var_SegViewArr[5] = '-';
			TaskUtility_Public_Var_SegViewArr[6] = '-';
			TaskUtility_Public_Var_SegViewArr[7] = '-';
			TaskUtility_Public_Var_SegViewArr[8] = '\0';
		}
		else
		{
			TaskUtility_Public_Var_SegViewArr[0] = '0';
			TaskUtility_Public_Var_SegViewArr[1] = '8';
			TaskUtility_Public_Var_SegViewArr[2] = '0';
			TaskUtility_Public_Var_SegViewArr[3] = '8';
			TaskUtility_Public_Var_SegViewArr[4] = '0';
			TaskUtility_Public_Var_SegViewArr[5] = '8';
			TaskUtility_Public_Var_SegViewArr[6] = '0';
			TaskUtility_Public_Var_SegViewArr[7] = '8';
			TaskUtility_Public_Var_SegViewArr[8] = '\0';
		}
		ringtoneManager_Spark_Timer = ringtoneManager_Spark_Timer + looper->deltaTime;
		ringtoneManager_Spark_Timer = ringtoneManager_Spark_Timer % SPARK_DURATION;
		SegScreen_Print_String(TaskUtility_Public_Var_SegViewArr);
	}
	
}



void RingtoneManager_StartRing()
{
	SET_RING_STATE(STATE_RINGING);
	ringtoneManager_Spark_Timer = 0;
}
void RingtoneManager_StopRing()
{
	SET_RING_STATE(STATE_NORINGING);
	ringtoneManager_Spark_Timer = 0;
}

