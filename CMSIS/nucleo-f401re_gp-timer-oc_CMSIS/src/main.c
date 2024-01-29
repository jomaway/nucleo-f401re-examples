/*
    Using Timer 3 in output compare mode to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>

/* Defines */
#define LED_PORT GPIOA
#define LED_PIN 6
#define LED_PIN_ALTFUNC 2

#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000
#define TIM_CCMR1_OC1M_TOGGLE (0b11UL << 4U)

/* Function declaration */
void timer3_init();

/* global Variables */

void gpio_setup()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER6_0;
    GPIOA->AFR[0] |= (LED_PIN_ALTFUNC << ((LED_PIN & 7) * 4));
    // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)
}

void timer_setup()
{
    // Enable clock for TIM3 on APB1 Bus
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Set Prescaler for TIM3
    TIM3->PSC = TIM3_PRESCALER;

    // Set Auto Relaod Register
    TIM3->ARR = TIM3_AUTO_RELOAD_VALUE;

    // Set Toggle mode.
    TIM3->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;

    // Activate Channel 1 as output
    TIM3->CCER |= TIM_CCER_CC1E;

    // Enable timer
    TIM3->CR1 |= TIM_CR1_CEN;
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
