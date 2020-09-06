#include "PlasmaSpeaker.h"

#include <string.h>
#include <stdio.h>

#define PLAY_MUSIC

// 1024 byte circular buffer
// If head == tail, buffer is empty
#define tune_buffer_max_len 1024
uint8_t tune_buffer[tune_buffer_max_len];

uint16_t head; // First free data index, put new data at this index and increase
uint16_t tail; // First available data index, fetch data from this index and increase
uint8_t tone;
volatile bool tune_buffer_empty;

int16_t buffer_push(uint8_t data)
{
	int16_t next = head + 1;
	if (next >= tune_buffer_max_len)
	{
		next = 0;
	}

	// Circular buffer full
	if (next == tail)
	{
		return -1;
	}

	tune_buffer[head] = data;
	head = next;
	
	// Success
	return 0;
}

int16_t buffer_pop(uint8_t *data)
{
	// Circular buffer empty
	if (head == tail)
	{
		return -1;
	}

	int16_t next = tail + 1;
	if(next >= tune_buffer_max_len)
	{
		next = 0;
	}

	*data = tune_buffer[tail];
	tail = next;
	
	// Success
	return 0;
}

void callback_8000hz()
{
#ifdef PLAY_MUSIC

	if (buffer_pop(&tone) == 0)
	{
		tone_pwm_update(tone);
	}
	else
	{
		tune_buffer_empty = true;
	}
	
#endif
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

	timer_init();
	timeMs_t lastFlashTime = timer_ms();

	uint8_t quickflashCount = 0;

	// TODO: Read from EEPROM
	uint8_t quickFlashCountNum = eeprom_read8_default(0, 10);
	timeMs_t quickFlashDivider = (timeMs_t)eeprom_read8_default(1, 4);
	timeMs_t flashInterval = (timeMs_t)eeprom_read16_default(2, 200); // flash LED every 200 ms
	
	uart_init();
	uart_write_str("DISCARD_LINE\n");
	uart_write_str("Hello World!\n");
	
	// Print EEPROM variables while still in text mode
	uart_write_str("quickFlashCountNum = ");
	uart_write_uint8(quickFlashCountNum);
	uart_write_str("\nquickFlashDivider = ");
	uart_write_uint8(quickFlashDivider);
	uart_write_str("\nflashInterval = ");
	uart_write_uint16(flashInterval);
	uart_write_str("\n\n");
	
#ifdef PLAY_MUSIC
	volatile bool requested_data = false;
	uart_write_str("BINARY\n");
	uart_set_binary_mode(true);
#endif

	while (1)
	{
		//watchdogReset();
		
#ifdef PLAY_MUSIC
		// Buffer empty, request more data
		if (tune_buffer_empty)
		{
			if (!requested_data)
			{
				uart_write_16(tune_buffer_max_len);
				requested_data = true;
			}
		}
		
		// Receive sound data from PC via UART and fill circular buffer
		uint8_t rec_play_tone;
		while (uart_read_byte(&rec_play_tone))
		{
			if (!buffer_push(rec_play_tone))
			{
				tune_buffer_empty = false;
				requested_data = false;
			}
			else
			{
				break;
			}
		}
#endif
		
		// Sanity test button
		// Make the LED toggle faster for a small period
		logicLevel_t in_button_cur = pinRead(in_button);
		if (in_button_cur != in_button_last)
		{
			in_button_last = in_button_cur;
			if (in_button_cur == HIGH)
			{
#ifndef PLAY_MUSIC
				uart_write_str("HIGH\n");
#endif

				quickflashCount = quickFlashCountNum;
			}
			else
			{
#ifndef PLAY_MUSIC
				uart_write_str("LOW\n");
#endif
			}
		}
		
		// LED flashing
		timeMs_t curTime = timer_ms();
		timeMs_t curFlashInterval = flashInterval;
		
		// If button was pressed and quickflashCount was increased
		// make the LED blink quickFlashDivider-times faster
		if (quickflashCount > 0)
		{
			curFlashInterval = curFlashInterval / quickFlashDivider;
		}
		
		if ((curTime - lastFlashTime) > curFlashInterval)
		{
			lastFlashTime = curTime;
			status_led_on = !status_led_on;
			
			if (quickflashCount > 0)
			{
				quickflashCount--;
			}
			else
			{
				quickflashCount = 0;
			}
			
			pinWrite(status_led, status_led_on);
		}
	}
}