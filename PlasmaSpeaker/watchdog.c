#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"
#include "avr/io.h"

void watchdogEnable(bool en)
{
	cli();
	watchdogReset();
	
	// Enable/disable
	if (en)
	{
		WDTCSR = (1 << WDCE) | (1 << WDE);
		WDTCSR = (0 << WDIF) | (0 << WDIE) | (0 << WDCE) | (1 << WDE) | WDTO_2S;
	}
	else
	{
		WDTCSR |= (1 << WDCE) | (1 << WDE);
		WDTCSR = 0;
	}

	sei();
}

void watchdogReset()
{
	wdt_reset();
}