#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/io.h"

volatile uint16_t time_counter;
volatile timeMs_t time_counter_ms;

void callback_8000hz();

ISR(TIMER1_COMPA_vect)
{
	time_counter++;
	
	if (time_counter >= 8) 
	{
		time_counter = 0;
		time_counter_ms++;
	}
	
	callback_8000hz();
}

void timer_init()
{
	cli();

	// Set clear timer on compare match mode
	TCCR1B = (TCCR1B & ~BIT(WGM13)) | BIT(WGM12);
	TCCR1A = TCCR1A & ~(BIT(WGM11) | BIT(WGM10));
	
	// No prescaler
	TCCR1B = (TCCR1B & ~(BIT(CS12) | BIT(CS11))) | BIT(CS10);
	
	// Trigger frequency 8000 Hz
	OCR1A = F_CPU / 8000; 
	TIMSK1 |= BIT(OCIE1A);

	sei();
}

timeMs_t timer_ms()
{
	return time_counter_ms;
}

void timer_delay_ms(timeMs_t ms)
{
	timeMs_t startTime = timer_ms();
	
	while ((timer_ms() - startTime) < ms)
	{
	}
}