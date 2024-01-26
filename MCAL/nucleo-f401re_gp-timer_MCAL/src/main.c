/*
    Using Timer 3 with interrupts to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>

#include <stdbool.h>
#include <gpio.h>
#include <timer.h>

/* Defines */
#define LED_PORT GPIOA
#define LED_PIN 5

#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000 - 1
#define TIM3_INTERUPT_ENABLE true

void gpio_setup()
{
    gpio_enable_port_clock(LED_PORT);
    gpio_set_mode(LED_PORT, LED_PIN, OUTPUT);
}

void timer_setup()
{
    timer_enable_bus_clock(TIM3);
    timer_set_prescaler(TIM3, TIM3_PRESCALER);
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);
    timer_enable(TIM3);
    timer_set_interrupt_active(TIM3);
    NVIC_EnableIRQ(TIM3_IRQn);
}

int main()
{
    // SETUP
    __disable_irq();

    gpio_setup();
    timer_setup();
    
    __enable_irq();

    // LOOP
    while (1)
    {
        // Do nothing
    }
}

void TIM3_IRQHandler(void)
{
    gpio_toggle_pin_state(LED_PORT, LED_PIN);
    timer_reset_uif(TIM3);
}
