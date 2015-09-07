#ifndef UART_H_
#define UART_H_

#include <stdbool.h>
#include "eprintf.h"

#define UART_TYPE       USART1
#define UART_PERIF      RCC_APB2Periph_USART1

#define UART_DMA_IRQ    DMA1_Channel4_IRQn
#define UART_DMA_IT_TC  DMA_IT_TC
#define UART_DMA_CH     DMA1_Channel4

#define UART_GPIO_PERIF RCC_APB2Periph_GPIOA
#define UART_GPIO_PORT  GPIOA
#define UART_GPIO_TX    GPIO_Pin_9
#define UART_GPIO_RX    GPIO_Pin_10

#define UART_NVIC_IRQn	USART1_IRQn

void uartInit(void);
void uartSendData(uint32_t size, uint8_t* data);
int uartPutchar(int ch);
#define uartPrintf(FMT, ...) eprintf(uartPutchar, FMT, ## __VA_ARGS__)
void uartSendDataDma(uint32_t size, uint8_t* data);
void uartIsr(void);
void uartDmaIsr(void);

#endif /* UART_H_ */
