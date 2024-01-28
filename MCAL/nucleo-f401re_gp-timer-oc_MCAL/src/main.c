/*
    Using Timer 3 in output compare mode to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>
#include <gpio.h>
#include <timer.h>

/* Defines */
#define LED_PORT GPIOA
#define LED_PIN 6

#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000

/* Functions */

void gpio_setup()
{
    gpio_enable_port_clock(GPIOA);
    gpio_set_mode(LED_PORT, LED_PIN, ALTFUNC); // Set PA6 to alternate function
    gpio_set_af(LED_PORT, LED_PIN, AF2);       // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)
}

void timer_setup()
{
    // Enable clock for TIM3 on APB1 Bus
    timer_enable_clock(TIM3);

    // Set Prescaler for TIM3
    timer_set_prescaler(TIM3, TIM3_PRESCALER);

    // Set Auto Relaod Register
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);

    // Set Toggle mode.
    timer_set_oc_mode(TIM3, TIM_CH1, CHN_TOGGLE);

    // Activate Channel 1 as output
    timer_enable_oc_channel(TIM3, TIM_CH1);

    // Enable the timer
    timer_enable(TIM3);
}

int main()
{
    gpio_setup();
    timer_setup();

    while (1)
    {
        // Do nothing
    }
}
