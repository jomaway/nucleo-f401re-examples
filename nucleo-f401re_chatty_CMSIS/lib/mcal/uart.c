#include "uart.h"
#include <gpio.h>

void uart_setup(USART_TypeDef *usart, uint64_t baud)
{
    // Enable clock for port a
    gpio_enable_port_clock(GPIOA);
    uart_init_pins(usart);
    uart_enable_clock(usart);
    uart_set_baudrate(usart, baud);
    uart_enable(usart); // Enable Usart TX & RX
    uart_enable_isr(usart);
}

void uart_init_pins(USART_TypeDef *usart)
{
    if (USART1 == usart)
    {
        // TX = PA9 , RX = PA10;  AF-NUM = 7 (See Section AF mapping)
        gpio_set_mode(GPIOA, 9, ALTFUNC);
        gpio_set_mode(GPIOA, 10, ALTFUNC);
        gpio_set_af(GPIOA, 9, AF7);
        gpio_set_af(GPIOA, 10, AF7);
    }
    else if (USART2 == usart)
    {
        // TX = PA3 , RX = PA2;  AF-NUM = 7 (See Section AF mapping)
        gpio_set_mode(GPIOA, 2, ALTFUNC);
        gpio_set_af(GPIOA, 2, AF7);
        gpio_set_mode(GPIOA, 3, ALTFUNC);
        gpio_set_af(GPIOA, 3, AF7);
    }
    else if (USART6 == usart)
    {
        // TX = PA11 ; RX = PA12; AF-NUM = 8  (See Section AF mapping)
        gpio_set_mode(GPIOA, 11, ALTFUNC);
        gpio_set_af(GPIOA, 11, AF8);
        gpio_set_mode(GPIOA, 12, ALTFUNC);
        gpio_set_af(GPIOA, 12, AF8);
    }
}

void uart_enable_clock(USART_TypeDef *usart)
{
    if (USART1 == usart)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN; 
    }
    else if (USART2 == usart)
    {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }
    else if (USART6 == usart)
    {
        RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
    }
}

void uart_set_baudrate(USART_TypeDef *usart, uint64_t baud)
{
    uint8_t over8_rate = 8 * ( 2 - ( usart->CR1 & USART_CR1_OVER8 ) );
    SystemCoreClockUpdate();  // Update SystemCoreClock
    uint32_t uartdiv = SystemCoreClock / baud;
    uint16_t mantissa = uartdiv / over8_rate;
    uint8_t fraction = uartdiv % over8_rate;
    usart->BRR = ( (mantissa << USART_BRR_DIV_Mantissa_Pos) | fraction ) ;  // For the moment overbit is always 0.
}

void uart_enable(USART_TypeDef *usart)
{
    // Enable Usart TX & RX
    usart->CR1 |= USART_CR1_UE | USART_CR1_RE | USART_CR1_TE;
}

void uart_enable_isr(USART_TypeDef *usart)
{
    usart->CR1 |= USART_CR1_RXNEIE;
    /* Enable and set the EXTI interrupt in NVIC */
    if (USART2 == usart)
    {
        NVIC_EnableIRQ(USART2_IRQn);
    }
    else if (USART1 == usart)
    {
        NVIC_EnableIRQ(USART1_IRQn);
    }
    else if (USART6 == usart)
    {   
        NVIC_EnableIRQ(USART6_IRQn);
    }
}


void uart_write(USART_TypeDef *usart, uint8_t *data, uint32_t length)
{
    while(length-- > 0)
    {
        uart_write_byte(usart, *(data++));
    }
}

void uart_write_byte(USART_TypeDef *usart, uint8_t byte)
{
    while (!(usart->SR & USART_SR_TXE));
    usart->DR = byte & 0x1FF;
}

uint32_t uart_read(USART_TypeDef *usart, uint8_t *data, uint32_t length)
{
    if (length > 0 && data_available)
    {
        *data = data_buffer;
        data_available = false;
        return 1;
    }
    return 0;
}

uint8_t uart_read_byte(USART_TypeDef *usart)
{
    return (uint8_t) (usart->DR & 0x1FF);
}

bool uart_data_available(USART_TypeDef *usart)
{
    return usart->SR & USART_SR_RXNE; 
}

/**
* USART2: IRQ-Handler
*/
void USART2_IRQHandler(void)
{
    if (uart_data_available(USART2))
    {
        data_buffer = uart_read_byte(USART2);
        data_available = true;
    }
}


