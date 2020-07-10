#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/io.h"

#define TIMER2_FREQ 10
#define OUT_PIN B2

volatile bool timer2_triggered;

ISR(TIMER2_OVF_vect)
{
	timer2_triggered = true;
	TCNT2 = TIMER2_FREQ;
	
	pinWrite(OUT_PIN, HIGH);
	pinWrite(OUT_PIN, LOW);
}

void tone_pwm_update()
{
	if (timer2_triggered)
	{
		timer2_triggered = false;
	}
	else
	{
		return;
	}
	

}

void tone_pwm_init()
{
	pinMode(OUT_PIN, MODE_OUTPUT);
	pinWrite(OUT_PIN, LOW);
	
	cli();
	timer2_triggered = false;

	TCCR2A = 0;
	TCCR2B = 0b00000011;
	TIMSK2 |= 1 << TOIE2;
	TCNT2 = TIMER2_FREQ;

	/*
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	OCR1AH = (timer_frequency >> 8) & 0xFF;
	OCR1AL = timer_frequency & 0xFF;
	TIMSK1 |= 1 << OCIE1A;
	*/


	sei();
}