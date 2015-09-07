#ifndef __SPI_H
#define __SPI_H

#define SPI_TYPE       	SPI2
#define SPI_RCC       	RCC_APB1Periph_SPI2

#define SPI_GPIO_PORT	GPIOB
#define SPI_GPIO_RCC    RCC_APB2Periph_GPIOB
#define SPI_GPIO_CE		GPIO_Pin_12
#define SPI_GPIO_SCK	GPIO_Pin_13
#define SPI_GPIO_MISO	GPIO_Pin_14
#define SPI_GPIO_MOSI	GPIO_Pin_15

// #define SPI_DMA_IRQ    	DMA1_Channel4_IRQn
// #define SPI_DMA_IT_TC  	DMA_IT_TC
// #define SPI_DMA_CH     	DMA1_Channel4

void spiInit(void);
uint8_t spiWriteReadByte(uint8_t dat);
uint8_t spiReadBuf(uint8_t *_pBuf, uint8_t _ucLen);

#endif