#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "spi.h"

#define NOP         	0xFF  // Define No Operation, might be used to read status register

#define SPI_CE_1()		GPIO_SetBits(SPI_GPIO_PORT, SPI_GPIO_CE)
#define SPI_CE_0()		GPIO_ResetBits(SPI_GPIO_PORT, SPI_GPIO_CE)


/*
void nRF24L01_DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;


	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


	DMA_DeInit(DMA1_Channel4);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI2_DR_Addr;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI2_Rx_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);


	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI2_DR_Addr;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI2_Tx_Buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = SPI_BUFFER_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);


	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Rx, ENABLE);

	SPI_I2S_DMACmd(SPI2, SPI_I2S_DMAReq_Tx, ENABLE);

}*/


void spiInit(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB1PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB2PeriphClockCmd(SPI_GPIO_RCC | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitStructure.GPIO_Pin = SPI_GPIO_CE;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = SPI_GPIO_SCK | SPI_GPIO_MOSI;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = SPI_GPIO_MISO;
	GPIO_Init(SPI_GPIO_PORT, &GPIO_InitStructure);


	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;

	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;

	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI_TYPE, &SPI_InitStructure);

	SPI_Cmd(SPI_TYPE, ENABLE);				/* 使能SPI  */

	SPI_CE_1();
}

uint8_t spiWriteReadByte(uint8_t dat)
{
	/* 当SPI发送缓冲器非空时等待 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* 通过SPI发送一字节数据 */
	SPI_I2S_SendData(SPI2, dat);

	/* 当SPI接收缓冲器为空时等待 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

	/* 通过SPI接收一字节数据 */
	return SPI_I2S_ReceiveData(SPI2);
}

uint8_t spiReadBuf(uint8_t *_pBuf, uint8_t _ucLen)
{
 	uint8_t i;

	SPI_CE_0();

	for(i = 0; i < _ucLen; i++)
	{
		_pBuf[i] = spiWriteReadByte(NOP);
	}

	SPI_CE_1();

 	return 0;
}
