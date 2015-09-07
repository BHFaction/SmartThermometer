#ifndef __MAX6675_H
#define __MAX6675_H

void max6675Init(void);
uint8_t max6675ReadReg(uint8_t* ms, uint8_t* ls);
uint8_t max6675IsThermocoupleConnected(void);
uint16_t max6675ReadTemperature(void);
uint8_t max6675GetID(void);

#endif
