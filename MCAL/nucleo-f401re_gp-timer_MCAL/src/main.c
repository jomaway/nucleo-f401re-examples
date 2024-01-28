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

void gpio_setup()
{
    gpio_enable_port_clock(LED_PORT);
    gpio_set_mode(LED_PORT, LED_PIN, OUTPUT);
}

void timer_setup()
{
    // Enable clock for TIM3 on APB1 Bus
    timer_enable_bus_clock(TIM3);

    // Set Prescaler for TIM3
    timer_set_prescaler(TIM3, TIM3_PRESCALER);

    // Set Auto Relaod Register
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);

    // Enable the timer
    timer_enable(TIM3);

    // Enable timer interrupt (TIM3_IRQHandler)
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
    // check the overflow flag, otherwise the toggle get called twice.
    if (timer_is_overflow(TIM3))
    {
        gpio_toggle_pin_state(LED_PORT, LED_PIN);
    }

    // reset overflow flag
    timer_reset_uif(TIM3);
}
