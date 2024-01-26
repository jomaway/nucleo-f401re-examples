#include <stm32f4xx.h>
#include <gpio.h>

void led_setup()
{
    gpio_enable_port_clock(GPIOA);
    gpio_set_mode(GPIOA, 5, OUTPUT);
}

void led_on()
{
    gpio_set_pin(GPIOA, 5);
}

void led_off()
{
    gpio_reset_pin(GPIOA, 5);
}