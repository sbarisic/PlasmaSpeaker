#include "PlasmaSpeaker.h"
#include <string.h>
#include <stdio.h>

// 110'000 baud - 13.75 kilobytes per second
// We need at least 8 kilobytes per second - 64000 baud
// Music is sampled at 8000 Hz, each sample is 1 byte in size in PCM format
// PCM - https://en.wikipedia.org/wiki/Pulse-code_modulation
#define BAUD 110000
#include <util/setbaud.h>

#define IN_BUFFER_LEN 512

bool uart_binary_mode;
char in_buffer[IN_BUFFER_LEN];
int16_t in_buffer_idx;

void clear_input()
{
	in_buffer_idx = 0;
	
	for (int i = 0; i < IN_BUFFER_LEN; i++)
	{
		in_buffer[i] = 0;
	}
}

void uart_init()
{
	clear_input();
	uart_set_binary_mode(false);
	
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
#if USE_2X
	UCSR0A |= BIT(U2X0);
#else
	UCSR0A &= ~BIT(U2X0);
#endif

	// Enable RX and TX
	UCSR0B |= BIT(RXEN0) | BIT(TXEN0);
}

bool uart_read_byte(uint8_t* out_byte)
{
	while (!(UCSR0A & BIT(RXC0)))
	{
		return false;
	}
	
	*out_byte = UDR0;
	return true;
}

bool uart_line_available()
{
	uint8_t inByte;
	if (uart_read_byte(&inByte)) {
		in_buffer[in_buffer_idx] = (char)inByte;
		in_buffer_idx++;
		
		if (inByte == 10)
		{
			return true;
		}
	}
	
	return false;
}

uint16_t uart_read(char* input)
{
	uint16_t ret_len = in_buffer_idx;
	
	for (uint16_t i = 0; i < ret_len; i++)
	{
		input[i] = in_buffer[i];
	}
	
	input[ret_len] = 0;
	clear_input();
	return ret_len;

}

void uart_write_8(uint8_t byte)
{
	while ((UCSR0A & BIT(UDRE0)) == 0)
	{
	};
		
	UDR0 = byte;
}

void uart_write_16(uint16_t ushort)
{
	uart_write_8(ushort & 0xFF);
	uart_write_8((ushort >> 8) & 0xFF);
}

void uart_write(const char* output, uint16_t length)
{
	for (int i = 0; i < length; i++)
	{
		uart_write_8(output[i]);
	}
}

void uart_write_uint8(uint8_t num)
{
	char buf[5];
	int len = sprintf(buf, "%u", num);
	uart_write(buf, len);
}

void uart_write_uint16(uint16_t num)
{
	char buf[10];
	int len = sprintf(buf, "%u", num);
	uart_write(buf, len);
}

void uart_write_str(const char* str)
{
	uint16_t len = strlen(str);
	uart_write(str, len);
}

void uart_set_binary_mode(bool binary_mode)
{
	uart_binary_mode = binary_mode;
}

bool uart_is_binary()
{
	return uart_binary_mode;
}