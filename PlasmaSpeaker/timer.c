#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/io.h"

volatile timeMs_t time_counter;

ISR(TIMER1_COMPA_vect)
{
	time_counter++;
}

void timer_init()
{
	cli();

	unsigned long timer_frequency = (F_CPU / 1000) / 8;

	// CS12, CS11, CS10
	// 0      1      0    - Divide clock b 8
	TCCR1A = 0;
	TCCR1B = (1 << WGM12) | (1 << CS11);
	OCR1AH = (timer_frequency >> 8) & 0xFF;
	OCR1AL = timer_frequency & 0xFF;
	TIMSK1 |= (1 << OCIE1A);

	sei();
}

timeMs_t timer_ms()
{
	return time_counter;
}

void timer_delay_ms(timeMs_t ms)
{
	timeMs_t startTime = timer_ms();
	
	while ((timer_ms() - startTime) < ms)
	{
	}
}