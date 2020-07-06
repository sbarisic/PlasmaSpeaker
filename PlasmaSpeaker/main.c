#include "PlasmaSpeaker.h"

int main(void)
{
	pinNum_t INPUT_PIN = B3;
	pinNum_t OUTPUT_PIN = B1;
	
	pinMode(OUTPUT_PIN, MODE_OUTPUT);
	pinMode(INPUT_PIN, MODE_INPUT_PULLUP);
	
	uart_init();
	uint8_t input[512];
	
	logicLevel_t lastRead = LOW;
	
	while (1)
	{
		logicLevel_t read_input = pinRead(INPUT_PIN);
		
		if (uart_available())
		{
			uart_read(input);
		}
		
		if (read_input != lastRead)
		{
			lastRead = read_input;
			
			if (read_input == HIGH)
			{
				uart_write((uint8_t*)"HIGH\n", 5);
			}
			else
			{
				uart_write((uint8_t*)"LOW\n", 4);
			}
		}
		
		pinWrite(OUTPUT_PIN, read_input);
		
		//watchdogReset();
		
		//pinWrite(B1, HIGH);
		//_delay_ms(HalfWave);
		
		//pinWrite(B1, LOW);
		//_delay_ms(HalfWave);
	}
}