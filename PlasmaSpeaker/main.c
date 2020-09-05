#include "PlasmaSpeaker.h"

#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "sounddata.h"

char input[256];

#define tune_buffer_max_len 1024
uint8_t tune_buffer[tune_buffer_max_len];
uint16_t head;
uint16_t tail;

uint16_t tune_idx = 0;
uint16_t recv_idx = 0;

int buffer_push(uint8_t data)
{
	int next;

	next = head + 1;  // next is where head will point to after this write.
	if (next >= tune_buffer_max_len)
	{
		next = 0;
	}

	if (next == tail)  // if the head + 1 == tail, circular buffer is full
	{
		return -1;
	}

	tune_buffer[head] = data;  // Load data and then move
	head = next;             // head to next data offset.
	return 0;  // return success to indicate successful push.
}

int buffer_pop(uint8_t *data)
{
	int next;

	if (head == tail)  // if the head == tail, we don't have any data
	{
		return -1;
	}

	next = tail + 1;  // next is where tail will point to after this read.
	if(next >= tune_buffer_max_len)
	{
		next = 0;
	}

	*data = tune_buffer[tail];  // Read data and then move
	tail = next;              // tail to next offset.
	return 0;  // return success to indicate successful push.
}

int main(void)
{
	pinNum_t in_button = B1;
	pinNum_t status_led = B2;

	logicLevel_t status_led_on = HIGH;
	logicLevel_t in_button_last = HIGH;
	
	tone_pwm_init();

	pinMode(in_button, MODE_INPUT_PULLUP);
	pinMode(status_led, MODE_OUTPUT);
	pinWrite(status_led, status_led_on);

	uart_init();
	uart_set_binary_mode(true);

	timer_init();
	timeMs_t lastFlashTime = timer_ms();
	timeMs_t flashInterval = 200; // 200
	int quickflashCount = 0;
	
	while (1)
	{
		uart_write("Hello World!\n", 13);
		_delay_ms(500);
	}
		
	while (1)
	{
		//watchdogReset();
		
		uint8_t cur_play_tone = 0;
		if (buffer_pop(&cur_play_tone) == 0)
		{
			tone_pwm_update(cur_play_tone);
			_delay_us(125);
		}
		else
		{
			uart_write_16(2048);
		}
		
		/*tone_pwm_update(tune_buffer[tune_idx++]);
		_delay_us(125);
		if (tune_idx >= tune_buffer_max_len)
		{
			tune_idx = 0;
		}*/

		logicLevel_t in_button_cur = pinRead(in_button);
		if (in_button_cur != in_button_last)
		{
			in_button_last = in_button_cur;
			if (in_button_cur == HIGH)
			{
				quickflashCount = 4;
				/*uart_write("READY\n", 6);
				uart_set_binary_mode(true);*/
				
				uart_write_16(1024);
			}
		}
		
		uint8_t rec_play_tone;
		while(uart_read_byte(&rec_play_tone))
		{
			buffer_push(rec_play_tone);
			/*tune_buffer[recv_idx++] = rec_play_tone;
			
			if (recv_idx >= tune_buffer_max_len)
			{
				recv_idx = 0;
			}*/
		}
		
		timeMs_t curTime = timer_ms();
		timeMs_t curFlashInterval = flashInterval;
		
		if (quickflashCount > 0)
		{
			curFlashInterval = curFlashInterval / 2;
		}
		
		if (curTime - lastFlashTime > curFlashInterval)
		{
			lastFlashTime = curTime;
			status_led_on = !status_led_on;
			quickflashCount--;
			pinWrite(status_led, status_led_on);
		}
	}
}