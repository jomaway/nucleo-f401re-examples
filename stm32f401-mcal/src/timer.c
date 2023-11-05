#include "timer.h"

void timer_enable_bus_clock(TIM_TypeDef *timer)
{
    if(TIM1 == timer)
    {
        RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    }
    else if(TIM2 == timer)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    }
    else if(TIM3 == timer)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    }
    else if(TIM4 == timer)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    }
    else if(TIM5 == timer)
    {
        RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
    }
    else if(TIM9 == timer)
    {
        RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
    }
    else if(TIM10 == timer)
    {
        RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
    }
    else if(TIM11 == timer)
    {
        RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
    }
}

void timer_set_prescaler(TIM_TypeDef *timer, uint16_t prescaler)
{
    timer->PSC = prescaler;
}

void timer_set_auto_reload_value(TIM_TypeDef *timer, uint16_t auto_reload_value)
{
    timer->ARR = auto_reload_value;
}

void timer_set_interrupt_active(TIM_TypeDef *timer)
{
    timer->DIER |= TIM_DIER_UIE;
}

void timer_enable(TIM_TypeDef *timer)
{
    timer->CR1 |= TIM_CR1_CEN;
}

bool timer_is_overflow(TIM_TypeDef *timer)
{
    return (TIM_SR_UIF == (timer->SR & TIM_SR_UIF));
}

void timer_reset_uif(TIM_TypeDef *timer)
{
    timer->SR &= ~TIM_SR_UIF;
}