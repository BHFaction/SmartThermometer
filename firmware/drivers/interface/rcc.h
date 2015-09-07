#ifndef __RCC_H
#define __RCC_H

void rccInit(void);
void delayInit(uint8_t SYSCLK);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);

#endif
