/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2005 Keil Software, Inc.
------------------------------------------------------------------------------*/
#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "ClockInterface.h"
#include "StopwatchTask.h"
#include "TaskSelector.h"
#include "Time.h"
#include "DateInterface.h"
#include "TestTask.h"
#include "Input.h"
#include <REG51.H>                /* special function register declarations   */
#include <absacc.h>
#include <stdio.h>
                             /* Stop Exection with Serial Intr.   */
#define uchar unsigned char	
LOOPER looper;
void updateTest(LOOPER *looper);
char cnt = 0;
void OnTimer1();

void main (void) {
	Looper_Init(&looper);
	
	TMOD=0x00;
	TH1= 0x4C;
	TL1 = 0x00;
	TR1=1;
	EA=1;
	ET1=1;
	
	while(1)
	{
		Looper_Update(&looper, 1);
		if(cnt >= 20)
		{
			cnt = 0;
			OnTimer1();
		}
	}
}


void timer1() interrupt 3
{
	TH1= 0x4C;
	TL1 = 0x00;
	cnt++;
}

void OnTimer1()
{
	ClockInterface_OnDeciClockTrigger();
	StopwatchTask_OnDeciClockTrigger();
}





