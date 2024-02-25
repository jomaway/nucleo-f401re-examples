// Using a sn74hc595n shift register.

#include <stm32f4xx.h>
#include <gpio.h>
#include <uart.h>
#include <delay.h>
#include <stdio.h>

int _write(int fd, char *ptr, int len)
{
    (void)fd;
    if (fd == 1)
    {
        return uart_write(USART2, (uint8_t *)ptr, (uint32_t)len);
        // return the number of bytes written.
    }
    return -1;
}

#define DS_PORT GPIOA
#define DS_PIN 9
#define STCP_PORT GPIOA
#define STCP_PIN 8
#define SHCP_PORT GPIOA
#define SHCP_PIN 5

#define delay_time 500

void gpio_setup();
void shift_out_data(uint8_t data);
void store_data(uint8_t data);

volatile uint8_t pattern[50] = { 0x55, 0xCC, 0xF0, 0x0F, 0x33, 0xAA, 0x00};
volatile bool run_pattern = true;

int main()
{
    /*setup*/
    systick_init();
    gpio_setup();
    uart_setup(USART2, 9600);

    uint8_t loading[8] = {0x01, 0x03, 0x7, 0xF, 0x1F, 0x3F, 0x7F, 0xFF};
    for (int i = 0; i< 8; i++)
    {
        store_data(loading[i]);
        systick_delay_ms(250);
    }

    while (1)
    {
        /*loop*/
        if (run_pattern)
        {
            for (int i = 0; i< 50; i++)
            {
                if (pattern[i] != 0x00 && run_pattern){
                    store_data(pattern[i]);
                    systick_delay_ms(delay_time);
                } else {
                    break;
                }
            }
        }
    }
}

void gpio_setup()
{
    gpio_enable_port_clock(DS_PORT);
    gpio_set_mode(DS_PORT, DS_PIN, OUTPUT);
    gpio_set_mode(SHCP_PORT, SHCP_PIN, OUTPUT);
    gpio_set_mode(STCP_PORT, STCP_PIN, OUTPUT);
}

void shift_out_data(uint8_t data)
{
    for (int i = 7; i >= 0; --i)
    {
        uint8_t value = data & (1 << i);

        gpio_write_pin_state(DS_PORT, DS_PIN, value);
        // pull shift pin high
        gpio_write_pin_state(SHCP_PORT, SHCP_PIN, HIGH);
        // pull shift pin low
        gpio_write_pin_state(SHCP_PORT, SHCP_PIN, LOW);
    }
}

void store_data(uint8_t data)
{
    // pull storage pin low
    gpio_write_pin_state(STCP_PORT, STCP_PIN, LOW);

    // shift data into the shift register
    shift_out_data(data);

    // pull storage pin high to
    gpio_write_pin_state(STCP_PORT, STCP_PIN, HIGH);
}

void USART2_IRQHandler()
{
    // make sure data is available.
    if (uart_data_available(USART2))
    {
        run_pattern = false;
        uint8_t byte = uart_read_byte(USART2);
        store_data(byte);
        if ('\n' == byte) {
            run_pattern = true;
        }
    }
}