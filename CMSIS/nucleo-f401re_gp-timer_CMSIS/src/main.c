/*
    Using Timer 3 with interrupts to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>

/* Defines */
#define LED_PORT GPIOA
#define LED_PIN 5

#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000 - 1

/* Function declaration */

void gpio_setup()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    LED_PORT->MODER |= GPIO_MODER_MODER5_0;
}

void timer_setup()
{
    // enable clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    // Set Prescaler for TIM3
    TIM3->PSC = TIM3_PRESCALER;

    // Set Auto Relaod Register
    TIM3->ARR = TIM3_AUTO_RELOAD_VALUE;

    // enable interrupt
    TIM3->DIER |= TIM_DIER_UIE;

    // Enable Timer 3 module
    TIM3->CR1 |= TIM_CR1_CEN;

    // Setup NVIC
    NVIC_EnableIRQ(TIM3_IRQn);
}

int main()
{
    __disable_irq();
    gpio_setup();
    timer_setup();

    __enable_irq();

    while (1)
    {
        // Do nothing
    }
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & TIM_SR_UIF)
    {
        LED_PORT->ODR ^= (1 << LED_PIN);
    }
    TIM3->SR &= ~TIM_SR_UIF;
}