#include "led.h"

void led_setup(GPIO_TypeDef *port, uint8_t led_pin)
{
    /* reset the pins mode to 0b00   */
    port->MODER &= ~(0b11 << (led_pin * 2));

    /* set the led pin to output mode (0b01) */
    port->MODER |= (0b01 << (led_pin * 2));
}

void led_set_on(GPIO_TypeDef *port, uint8_t led_pin)
{
    port->BSRR |= (1 << led_pin);
}

void led_set_off(GPIO_TypeDef *port, uint8_t led_pin)
{
    port->BSRR |= (1 << (led_pin + 16));
}

void led_toggle(GPIO_TypeDef *port, uint8_t led_pin)
{
    port->ODR ^= (1 << led_pin);
}

void led_set_state(GPIO_TypeDef *port, uint8_t led_pin, uint32_t led_state)
{
    if (LED_ON == led_state)
    {
        led_set_on(port, led_pin);
    }
    else
    {
        led_set_off(port, led_pin);
    }
}

uint32_t led_get_state(GPIO_TypeDef *port, uint8_t led_pin)
{
    return (port->ODR & (1 << led_pin));
}
