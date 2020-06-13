#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= (1 << DDB1);
	
	double Frq = 260;
	double HalfWave = (1000.0 / Frq) / 2.0;
	
	while (1)
	{
		PORTB |= (1 << PORTB1);
		_delay_ms(HalfWave);
		
		PORTB &= ~(1 << PORTB1);
		_delay_ms(HalfWave);
	}
}