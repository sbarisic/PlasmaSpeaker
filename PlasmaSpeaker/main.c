#include "PlasmaSpeaker.h"

#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include "sounddata.h"

//char input[256];

#define tune_buffer_max_len 1024
#define tune_buffer_min_req_cnt 680

uint8_t tune_buffer[tune_buffer_max_len];
uint16_t head;
uint16_t tail;

int16_t buffer_push(uint8_t data)
{
	int16_t next = head + 1;  // next is where head will point to after this write.
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
	
	// Success
	return 0;
}

int16_t buffer_pop(uint8_t *data)
{
	// No data in buffer
	if (head == tail)
	{
		return -1;
	}

	int16_t next = tail + 1;
	if(next >= tune_buffer_max_len)
	{
		next = 0;
	}

	*data = tune_buffer[tail]; // Read data and then move
	tail = next; // Tail to next offset.
	
	// Success
	return 0;
}

uint16_t buffer_get_free_count() {
	uint16_t free = 0;
	uint16_t next = head + 1;

	while (next != tail) {
		next++;
		free++;

		if (next >= tune_buffer_max_len) {
			next = 0;
		}
	}

	return free;
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
	
	/*while (1)
	{
		status_led_on = !status_led_on;
		pinWrite(status_led, status_led_on);
		
		uart_write("Hello\n", 6);
		_delay_ms(500);
	}*/
	
	while (1)
	{
		//watchdogReset();
		//_delay_us(125);
		
		// Fetch current tone and play it
		uint8_t cur_play_tone = 0;
		if (buffer_pop(&cur_play_tone) == 0)
		{
			tone_pwm_update(cur_play_tone);
			//_delay_us(125);
			_delay_us(250);
		} else {
			uart_write_16(tune_buffer_max_len - 10);
		}
		
		/*// If there's free space in the buffer, request more data
		uint16_t free = buffer_get_free_count();
		if (free >= tune_buffer_min_req_cnt)
		{
			uart_write_16(free);
		}*/
		
		// Receive sound data from PC via UART
		uint8_t rec_play_tone;
		while (uart_read_byte(&rec_play_tone))
		{
			if (!buffer_push(rec_play_tone))
			{
				break;
			}
		}
		
		// Sanity test button
		logicLevel_t in_button_cur = pinRead(in_button);
		if (in_button_cur != in_button_last)
		{
			in_button_last = in_button_cur;
			if (in_button_cur == HIGH)
			{
				quickflashCount = 4;
			}
		}
		
		// LED flashing
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