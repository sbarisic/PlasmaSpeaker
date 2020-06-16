#pragma once

#define F_CPU 16000000UL

#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>

typedef int pinNum_t;
typedef int pinMode_t;
typedef bool logicLevel_t;

#define B0
#define B1
#define B2
#define B3
#define B4
#define B5
#define B6
#define B7

#define C0
#define C1
#define C2
#define C3
#define C4
#define C5
#define C6
#define C7

#define D0
#define D1
#define D2
#define D3
#define D4
#define D5
#define D6
#define D7

void pinMode(pinNum_t pinNum, pinMode_t pinMode);
void pinWrite(pinNum_t pinNum, logicLevel_t level);