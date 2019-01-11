#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "RingtoneManager.h"
#include "BinHelper.h"
#include "TaskUtility.h"
#include "Input.h"
#include <REG51.H>                /* special function register declarations   */
#include <absacc.h>
#include <stdio.h>
#define BEEPER_VALID 0
#define BEEPER_INVALID 1
sbit P1_6 = P1^6;
#define SPARK_DURATION 200
#define STATE_RINGING 1
#define STATE_NORINGING 0
#define GET_RING_STATE (BIN_GET_BIT(ringtoneManager_Control, 0))
#define SET_RING_STATE(state) (BIN_SET_BIT(ringtoneManager_Control, 0, (state)))
unsigned char ringtoneManager_Spark_Timer;
#define MUSIC_LENGTH 7
unsigned int code ringtoneManager_Music[] = {62018u, 62401u, 62743u, 62895u, 63185u, 63441u, 63670u};
unsigned int code ringtoneManager_Music_Interval[] = {100, 100, 100, 100, 100, 100, 100};
char ringtoneManager_Control;
unsigned char ringtoneManager_CurrNoteIndex = 0;
unsigned int ringtoneManager_NoteIntervalTimer = 0;


void RingtoneManager_Init()
{
	SET_RING_STATE(STATE_NORINGING);
	ringtoneManager_Spark_Timer = 0;
	ringtoneManager_NoteIntervalTimer = 0;
	ringtoneManager_CurrNoteIndex = 0;
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
	else if(GET_RING_STATE == STATE_RINGING)
	{
		if(Input_GetKeyDown() != 0)
		{
			RingtoneManager_StopRing();
		}
			
		if(ringtoneManager_Spark_Timer >= SPARK_DURATION/2)
		{
			TaskUtility_Public_Var_SegViewArr[0] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[1] = '-';
			TaskUtility_Public_Var_SegViewArr[2] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[3] = '-';
			TaskUtility_Public_Var_SegViewArr[4] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[5] = '-';
			TaskUtility_Public_Var_SegViewArr[6] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[7] = '-';
			TaskUtility_Public_Var_SegViewArr[8] = '\0';
		}
		else
		{
			TaskUtility_Public_Var_SegViewArr[0] = '_';
			TaskUtility_Public_Var_SegViewArr[1] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[2] = '_';
			TaskUtility_Public_Var_SegViewArr[3] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[4] = '_';
			TaskUtility_Public_Var_SegViewArr[5] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[6] = '_';
			TaskUtility_Public_Var_SegViewArr[7] = ringtoneManager_CurrNoteIndex + '0';
			TaskUtility_Public_Var_SegViewArr[8] = '\0';
		}
		ringtoneManager_Spark_Timer = ringtoneManager_Spark_Timer + looper->deltaTime;
		ringtoneManager_Spark_Timer = ringtoneManager_Spark_Timer % SPARK_DURATION;
		SegScreen_Print_String(TaskUtility_Public_Var_SegViewArr);
		
		if(ringtoneManager_NoteIntervalTimer >= ringtoneManager_Music_Interval[ringtoneManager_CurrNoteIndex])
		{
			ringtoneManager_NoteIntervalTimer = 0;
			ringtoneManager_CurrNoteIndex = (ringtoneManager_CurrNoteIndex + 1) % MUSIC_LENGTH;
		}
		ringtoneManager_NoteIntervalTimer = ringtoneManager_NoteIntervalTimer + looper->deltaTime;
	}
	
}



void RingtoneManager_StartRing()
{
	SET_RING_STATE(STATE_RINGING);
	ringtoneManager_Spark_Timer = 0;
	ringtoneManager_CurrNoteIndex = 0;
	ringtoneManager_NoteIntervalTimer = 0;
}
void RingtoneManager_StopRing()
{
	SET_RING_STATE(STATE_NORINGING);
	ringtoneManager_Spark_Timer = 0;
	
	ringtoneManager_CurrNoteIndex = 0;
	ringtoneManager_NoteIntervalTimer = 0;
	P1_6 = 0;
}

void timer0() interrupt 1
{
	TH0 = ringtoneManager_Music[ringtoneManager_CurrNoteIndex] / 256;
	TL0 = ringtoneManager_Music[ringtoneManager_CurrNoteIndex] % 256;
	if(GET_RING_STATE == STATE_RINGING)
	{
		P1_6 = ~P1_6;
	}
	else
	{
		P1_6 = BEEPER_INVALID;
	}
	//cnt++;
	//P1_6 = ~P1_6;
}



