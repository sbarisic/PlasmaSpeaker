#include "PlasmaSpeaker.h"

int main(void)
{
	pinNum_t in_button = B1;
	pinNum_t status_led = D3;
	logicLevel_t status_led_on = HIGH;

	pinMode(in_button, MODE_INPUT_PULLUP);
	pinMode(status_led, MODE_OUTPUT);
	pinWrite(status_led, status_led_on);	

	uart_init();
	uint8_t input[512];

	timer_init();
	timeMs_t lastFlashTime = timer_ms();
	timeMs_t flashInterval = 100;

	timer_delay_ms(2500);
	watchdogEnable(true);

	while (1)
{

}

	while (1)
	{
		//watchdogReset();

		if (uart_available())
		{
			uart_read(input);
			// TODO: Handle UART input
		}
		
		timeMs_t curTime = timer_ms();
		if (curTime - lastFlashTime > flashInterval)
		{
			lastFlashTime = curTime;
			status_led_on = !status_led_on;
			pinWrite(status_led, status_led_on);
		}
		
		//pinWrite(OUTPUT_PIN, read_input);
		
		//watchdogReset();
		
		//pinWrite(B1, HIGH);
		//_delay_ms(HalfWave);
		
		//pinWrite(B1, LOW);
		//_delay_ms(HalfWave);
	}
}