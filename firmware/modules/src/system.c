#define DEBUG_MODULE "SYS"

#include <stdbool.h>
// #include <stdio.h>
#include "config.h"
#include "system.h"

#include "eprintf.h"
#include "uart.h"
#include "rcc.h"

/* Private variable */
static uint8_t isInit;

// This must be the first module to be initialized!
void systemInit(void)
{
	if(isInit)
		return;

	rccInit();

	uartInit();

	uartPrintf("System init\n");

	isInit = 1;
}
