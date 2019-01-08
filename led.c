/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2005 Keil Software, Inc.
------------------------------------------------------------------------------*/
#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "Time.h"
#include "Input.h"
#include <REG51.H>                /* special function register declarations   */
#include <absacc.h>
#include <stdio.h>
                             /* Stop Exection with Serial Intr.   */
#define uchar unsigned char	
LOOPER looper;
void updateTest(LOOPER *looper);
char teststr[10];
void main (void) {
	Looper_Init(&looper);
	Looper_AddTask(&looper, Input_LooperUpdate);
	Looper_AddTask(&looper, updateTest);
	Looper_AddTask(&looper, SegScreen_LooperUpdate);
	
	while(1)
	{
		Looper_Update(&looper, 1);
	}
}
void updateTest(LOOPER *looper)
{
	TIME t;
	TIME_SET_HOUR(t, 40);
	TIME_SET_MINUTE(t, 45);
	TIME_SET_SECOND(t, 9);
	Time_ToString(&t, teststr);
	//SegScreen_Print_Int(INPUT_TOKEYCODE(3,2) == Input_GetLatestKeyCode());
	SegScreen_Print_String(teststr);
}



