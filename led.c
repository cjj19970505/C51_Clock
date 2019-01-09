/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2005 Keil Software, Inc.
------------------------------------------------------------------------------*/
#include "Looper\\Looper.h"
#include "SegScreen\\SegScreen.h"
#include "ClockInterface.h"
#include "Time.h"
#include "DateInterface.h"
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
	Looper_AddTask(&looper, Input_LooperUpdate);
	//Looper_AddTask(&looper, updateTest);
	Looper_AddTask(&looper, ClockInterface_LooperUpdate);
	//Looper_AddTask(&looper, DateInterface_LooperUpdate);
	Looper_AddTask(&looper, SegScreen_LooperUpdate);
	
	TMOD=0x00;
	TH1= 0xEE;
	TL1 = 0x00;
	TR1=1;
	EA=1;
	ET1=1;
	
	ClockInterface_Init();
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
	TH1= 0x3C;
	TL1 = 0xB0;
	cnt++;
}

void OnTimer1()
{
	ClockInterface_OnDeciClockTrigger();
}





