#include "PlasmaSpeaker.h"

int main(void)
{
	DDRB |= (1 << DDB1);
	
	double Frq = 666;
	double HalfWave = (1000.0 / Frq) / 2.0;
	
	while (1)
	{
		PORTB |= (1 << PORTB1);
		_delay_ms(HalfWave);
		
		PORTB &= ~(1 << PORTB1);
		_delay_ms(HalfWave);
	}
}