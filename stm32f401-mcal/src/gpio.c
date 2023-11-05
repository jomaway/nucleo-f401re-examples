#include "gpio.h"

void gpio_enable_port_clock(GPIO_TypeDef *port)
{
    // Check if port is a valid port

    if (GPIOA == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    }
    else if (GPIOB == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    }
    else if (GPIOC == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    }
    else if (GPIOD == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
    else if (GPIOE == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
    }
    else if (GPIOH == port)
    {
        RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
    }
}

void gpio_set_pin_mode(GPIO_TypeDef *port, uint8_t pin, GPIO_PIN_MODE_t mode)
{
    port->MODER &= ~(0b11 << (pin * 2));         // Clear to input (default)
    if (INPUT != mode)
    {
        port->MODER |= mode << (pin * 2);   // Set new mode
    }
}

void gpio_set_output_type(GPIO_TypeDef *port, uint8_t pin, GPIO_OUTPUT_TYPE_t type)
{
    port->OTYPER &= ~(1 << pin);  // reset to pushpull
    if (OPENDRAIN == type)
    {
        port->OTYPER |= (1 << pin);
    }
}

void gpio_set_pull_type(GPIO_TypeDef *port, uint8_t pin, GPIO_PULL_TYPE_t type)
{
    port->PUPDR &= ~(0b11 << (pin * 2)); //reset
    if (PULLUP == type || PULLDOWN == type)
    {
        port->PUPDR |= (type << (pin * 2));  //set new type;
    }
}

void gpio_set_pin(GPIO_TypeDef *port, uint8_t pin)
{
    port->BSRR = (1 << pin);
}

void gpio_reset_pin(GPIO_TypeDef *port, uint8_t pin)
{
    port->BSRR = (1 << (pin + 16));
}

void gpio_write_pin_state(GPIO_TypeDef *port, uint8_t pin, GPIO_PIN_STATE_t state)
{
    if (LOW == state){
        port->BSRR = (1 << (pin + 16));
    } else {
        port->BSRR = (1 << pin);
    }
}

GPIO_PIN_STATE_t gpio_read_pin_state(GPIO_TypeDef *port, uint8_t pin)
{
    if(port->IDR & (1 << pin)){
        return HIGH;
    }
    return LOW;
}

void gpio_toggle_pin_state(GPIO_TypeDef *port, GPIO_PIN_t pin)
{
    port->ODR ^= (1 << pin);
}

void gpio_set_pin_speed(GPIO_TypeDef *port, uint8_t pin, GPIO_SPEED_t speed)
{
    port->OSPEEDR &= ~(0b11 << (pin * 2));   // reset speed
    port->OSPEEDR |= speed << (pin * 2);     // set new speed
}

void gpio_set_pin_af(GPIO_TypeDef *port, uint8_t pin, GPIO_ALT_FUNC_t af)
{
    // SET pin to alternative function mode
    gpio_set_pin_mode(port, pin, ALTFUNC);

    if (pin <= 7)
    {
        // Reset previous selected AF
        port->AFR[0] &= ~(0x0F << pin * 4);
        // Set AFR[0] (Alternate Function Low Register) to the new AF
        port->AFR[0] |= af << (pin * 4);
    }
    else
    {
        // Reset previous selected AF
        port->AFR[1] &= ~(0x0F << (pin - 8) * 4);
        // Set AFR[1] (Alternate Function High Register) to the new AF
        port->AFR[1] |= af << ((pin - 8) * 4);
    }

    /* 
    * Alternativ without if: port->AFR[pin>>3] |= (af << ((pin & 7) * 4));
    */
}
