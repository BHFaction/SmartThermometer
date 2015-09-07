#ifndef UART_H_
#define UART_H_

#include <stdbool.h>

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


/**
 * Initialize the UART.
 *
 * @note Initialize CRTP link only if USE_CRTP_UART is defined
 */
void uartInit(void);

/**
 * Test the UART status.
 *
 * @return true if the UART is initialized
 */
bool uartTest(void);

/**
 * Get CRTP link data structure
 *
 * @return Address of the crtp link operations structure.
 */
struct crtpLinkOperations * uartGetLink();

/**
 * Sends raw data using a lock. Should be used from
 * exception functions and for debugging when a lot of data
 * should be transfered.
 * @param[in] size  Number of bytes to send
 * @param[in] data  Pointer to data
 *
 * @note If UART Crtp link is activated this function does nothing
 */
void uartSendData(uint32_t size, uint8_t* data);

/**
 * Send a single character to the serial port using the uartSendData function.
 * @param[in] ch Character to print. Only the 8 LSB are used.
 * @return Character printed
 *
 * @note If UART Crtp link is activated this function does nothing
 */
int uartPutchar(int ch);

/**
 * Uart printf macro that uses eprintf
 * @param[in] FMT String format
 * @param[in] ... Parameters to print
 *
 * @note If UART Crtp link is activated this function does nothing
 */
#define uartPrintf(FMT, ...) eprintf(uartPutchar, FMT, ## __VA_ARGS__)

/**
 * Sends raw data using DMA transfer. Should be used from
 * exception functions and for debugging when a lot of data
 * should be transfered.
 * @param[in] size  Number of bytes to send
 * @param[in] data  Pointer to data
 *
 * @note If UART Crtp link is activated this function does nothing
 */
void uartSendDataDma(uint32_t size, uint8_t* data);

/**
 * Interrupt service routine handling UART interrupts.
 */
void uartIsr(void);

/**
 * Interrupt service routine handling UART DMA interrupts.
 */
void uartDmaIsr(void);

#endif /* UART_H_ */
