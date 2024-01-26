/* Nucleo F401RE uart echo example (MCAL version)*/
#include <stm32f401xe.h>
#include <stdio.h>
#include <gpio.h>
#include <uart.h>

#define USART2_PORT GPIOA
#define USART2_TX_PIN 2
#define USART2_RX_PIN 3

void uart2_setup()
{
    // Setup gpio pins PA2 and PA3 for use with USART2
    gpio_enable_port_clock(GPIOA);
    gpio_set_mode(USART2_PORT, USART2_TX_PIN, ALTFUNC);
    gpio_set_af(USART2_PORT, USART2_TX_PIN, AF7);
    gpio_set_mode(USART2_PORT, USART2_RX_PIN, ALTFUNC);
    gpio_set_af(USART2_PORT, USART2_RX_PIN, AF7);

    uart_enable_clock(USART2);
    uart_set_baudrate(USART2, 115200);
    uart_enable(USART2); // Enable Usart TX & RX
    uart_enable_isr(USART2);
}

int main()
{
    uart2_setup();

    while (1)
    {
    }
}

void USART2_IRQHandler()
{
    // make sure data is available.
    if (uart_data_available(USART2))
    {
        char byte = uart_read_byte(USART2);
        // write byte back
        uart_write_byte(USART2, byte);
    }
}
