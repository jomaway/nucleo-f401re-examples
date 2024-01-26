/*
    Using Timer 3 with interrupts to blink an LED.
*/

/* Includes */
#include <stm32f4xx.h>

#include <stdbool.h>
#include <gpio.h>

/* Defines */
#define TIM3_PRESCALER 16000 - 1
#define TIM3_AUTO_RELOAD_VALUE 1000 - 1
#define TIM3_INTERUPT_ENABLE true

/* Function declaration */
void timer_init(TIM_TypeDef *timer, uint16_t prescaler, uint16_t reload_value, bool interrupt_enable);
void timer_reset_uif(TIM_TypeDef *timer);

/* global Variables */

int main()
{
    __disable_irq();
    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, PIN_5, OUTPUT);

    timer_init(TIM3, TIM3_PRESCALER, TIM3_AUTO_RELOAD_VALUE, TIM3_INTERUPT_ENABLE);
    __NVIC_EnableIRQ(TIM3_IRQn);
    __enable_irq();

    while(1)
    {
        // Do nothing
    }
}

void TIM3_IRQHandler(void)
{
    gpio_toggle_pin_state(GPIOA, PIN_5);
    timer_reset_uif(TIM3);
}


/* Funktions */
void timer_init(TIM_TypeDef *timer, uint16_t prescaler, uint16_t reload_value, bool interrupt_enable)
{
    // Enable clock for TIM3 on APB1 Bus
    if(TIM3 == timer)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
    // else ... for other timers.

    // Set Prescaler for TIM3 
    timer->PSC = prescaler;

    // Set Auto Relaod Register
    timer->ARR = reload_value;

    // enable interrupt
    if (interrupt_enable)
    {
        timer->DIER |= TIM_DIER_UIE;
    }
    // Enable Timer 3 module
    timer->CR1 |= TIM_CR1_CEN;
}

void timer_reset_uif(TIM_TypeDef *timer)
{
    timer->SR &= ~TIM_SR_UIF;
}