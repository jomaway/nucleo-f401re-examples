#include <stm32f4xx.h>
#include <gpio.h>
#include <uart.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <led.h>

#ifdef USE_UART_PRINTF
#include <stdio.h>
int _write(int fd, char *ptr, int len)
{
    (void)fd;
    if (fd == 1)
        uart_write(USART2, (uint8_t *)ptr, (uint32_t)len);
    return -1;
}
#define UART_LOG(msg...) printf(msg);
#else
#define UART_LOG(msg...) ;
#endif

#define RX_BUFFER_SIZE 64

// Buffer to store received data
volatile uint8_t rx_buffer[RX_BUFFER_SIZE];
volatile uint32_t rx_index = 0;
volatile bool rx_buffer_overflow = false;
volatile bool rx_newline = false;

void clear_rx_buffer()
{
    for (size_t i = 0; i < RX_BUFFER_SIZE; i++)
    {
        rx_buffer[i] = 0;
    }
    rx_index = 0;
}

typedef struct Stats
{
    uint8_t received;
    uint8_t invalid;
} stats_t;

// init struct for statistics
stats_t stats = {0, 0};

typedef enum
{
    CMD_INVALID,
    CMD_LED_ON,
    CMD_LED_OFF,
    CMD_HELLO,
    CMD_STATS,
} SERIAL_COMMAND_t;

SERIAL_COMMAND_t process_input(volatile uint8_t *buf)
{
    if (strncmp((const char *)buf, "on\r\n", 4) == 0)
    {
        return CMD_LED_ON;
    }
    else if (strncmp((const char *)buf, "off\r\n", 5) == 0)
    {
        return CMD_LED_OFF;
    }
    else if (strncmp((const char *)buf, "hello\r\n", 7) == 0)
    {
        return CMD_HELLO;
    }
    else if (strncmp((const char *)buf, "stats\r\n", 7) == 0)
    {
        return CMD_STATS;
    }
    else
    {
        return CMD_INVALID;
    }
}

void execute_command(SERIAL_COMMAND_t cmd)
{
    stats.received++;
    switch (cmd)
    {
    case CMD_LED_ON:
        led_on();
        UART_LOG("\033[32m DEBUG: LED turned ON.\033[0m \n");
        break;
    case CMD_LED_OFF:
        led_off();
        UART_LOG("\033[32m DEBUG: LED turned OFF. \033[0m \n");
        break;
    case CMD_HELLO:
        UART_LOG("\033[34m HELLO from the Nucleo-STM32F401RE :) \033[0m \n");
        break;
    case CMD_STATS:
        UART_LOG("STATS:\033[33m received: %d,\033[91m invalid: %d,\033[32m valid: %d \033[0m \n", stats.received, stats.invalid, stats.received - stats.invalid);
        break;
    default:
        stats.invalid++;
        UART_LOG("\033[91;1m DEBUG: Invalid command\033[0m \n");
        break;
    }
}

int main()
{
    // Setup UART2 with a baudrate of 115200
    uart_setup(USART2, 115200);
    led_setup();

    while (1)
    {
        if (rx_buffer_overflow)
        {
            // handle buffer overflow in uart
            clear_rx_buffer(rx_buffer, RX_BUFFER_SIZE, rx_index);
            rx_buffer_overflow = false; // just clearing the buffer might have impact on next incomming parts.
            UART_LOG("\033[91;1m ERROR: rx_buffer overflow detected.\033[33m \n");
        }

        if (rx_newline)
        {
            // process command
            UART_LOG("\033[0m")
            SERIAL_COMMAND_t cmd = process_input(rx_buffer);
            clear_rx_buffer(rx_buffer, RX_BUFFER_SIZE, rx_index);
            rx_newline = false;
            execute_command(cmd);
        }
    }
}

void USART2_IRQHandler(void)
{
    if (uart_data_available(USART2))
    {
        uint8_t byte = uart_read_byte(USART2);
        uart_write_byte(USART2, byte);

        // check for buffer overflow
        if (rx_index >= RX_BUFFER_SIZE)
        {
            rx_buffer_overflow = true;
            rx_index = 0;
        }
        else
        {
            rx_buffer[rx_index++] = byte;
        }

        if ('\n' == byte)
        {
            rx_newline = true;
        }
    }
}
