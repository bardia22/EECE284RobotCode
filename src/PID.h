/*-------------------------------------------------*/
// EECE 284
// PID.h
// Jan. 23, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__PID__H_
#define _H__PID__H_

#include <p89lpc9351.h>

#define proportionalGain 12 // the proportional gain of the PID control
#define derivativeGain 25 // the derivative gain of the PID control
#define integralGain 0 // the integral gain of the PID control

volatile int currError = 0; // current error of the control system
volatile int lastError = 0; // last error of the control system
volatile unsigned char left = 0; // left sensor
volatile unsigned char right = 0; // right sensor
volatile int error = 0;
volatile float gain = 0;

void SetError_(int error); // set the new error, reset the duration
float CalcD_(); // calculates the derivative response of the control system
float CalcP_(); // calculates the proportional response of the control system
float PIDUpdate(int error); // calculates the response of PID based on system inputs

void SetError_(int error) // set the new error, reset the duration
{
	lastError = currError;
	currError = error;
}

float CalcD_() // calculates the derivative response of the control system
{
	return derivativeGain * (currError - lastError);
}

float CalcP_() // calculates the proportional response of the control system
{
	return proportionalGain * currError;
}

float PIDUpdate(int error) // calculates the response of PID based on system inputs
{
	float gain = 0;
	SetError_(error);
	gain = CalcP_() + CalcD_();
	return gain;  
}

#endif /* _H__PID__H_ */