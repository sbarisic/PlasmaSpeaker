#pragma once

#define F_CPU 16000000UL

#include <stddef.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

typedef int pinNum_t;
typedef int pinMode_t;
typedef uint8_t logicLevel_t;
typedef unsigned long timeMs_t;

#define B0 0
#define B1 1
#define B2 2
#define B3 3
#define B4 4
#define B5 5
#define B6 6
#define B7 7

#define C0 8
#define C1 9
#define C2 10
#define C3 11
#define C4 12
#define C5 13
#define C6 14
#define C7 15

#define D0 16
#define D1 17
#define D2 18
#define D3 19
#define D4 20
#define D5 21
#define D6 22
#define D7 23

#define MODE_INPUT 0
#define MODE_INPUT_PULLUP 1
#define MODE_OUTPUT 2

#define HIGH 1
#define LOW 0

// Pin IO functions
volatile uint8_t* pinDDR(pinNum_t pinNum);
volatile uint8_t* pinPort(pinNum_t pinNum);
void pinMode(pinNum_t pinNum, pinMode_t pinMode);
void pinWrite(pinNum_t pinNum, logicLevel_t level);
uint8_t pinRead(pinNum_t pinNum);

// Watchdog functions
void watchdogEnable(bool en);
void watchdogReset();

// UART functions
void uart_init();
bool uart_available();
int uart_read(char* input);
void uart_write(const char* output, int length);

// Timer functions
void timer_init();
timeMs_t timer_ms();
void timer_delay_ms(timeMs_t ms);

// PWM functions
void pwm_init();