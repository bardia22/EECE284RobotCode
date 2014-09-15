/*-------------------------------------------------*/
// EECE 284
// LCD.h
// Jan. 21, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__LCD__H_
#define _H__LCD__H_

#include <stdio.h>
#include <stdlib.h>
#include <p89lpc9351.h>
#include "Pins.h"
#include "PWM.h"

#define lcdDelayShort 3 // 300 usec
#define lcdDelayLong 17 // 1700 usec

unsigned char lcdFirstLineCursor = 0; // current cursor position in the first line
unsigned char lcdSecondLineCursor = 0; // current cursor position in the second line
unsigned char lineToWrite = 0; // the line to write to

void LCDCmd(unsigned char item);  //Function to send command to LCD
void LCDData(unsigned char item);  //Function to send data to LCD
void InitializeLCD();
void LCDFirstLine(); // Bring the cursor to the start of first line
void LCDSecondLine(); // Bring the cursor to the start of second line
void LCDReset(); // Clears the LCD and takes the cursor back home
void LCDWriteString(unsigned char* item);
void LCDWriteInteger(unsigned int item);

void LCDCmd(unsigned char item)  //Function to send command to LCD
{
	dataPort = item;
	registerSelect = 0;
	readWrite = 0;
	enable = 1;
	Delay(lcdDelayShort);
	enable = 0;
}

void LCDData(unsigned char item)  //Function to send data to LCD
{
	dataPort = item;
	registerSelect = 1;
	readWrite = 0;
	enable = 1;
	Delay(lcdDelayShort);
	enable = 0;
}

void InitializeLCD()
{
	LCDCmd(0x38);  // for using 8-bit 2 row mode of LCD
	Delay(lcdDelayShort);
	LCDCmd(0x0E);  // turn display ON for cursor blinking
	Delay(lcdDelayShort);
	LCDCmd(0x01);  //clear screen
	Delay(lcdDelayShort);
	LCDCmd(0x06);  //display ON
	Delay(lcdDelayShort);
}

void LCDFirstLine() // Bring the cursor to the start of first line
{
	LCDCmd(0x80);
	Delay(lcdDelayShort);
	lineToWrite = 0;
}

void LCDSecondLine() // Bring the cursor to the start of second line
{
	LCDCmd(0xc0);
	Delay(lcdDelayShort);
	lineToWrite = 1;
}

void LCDReset() // Clears the LCD and takes the cursor back home
{
	LCDCmd(0x01); // clears the LCD
	Delay(lcdDelayLong);
	LCDCmd(0x02); // takes the cursor back home
	Delay(lcdDelayLong);
	lcdFirstLineCursor = 0;
	lcdSecondLineCursor = 0;
}

void LCDWriteString(unsigned char* item)
{
	char count = 0;
	
	// until run out of characters
	while (*(item + count) != '\0')
	{
		if (*(item + count) != 0)
		{
			LCDData(*(item + count));
			++count;
			LCDCmd(0x6);
			lineToWrite ? ++lcdSecondLineCursor : ++lcdFirstLineCursor;
			if ( ( lineToWrite ? lcdSecondLineCursor : lcdFirstLineCursor ) >= 16) // if goes out of screen
				break;
		}
	}
}

void LCDWriteInteger(unsigned int item)
{
	char buffer[8] = "";
	
	sprintf(buffer, "%d", item);
	
	LCDWriteString(buffer);
}

#endif /* _H__LCD__H_ */