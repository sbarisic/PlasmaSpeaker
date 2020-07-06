#include "PlasmaSpeaker.h"

pinNum_t translatePinNum(pinNum_t pinNum)
{
	if (pinNum >= 16)
	{
		return pinNum - 16;
	}
	
	if (pinNum >= 8)
	{
		return pinNum - 8;
	}
	
	return pinNum;
}

volatile uint8_t* pinDDR(pinNum_t pinNum)
{
	if (pinNum >= B0 && pinNum <= B7)
	{
		return &DDRB;
	}
	else if (pinNum >= C0 && pinNum <= C7)
	{
		return &DDRC;
	}
	else if (pinNum >= D0 && pinNum <= D7)
	{
		return &DDRD;
	}
	else
	{
		return NULL;
	}
}

volatile uint8_t* pinPort(pinNum_t pinNum)
{
	if (pinNum >= B0 && pinNum <= B7)
	{
		return &PORTB;
	}
	else if (pinNum >= C0 && pinNum <= C7)
	{
		return &PORTC;
	}
	else if (pinNum >= D0 && pinNum <= D7)
	{
		return &PORTD;
	}
	else
	{
		return NULL;
	}
}

volatile uint8_t* pinPin(pinNum_t pinNum)
{
	if (pinNum >= B0 && pinNum <= B7)
	{
		return &PINB;
	}
	else if (pinNum >= C0 && pinNum <= C7)
	{
		return &PINC;
	}
	else if (pinNum >= D0 && pinNum <= D7)
	{
		return &PIND;
	}
	else
	{
		return NULL;
	}
}

void pinMode(pinNum_t pinNum, pinMode_t pinMode)
{
	pinNum_t pinNumTrans = translatePinNum(pinNum);
	volatile uint8_t* reg_ddr = pinDDR(pinNum);
	volatile uint8_t* reg_port = pinPort(pinNum);
	
	if (pinMode == MODE_INPUT)
	{
		*reg_ddr &= ~(1 << pinNumTrans);
		*reg_port &= ~(1 << pinNumTrans);
	}
	else if (pinMode == MODE_INPUT_PULLUP)
	{
		*reg_ddr &= ~(1 << pinNumTrans);
		*reg_port |= (1 << pinNumTrans);
	}
	else if (pinMode == MODE_OUTPUT)
	{
		*reg_ddr |= (1 << pinNumTrans);
	}
}

void pinWrite(pinNum_t pinNum, logicLevel_t level)
{
	pinNum_t pinNumTrans = translatePinNum(pinNum);
	volatile uint8_t* reg_port = pinPort(pinNum);
	
	if (level == HIGH)
	{
		*reg_port &= ~(1 << pinNumTrans);
	}
	else
	{
		*reg_port |= (1 << pinNumTrans);
	}
}

logicLevel_t pinRead(pinNum_t pinNum)
{
	pinNum_t pinNumTrans = translatePinNum(pinNum);
	volatile uint8_t* reg_pin = pinPin(pinNum);
	
	if ((~*reg_pin & (1 << pinNumTrans)) != 0)
	{
		return LOW;
	}
	else
	{
		return HIGH;
	}
}