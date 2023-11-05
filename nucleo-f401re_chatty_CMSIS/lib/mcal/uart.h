/* uart.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_UART_H
#define RDF_MCAL_UART_H

#include <stm32f401xe.h>
#include <inttypes.h>
#include <stdbool.h>

static uint8_t data_buffer = 0U;
static bool data_available = false;

void uart_setup(USART_TypeDef *usart, uint64_t baud);
void uart_write(USART_TypeDef *usart, uint8_t *data, uint32_t length);
void uart_write_byte(USART_TypeDef *usart, uint8_t byte);
uint32_t uart_read(USART_TypeDef *usart, uint8_t *data, uint32_t length);
uint8_t uart_read_byte(USART_TypeDef *usart);
bool uart_data_available(USART_TypeDef *usart);

void uart_enable_isr(USART_TypeDef *usart);

void USART2_IRQHandler(void);

#endif // RDF_MCAL_UART_H