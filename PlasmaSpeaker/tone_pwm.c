#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <math.h>

#define OUT_PIN B2
volatile uint16_t timer2_cnt;

/*ISR(TIMER2_OVF_vect)
{
	timer2_cnt++;
}*/

void f_tone_delay(uint16_t tone_delay) {
	_delay_us(20);
	
	for (uint16_t i = 0; i < tone_delay; i++)
		_delay_us(1);
}

void tone_pwm_update(uint16_t tone_del)
{
	uint16_t tone_delay = tone_del / 2;
	
	pinWrite(OUT_PIN, HIGH);
	f_tone_delay(tone_delay);
	pinWrite(OUT_PIN, LOW);
	f_tone_delay(tone_delay);
}

void tone_pwm_init()
{
	pinMode(OUT_PIN, MODE_OUTPUT);
	pinWrite(OUT_PIN, LOW);
	
	/*cli();
	
	TCCR2A = 0;
	TCCR2B = 1;
	TIMSK2 = 1 << TOIE2;
	
	sei();*/
}