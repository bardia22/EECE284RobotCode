/*-------------------------------------------------*/
// EECE 284
// Serial.h
// Jan. 23, 2014
// Author: Bardia
/*-------------------------------------------------*/

#ifndef _H__SERIAL__H_
#define _H__SERIAL__H_

#include <p89lpc9351.h>
#include "PWM.h"

#define BAUD 7200L

void InitializeSerial(void);

void InitializeSerial(void)
{
	BRGCON = 0x00; // Make sure the baud rate generator is off
	BRGR1 = ((XTAL/BAUD)-16) / 0x100;
	BRGR0 = ((XTAL/BAUD)-16) % 0x100;
	BRGCON = 0x03; // Turn-on the baud rate generator
	SCON = 0x52; // Serial port in mode 1, ren, txrdy, rxempty
}

#endif /* _H__SERIAL__H_ */