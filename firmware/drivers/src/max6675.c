#include "max6675.h"
#include "spi.h"

uint8_t max6675_ms, max6675_ls;

void max6675Init(void)
{
	spiInit();
}

uint8_t max6675ReadReg(uint8_t* ms, uint8_t* ls)
{
	uint8_t tempbuf[2];

	spiReadBuf(tempbuf, 2);

	*ms = tempbuf[1];
	*ls = tempbuf[0];

	return 0;
}

uint8_t max6675IsThermocoupleConnected(void)
{
	if(max6675ReadReg(&max6675_ms, &max6675_ls))
		return 0;

	return max6675_ls >> 2 && 1;
}

uint16_t max6675ReadTemperature(void)
{
	if(max6675ReadReg(&max6675_ms, &max6675_ls))
		return 0;

	return (max6675_ms & 0x7f) << 5 | max6675_ls >> 3;
}

uint8_t max6675GetID(void)
{
	if(max6675ReadReg(&max6675_ms, &max6675_ls))
		return 0;

	return max6675_ls >> 1 && 1;
}
