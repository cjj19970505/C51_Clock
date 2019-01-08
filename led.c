/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2005 Keil Software, Inc.
------------------------------------------------------------------------------*/
#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "Input.h"
#include <REG51.H>                /* special function register declarations   */
#include <absacc.h>
#include <stdio.h>
                             /* Stop Exection with Serial Intr.   */
#define uchar unsigned char	
LOOPER looper;
void updateTest(LOOPER *looper);
char teststr[] = {'1', '2', '3','.', '4', '9', '.','\0'};
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
	//SegScreen_Print_Int(INPUT_TOKEYCODE(3,2) == Input_GetLatestKeyCode());
	SegScreen_Print_String(teststr);
}

