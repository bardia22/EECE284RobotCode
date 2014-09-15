#include <p89lpc9351.h>
#include "LCD.h"
#include "Pins.h"
#include "PWM.h"
#include "PID.h"
#include "ExtInterrupt.h"
 
void main(void) 
{
	unsigned int lcdPrintCounter = 0; // counter to keep track of when to write to lcd

	InitializePin(); // initialize the pins as output/input
	InitializeADC(); // initialize ADC
	InitializeTimer0(); // initialize timer0
	InitializeExtInterrupt(); // initialize external interrupt
	InitializeLCD(); // initialize LCD
	InitializeBatteryVoltage(); // initialize battery voltage values in memory

	while(bRun != 2) // while robot is running 
	{
		left = 0.75 * AD0DAT1; // left inductor
		right = 1.25 * AD1DAT0; // right inductor
		
		error = left - right; // error in the PID control is the difference in the readings of the sensors
		gain = PIDUpdate(error); // gain is the output of the PID blackbox, if error is input.

		pwmL = (int)(100 - gain); // setting left motor speed
		pwmR = (int)(100 + gain); // setting right motor speed
		
		if (pwmL > 100) pwmL = 100; // capping pwm left and right to a max and a min value
		if (pwmR > 100) pwmR = 100;
		if (pwmL < 1) pwmL = 1;
		if (pwmR < 1) pwmR = 1;
		
		if (lcdPrintCounter % 80 == 0) // roughly about 0.1s
		{
			absTimeDec = ((int)(absTime * 10)) % 10; // stores the first decimal values of the current time
			lcdPrintCounter = 0; // resets the counter
					
			LCDReset(); // resets and prints to LCD
			LCDFirstLine();
			LCDWriteString("L"); LCDWriteInteger(left); LCDWriteString(" R"); LCDWriteInteger(right); 
			LCDWriteString(" M"); LCDWriteInteger(middle); LCDWriteString(" INT"); LCDWriteInteger(turn);
			LCDSecondLine(); 
			LCDWriteString("C"); LCDWriteInteger(turnCounter); LCDWriteString(" T:"); LCDWriteInteger((int)absTime);
			LCDWriteString("."); LCDWriteInteger(absTimeDec); LCDWriteString(" "); LCDWriteInteger(batteryVoltage);
			LCDWriteString("."); LCDWriteInteger(batteryVoltageDec); LCDWriteString("V");
		}
		
		++lcdPrintCounter;
	}
	
	// once 4 perpendicular lines have been seen, the robot stops
	pwmL = 0;
	pwmR = 0;
}
