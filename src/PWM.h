/*-------------------------------------------------*/
// EECE 284
// PWM.h
// Jan. 23, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__PWM__H_
#define _H__PWM__H_

#include <p89lpc9351.h>

#define XTAL 7373000L
//We want timer 0 to interrupt every 100 microseconds ((1/10000Hz)=100 us)
#define FREQ 11300L
//The reload value formula comes from the datasheet...
#define TIMER0_RELOAD_VALUE (65536L-((XTAL)/(2*FREQ)))

volatile unsigned char pwmCount = 0; // pwm signal counter
volatile int pwmL = 100; // left motor pwm duty cycle
volatile int pwmR = 100; // right motor pwm duty cycle
volatile unsigned int q = 0; // delay tracker
volatile unsigned char delayOn = 0; // turn off delay
volatile double absTime = 0; // absolute time
volatile unsigned char absTimeDec = 0; // absolute time decimal

void InitializeTimer0(void); // initializes the timer0  
void Delay(long hundredusec);

void InitializeTimer0(void) // initializes the timer0  
{
	// Initialize timer 0 for ISR 'pwmcounter' below
	TR0 = 0; // Stop timer 0
	TF0 = 0; // Clear the overflow flag
	TMOD = (TMOD & 0xf0) | 0x01; // 16-bit timer
	TH0 = TIMER0_RELOAD_VALUE / 0x100;
	TL0 = TIMER0_RELOAD_VALUE % 0x100;
	TR0 = 1; // Start timer 0
	ET0 = 1; // Enable timer 0 interrupt
	EA = 1;  // Enable global interrupts
	IP0_1 = 1; // interrupt priority
}

//Interrupt 1 is for timer 0.  This function is executed every 100 us.
void PWMCounter(void) interrupt 1 using 1
{
	TF0 = 0; // Clear the overflow flag
	//Reload the timer
	TR0 = 0; // Stop timer 0
	TH0 = TIMER0_RELOAD_VALUE / 0x100;
	TL0 = TIMER0_RELOAD_VALUE % 0x100;
	TR0 = 1; // Start timer 0
	if(++pwmCount > 99) 
	{
		pwmCount = 0;
		absTime += 0.01; // increment the absTime by 0.01s.
	}
	leftMotor = (pwmL < pwmCount) ? 1 : 0;
	rightMotor = (pwmR < pwmCount) ? 1 : 0;
	
	if(delayOn) ++q; // increment the counter when delay is on
}

void Delay(long hundredusec)
{
	q = 0; // reset the tracker
	delayOn = 1;
	while(q < hundredusec)
	{}
	delayOn = 0;
}

#endif /* _H__PWM__H_ */