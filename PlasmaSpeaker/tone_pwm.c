#include "PlasmaSpeaker.h"
#include "avr/interrupt.h"
#include "avr/io.h"
#include <math.h>

#define OUT_PIN D3

void tone_pwm_update(uint8_t tone)
{	
	OCR2B = tone;
}

void tone_pwm_init()
{
	pinMode(OUT_PIN, MODE_OUTPUT);
	pinWrite(OUT_PIN, LOW);
	
	// Fast PWM, 62.5 kHz
	TCCR2A |= BIT(WGM21) | BIT(WGM20);
	TCCR2B &= ~BIT(WGM22);
	
	TCCR2A = (TCCR2A | BIT(COM2B1)) & ~BIT(COM2B0);
	TCCR2A &= ~(BIT(COM2A1) | BIT(COM2A0));
	
	TCCR2B = (TCCR2B & ~(BIT(CS12) | BIT(CS11))) | BIT(CS10);
}