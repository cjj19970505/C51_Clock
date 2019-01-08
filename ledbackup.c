/*------------------------------------------------------------------------------
HELLO.C

Copyright 1995-2005 Keil Software, Inc.
------------------------------------------------------------------------------*/
#include "Looper\\Looper.h"
#include <REG51.H>                /* special function register declarations   */
#include <absacc.h>
#include <stdio.h>
                             /* Stop Exection with Serial Intr.   */
                                         /* is enabled                        */
#define uchar unsigned char	
unsigned char code led_table[]={			              //?????
	0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00,0x08,0x40,0x79	}; 

	
void delay(unsigned char  p)				//????
{
	unsigned char  i,j; 				
	for(;p>0;p--) 				
		for(i=181;i>0;i--) 		//
			for(j=181;j>0;j--);
}	
void delay2(unsigned char  p)				//????
{
	unsigned char  i,j; 				
	for(;p>0;p--) 				
		for(i=10;i>0;i--) 		//
			for(j=10;j>0;j--);
}	
int arrtest[6];
unsigned char getkeycode(void)    /*??????,??????*/
{
	unsigned char line=0x00;        /*??*/
	unsigned char col=0x00; /*??*/
	unsigned char scancode=0x01;    /*????*/
	unsigned char keycode;          /*??*/

	XBYTE[0x8000]=0xff;
	col=XBYTE[0x8000]&0x0f;      /*??????????*/
	if (col==0x00)
		keycode=0x00;
	else
	{
		while((scancode&0x0f)!=0) /*?scancode????,????0,??*/
		{
			line=scancode;                /*??*/
			XBYTE[0x8000]=scancode;     /*??????,????0x01*/
			if((XBYTE[0x8000]&0x0f)==col) /*????????????*/
			break; 
			scancode=scancode<<1;         /*????????,????*/
		}
		col=col<<4;                     /*????????*/
		keycode=col|line;
	}
	return keycode;
}
int preKey = 0;
int segMaskToIndex(int segMask);
void decodeKeycode(char keycode, int *row, int *column);
void setJumpMode(int *jumpStep, int step0, int step1, int step2, int step3, int step4, int step5, int step6, int step7);
LOOPER looper;
void main (void) {
	int segMask = 0x01;
	int mode = 0;
	int segArr[8];
	int dotArr[8];
	int jumpStep[8]; 
	int i = 0;
	int jumpIndex = 0;
	for(i = 0; i < 8; i++){
		segArr[i] = 0;
		jumpStep[i] = 1;
		dotArr[i] = 0;
	}
	
	while(1){
		char key = getkeycode();
		
		int keyRow = 0;
		int keyColumn = 0;
		if(key != preKey){
			decodeKeycode(key, &keyRow, &keyColumn);
			preKey = key;
		}
		
		//segArr[0] = keyColumn;
		//segArr[1] = keyRow;
		if(keyRow == 1 && keyColumn == 1){
			segArr[7] = (segArr[7] + 1)%10;
		}else if(keyRow == 1 && keyColumn == 2){
			segArr[6] = (segArr[6] + 1)%10;
		}else if(keyRow == 1 && keyColumn == 3){
			segArr[5] = (segArr[5] + 1)%10;
		}else if(keyRow == 1 && keyColumn == 4){
			segArr[4] = (segArr[4] + 1)%10;
		}else if(keyRow == 2 && keyColumn == 1){
			segArr[3] = (segArr[3] + 1)%10;
		}else if(keyRow == 2 && keyColumn == 2){
			segArr[2] = (segArr[2] + 1)%10;
		}else if(keyRow == 2 && keyColumn == 3){
			segArr[1] = (segArr[1] + 1)%10;
		}else if(keyRow == 2 && keyColumn == 4){
			segArr[0] = (segArr[0] + 1)%10;
		}else if(keyRow == 3 && keyColumn == 1){
			setJumpMode(jumpStep, 1, 1, 1, 1, 1, 1, 1, 1);
			mode = 0;
		}else if(keyRow == 3 && keyColumn == 2){
			setJumpMode(jumpStep, -1, -1, -1, -1, -1, -1, -1, -1);
			mode = 0;
		}else if(keyRow == 3 && keyColumn == 3){
			setJumpMode(jumpStep, -1, 1, 1, -1, 1, 1, -1, 1);
			mode = 0;
		}else if(keyRow == 4 && keyColumn == 4){
			setJumpMode(jumpStep, 1, 1, 1, 1, 1, 1, 1, 1);
			mode = 1;
		}
		if(jumpStep[jumpIndex] > 0){
			segMask = (segMask << jumpStep[jumpIndex]) & 0xff;
			if (segMask == 0){
				segMask = 0x01;
			}
		}
		else if(jumpStep[jumpIndex] < 0){
			segMask = (segMask >> (-jumpStep[jumpIndex])) & 0xff;
			if (segMask == 0){
				segMask = 0x80;
			}
		}
		jumpIndex = (jumpIndex + 1) % 8;
		Looper_Init(&looper);
		segArr[4] = looper.inited;
		if(segArr[segMaskToIndex(segMask)] == -1){
			XBYTE[0x9000] = 0x00 | dotArr[segMaskToIndex(segMask)];
		}
		else
		{
			XBYTE[0x9000]= led_table[segArr[segMaskToIndex(segMask)]] | dotArr[segMaskToIndex(segMask)];
		}
		XBYTE[0x8000] = segMask;
		if(mode == 0){
			delay(1);
		}
		else if(mode == 1){
			delay2(1);
		}
		
	}
}
void setJumpMode(int *jumpStep, int step0, int step1, int step2, int step3, int step4, int step5, int step6, int step7){
	jumpStep[0] = step0;
	jumpStep[1] = step1;
	jumpStep[2] = step2;
	jumpStep[3] = step3;
	jumpStep[4] = step4;
	jumpStep[5] = step5;
	jumpStep[6] = step6;
	jumpStep[7] = step7;
}
int segMaskToIndex(int segMask){
	switch(segMask){
		case 0x01:
			return 0;
		case 0x02:
			return 1;
		case 0x04:
			return 2;
		case 0x08:
			return 3;
		case 0x10:
			return 4;
		case 0x20:
			return 5;
		case 0x40:
			return 6;
		case 0x80:
			return 7;
		default:
			return -1;
	}
}

void decodeKeycode(char keycode, int *row, int *column){
	switch(keycode){
		case 0x11: *row = 1; *column = 1; break;
		case 0x21: *row = 1; *column = 2; break;
		case 0x41: *row = 1; *column = 3; break;
		case 0x81: *row = 1; *column = 4; break;
		
		case 0x12: *row = 2; *column = 1; break;
		case 0x22: *row = 2; *column = 2; break;
		case 0x42: *row = 2; *column = 3; break;
		case 0x82: *row = 2; *column = 4; break;
		
		case 0x14: *row = 3; *column = 1; break;
		case 0x24: *row = 3; *column = 2; break;
		case 0x44: *row = 3; *column = 3; break;
		case 0x84: *row = 3; *column = 4; break;
		
		case 0x18: *row = 4; *column = 1; break;
		case 0x28: *row = 4; *column = 2; break;
		case 0x48: *row = 4; *column = 3; break;
		case 0x88: *row = 4; *column = 4; break;
		
		default: *row = 0; *column = 0;
	}
}

