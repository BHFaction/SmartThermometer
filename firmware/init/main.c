#include "config.h"
#include "system.h"

#include "stm32f10x.h"

#include "uart.h"

#include "max6675.h"
#include "rcc.h"

int main()
{
	systemInit();

	max6675Init();

	while(1)
	{
		uartPrintf("thermocouple, temperature => [%d, %d]\n", max6675IsThermocoupleConnected(), max6675ReadTemperature());
		delay_ms(100);
	}

	while(1);

	return 0;
}

