#include "PlasmaSpeaker.h"

int main(void)
{
	pinNum_t in_button = B1;
	pinNum_t status_led = D3;

	logicLevel_t status_led_on = HIGH;
	logicLevel_t in_button_last = HIGH;
	
	tone_pwm_init();

	pinMode(in_button, MODE_INPUT_PULLUP);
	pinMode(status_led, MODE_OUTPUT);
	pinWrite(status_led, status_led_on);

	uart_init();
	char input[512];

	timer_init();
	timeMs_t lastFlashTime = timer_ms();
	timeMs_t flashInterval = 200;

	//watchdogEnable(true);

	while (1)
	{
		//watchdogReset();
		
		tone_pwm_update();

		logicLevel_t in_button_cur = pinRead(in_button);
		if (in_button_cur != in_button_last)
		{
			in_button_last = in_button_cur;
			if (in_button_cur == HIGH)
			{
				uart_write("HIGH\n", 5);
			}
			else
			{
				uart_write("LOW\n", 4);
			}
		}

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