/*-------------------------------------------------*/
// EECE 284
// Pins.h
// Jan. 21, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__PINS__H_
#define _H__PINS__H_

#include <p89lpc9351.h>

#define leftInductor P0_0
#define rightInductor P0_1
#define turnInductor P0_2
//#define batteryVoltage P0_3

#define leftMotor P0_6
#define rightMotor P0_7

#define turn P1_3

#define registerSelect P1_4
#define readWrite P1_6
#define enable P1_7

#define dataPort P2

unsigned char batteryVoltage = 0;
unsigned char batteryVoltageDec = 0; // first decimal

void InitializeADC(void);
void InitializePin();
void InitializeBatteryVoltage();

void InitializeADC(void)
{
	// Set adc1 (pins 0.1, 0.2, and 0.3) and adc0 (pin 0.0) channel pins as input only 
	P0M1 |= (P0M1_3 | P0M1_2 | P0M1_1 | P0M1_0);
	P0M2 &= ~(P0M1_3 | P0M1_2 | P0M1_1 | P0M1_0);	

	BURST0 = 1; // ADC0 autoscan continuous conversion mode
	BURST1 = 1; // ADC1 autoscan continuous conversion mode
	
	ADMODB = CLK0; // ADC0, ADC1 clock is 7.3728MHz/2
	ADINS  = (ADI12 | ADI11 | ADI10 | ADI01); // Select the three channels for conversion
	
	ADCON0 = (ENADC0 | ADCS00 | ENADCI0); // Enable ADC0 and start immediately 
	ADCON1 = (ENADC1 | ADCS10 | ENADCI1); // Enable ADC1 and start immediately
	
	while((ADCI1 & ADCON1 & ADCI0 & ADCON0) == 0); // Wait for first conversion to complete
}

void InitializePin() 
{
	// Set port 0 pins configurations
	P0M1 = 0x3f; // 00111111
	P0M2 = 0xc0; // 11000000

	// Sets port 1 pins configurations
	P1M1 = 0x0C; // 00001100 
	P1M2 = 0x00; // 00000000
	
	// Sets port 2 pins to output
	P2M1 = 0x00; // 00000000
	P2M2 = 0x00; // 00000000
}

void InitializeBatteryVoltage()
{
	batteryVoltage = (unsigned char)(0.414 * AD1DAT2); // only once per lap, read battery voltage times 10
	batteryVoltageDec = batteryVoltage % 10; // store the decimal value
	batteryVoltage /= 10; // store the rest
}

#endif /* _H__PINS__H_ */