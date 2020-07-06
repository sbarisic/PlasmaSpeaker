#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/wdt.h"

void watchdogEnable(bool en)
{
	cli();
	wdt_reset();
	
	if (en)
	{
		wdt_enable(WDTO_30MS);
	}
	else
	{
		wdt_disable();
	}
	
	sei();
}

void watchdogReset()
{
	wdt_reset();
}