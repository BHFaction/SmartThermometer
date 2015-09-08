#include "config.h"
#include "system.h"

#include "stm32f10x.h"

#include "uart.h"

#include "max6675.h"
#include "rcc.h"

int main()
{
	systemInit();
	delayInit(72);

	max6675Init();

	while(1)
	{
		max6675Refresh();
		uartPrintf("thermocouple, temperature => [%d, %.2f]\n", max6675IsThermocoupleConnected(), max6675ReadTemperature());
		delay_ms(500);
	}

	while(1);

	return 0;
}

