#include "PlasmaSpeaker.h"
#include "avr/io.h"

void pwm_init()
{
	pinMode(B2, MODE_OUTPUT);

	//OCR1B = 0;
	//TCCR1A = 0;
	//TCCR1B = 0;
}