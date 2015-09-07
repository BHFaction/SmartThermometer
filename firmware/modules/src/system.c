#define DEBUG_MODULE "SYS"

#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "system.h"



/* Private variable */
static bool isInit;

// This must be the first module to be initialized!
void systemInit(void)
{
  if(isInit)
    return;

  printf("System init\n");

  isInit = true;
}
