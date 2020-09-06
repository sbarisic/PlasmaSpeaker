#include "PlasmaSpeaker.h"
#include <avr/io.h>
#include <avr/eeprom.h>

void eeprom_write8(uint8_t offset, uint8_t byte)
{
	eeprom_write_byte((uint8_t*)(uint16_t)offset, byte);
}

uint8_t eeprom_read8(uint8_t offset)
{
	return eeprom_read_byte((uint8_t*)(uint16_t)offset);
}

// Split 16 bit numbers into high and low and read/write them separately
void eeprom_write16(uint8_t offset, uint16_t ushort)
{
	uint8_t hi = (uint8_t)((ushort >> 8) & 0xFF);
	uint8_t lo = (uint8_t)(ushort & 0xFF);
	eeprom_write8(offset, hi);
	eeprom_write8(offset + 1, lo);
}

uint16_t eeprom_read16(uint8_t offset)
{
	uint8_t hi = eeprom_read8(offset);
	uint8_t lo = eeprom_read8(offset + 1);
	return ((uint16_t)hi << 8) | lo;
}

// Read a number at an offset, if there's no value there (0xFF default), write a default
// preset value in it's place and return it
uint8_t eeprom_read8_default(uint8_t offset, uint8_t def)
{
	uint8_t val = eeprom_read8(offset);
	
	if (val == 0xFF)
	{
		eeprom_write8(offset, def);
		return def;
	}
	
	return val;
}

uint16_t eeprom_read16_default(uint8_t offset, uint16_t def)
{
	uint16_t val = eeprom_read16(offset);
	
	if (val == 0xFFFF)
	{
		eeprom_write16(offset, def);
		return def;
	}
	
	return val;
}