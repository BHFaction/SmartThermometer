#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "config.h"
#include "rcc.h"

#define SysTick_CTRL_ENABLE_Pos             0 
#define SysTick_CTRL_ENABLE_Msk            (1UL << SysTick_CTRL_ENABLE_Pos) 

static uint8_t  fac_us;
static uint16_t fac_ms;

void rccInit(void)
{
  ErrorStatus HSEStartUpStatus;

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  while(HSEStartUpStatus != SUCCESS)
  {
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
  }

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    if(HSE_VALUE == ((uint32_t)8000000))
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
    } else {
        RCC_HCLKConfig(RCC_SYSCLK_Div2);
    }
    
    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    //RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08) {}
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

}

void delayInit(uint8_t SYSCLK)
{
  SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
  fac_us = SYSCLK/8;
  fac_ms = (u16)fac_us*1000;
}

void delay_us(uint32_t nus)
{
  u32 temp;
  SysTick->LOAD=nus*fac_us; //时间加载
  SysTick->VAL=0x00;        //清空计数器
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
  SysTick->VAL =0X00;       //清空计数器
}

void delay_ms(uint16_t nms)
{
  u32 temp;
  SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
  SysTick->VAL =0x00;           //清空计数器
  SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数
  do
  {
    temp=SysTick->CTRL;
  }
  while(temp&0x01&&!(temp&(1<<16)));//等待时间到达
  SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
  SysTick->VAL =0X00;       //清空计数器
}


