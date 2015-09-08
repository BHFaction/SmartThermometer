#include "max6675.h"
#include "spi.h"
// #include "uart.h"

uint8_t max6675_ms, max6675_ls;

void max6675Init(void)
{
	spiInit();
}

uint8_t max6675ReadReg(uint8_t* ms, uint8_t* ls)
{
	uint8_t tempbuf[2];

	spiReadBuf(tempbuf, 2);

	// *ms = tempbuf[0]<<1 | tempbuf[1]>>7;
	// *ls = tempbuf[1]<<1;
	*ms = tempbuf[0];
	*ls = tempbuf[1];

	// uartPrintf("reg => [%04X]", (*ms)<<8 | (*ls) );

	return 0;
}

uint8_t max6675Refresh(void)
{
	if(max6675ReadReg(&max6675_ms, &max6675_ls))
		return 1;

	return 0;
}

uint8_t max6675IsThermocoupleConnected(void)
{
	return max6675_ls >> 2 & 1;
}

float max6675ReadTemperature(void)
{
	return ((max6675_ms & 0x7f) << 5 | max6675_ls >> 3 ) / 4096.0 * 1023.75;
}

uint8_t max6675GetID(void)
{
	return max6675_ls >> 1 & 1;
}
