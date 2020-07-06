#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"

void watchdogEnable(bool en)
{
	cli();
	watchdogReset();
	
	// Enable/disable
	WDTCSR = ((en ? 1 : 0) << WDCE) | (1 << WDE);
	
	WDTCSR = (0 << WDIF) | (0 << WDIE) | (0 << WDCE) | (1 << WDE) | WDTO_2S;

	sei();
}

void watchdogReset()
{
	wdt_reset();
}