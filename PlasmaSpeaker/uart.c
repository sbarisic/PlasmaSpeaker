#include "PlasmaSpeaker.h"
#define UART_BAUD 9600
#define IN_BUFFER_LEN 512

uint8_t in_buffer[IN_BUFFER_LEN];
int in_buffer_idx;

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
	
	// Enable RX and TX
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
	
	// Char size to 8 bit
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
	
	int baudPrescale = ((F_CPU / (UART_BAUD * 16UL))) - 1;
	UBRR0H = (baudPrescale >> 8);
	UBRR0L = baudPrescale;
}

bool uart_available()
{
	while ((UCSR0A & (1 << RXC0)) != 0)
	{
		uint8_t inByte = UDR0;
		in_buffer[in_buffer_idx++] = inByte;
		
		if (inByte == 0)
		{
			return true;
		}
	}
	
	return false;
}

int uart_read(uint8_t* input)
{
	for (int i = 0; i < IN_BUFFER_LEN; i++)
	{
		if ((input[i] = in_buffer[i]) == 0)
		{
			clear_input();
			return i;
		}
	}
	
	clear_input();
	return IN_BUFFER_LEN;
}

void uart_write(uint8_t* output, int length)
{
	for (int i = 0; i < length + 1; i++)
	{
		while ((UCSR0A & (1 << UDRE0)) == 0)
		{
		};
		
		if (i >= length)
		{
			UDR0 = 0;
		}
		else
		{
			UDR0 = output[i];
		}
	}
}