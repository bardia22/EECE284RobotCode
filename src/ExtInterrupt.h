/*-------------------------------------------------*/
// EECE 284
// ExtInterrupt.h
// Jan. 23, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__EXTINTERRUPT__H_
#define _H__EXTINTERRUPT__H_

#include <p89lpc9351.h>
#include "PWM.h"

volatile unsigned char turnCounter = 0;
volatile double lastInterruptTime = 0;
volatile unsigned char bRun = 0;
volatile double currentTime = 0;
volatile double diffTime = 0;
static float const consecutiveTurnSignalTime = 0.8;

void InitializeExtInterrupt();
void Turn(unsigned char direction);

void InitializeExtInterrupt()
{
	/*-----------------------------------------------
	Configure INT0 (external interrupt 0) to generate
	an interrupt on the falling-edge of /INT0 (P1.3).
	Enable the EX0 interrupt and then enable the
	global interrupt flag.
	-----------------------------------------------*/
	IT0 = 1;   // Configure interrupt 0 for falling edge on /INT0 (P3.2)
	EX0 = 1;   // Enable EX0 Interrupt
	EA = 1;    // Enable Global Interrupt Flag
	IP0H = 0; // interrupt priority
	IP0_0 = 0; // interrupt priority
}

void Turn(unsigned char direction)
{
	if (direction == 1) // turn right
	{
		pwmR = 8;
		pwmL = 100;
		Delay(7500);
	}
	else // turn left
	{
		pwmR = 100;
		pwmL = 8;
		Delay(8000);
	}
}

void TurnSignalCounter(void) interrupt 0
{
	currentTime = absTime; // store the time the interrupt was triggered 
	diffTime = currentTime - lastInterruptTime; // store the time since last interrupt
	
	if (diffTime < consecutiveTurnSignalTime) // if less than allowed threshold
	{	
		++turnCounter;   // Increment the counter			
		if (turnCounter == 3) // if 4 wires have been seen in a row
		{
			if (bRun == 0 || absTime > 70)
				++bRun;
			else
				turnCounter = 0;
				
			if (bRun == 1) // start line has been seen, reset all values
			{
				absTime = 0;
				turnCounter = 0;
				currentTime = 0;
				lastInterruptTime = 0;
			}
		}
	}
	
	else if (diffTime < 2) // if diffTime between 0.8 and 2 seconds, it's a turn intersection
	{	
		if (turnCounter == 1) // if two wires have been seen
			Turn(0 /* left */);
			
		else if (turnCounter == 2) // if three wires have been seen
			Turn(1 /* right */);
		
		turnCounter = 0;
	}
	
	else
		turnCounter = 0;

	lastInterruptTime = currentTime; // update the time interrupt was seen 
}

#endif /* _H__EXTINTERRUPT__H_ */