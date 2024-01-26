/*
    Using Timer 3 in output compare mode to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>
#include <gpio.h>
#include <timer.h>

/* Defines */
#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000
#define TIM_CCMR1_OC1M_TOGGLE  (0b11UL << 4U)

/* Function declaration */
void timer3_init();

/* global Variables */

int main()
{

    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, PIN_6, ALTFUNC);  // Set PA6 to alternate function
    gpio_set_pin_af(GPIOA, PIN_6, AF2);  // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)

    timer3_init();
    timer_enable(TIM3);

    while(1)
    {
        // Do nothing
    }
}

/* Funktions */
void timer3_init()
{
    // Enable clock for TIM3 on APB1 Bus
    timer_enable_bus_clock(TIM3);

    // Set Prescaler for TIM3 
    timer_set_prescaler(TIM3, TIM3_PRESCALER);

    // Set Auto Relaod Register
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);

    // Set Toggle mode.
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;
    //timer_set_oc_mode(TIM3, TIM_CH1, CHN_TOGGLE);

    // Activate Channel 1 as output
    TIM3->CCER |= TIM_CCER_CC1E;
    //timer_enable_oc_channel(TIM3, TIM_CH1);
}
