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

/*
    verify if a given timer has the given channel or not.
*/
static bool verify_oc_channel_assignment(TIM_TypeDef *timer,  TIMER_CHANNEL_NUM_t channel_number)
{
    // TIM10 and TIM11 only have Channel 1
    // Check assignment for channels > TIM_CH1
    if ((TIM10 == timer || TIM11 == timer) && (channel_number > TIM_CH1))
    {
        return false;
    }

    // TIM9 only has Channel 1 & Channel 2
    // Check assignment for channels > TIMIO_CH2
    if ((TIM9 == timer) && (channel_number > TIM_CH2))
    {
        return false;
    }

    return true;
}

void timer_set_oc_mode(TIM_TypeDef *timer,  TIMER_CHANNEL_NUM_t channel_number, TIMER_OC_MODE_t mode)
{
    #define TIM_CCMR1_OC1M_OFFSET 4
    #define TIM_CCMR1_OC2M_OFFSET 12
    #define TIM_CCMR2_OC3M_OFFSET 4
    #define TIM_CCMR2_OC4M_OFFSET 12

    if (!verify_oc_channel_assignment(timer, channel_number))
    {
        return;     // todo: handle some error message instead of returning silent
    }

    switch (channel_number)
    {
    case TIM_CH1:
        timer->CCMR1 &= ~(0b111 << TIM_CCMR1_OC1M_OFFSET);  // reset channel 1
        timer->CCMR1 |= (mode << TIM_CCMR1_OC1M_OFFSET);    // set new mode for channel 1
        break;
    case TIM_CH2:
        timer->CCMR1 &= ~(0b111 << TIM_CCMR1_OC2M_OFFSET);  // reset channel 2
        timer->CCMR1 |= (mode << TIM_CCMR1_OC2M_OFFSET);    // set new mode for channel 2
        break;
    case TIM_CH3:
        timer->CCMR2 &= ~(0b111 << TIM_CCMR2_OC3M_OFFSET);  // reset channel 3
        timer->CCMR2 |= (mode << TIM_CCMR2_OC3M_OFFSET);    // set new mode for channel 3
        break;
    case TIM_CH4:
        timer->CCMR2 &= ~(0b111 << TIM_CCMR2_OC4M_OFFSET);  // reset channel 4
        timer->CCMR2 |= (mode << TIM_CCMR2_OC4M_OFFSET);    // set new mode for channel 4
        break;
    }

}

void timer_enable_oc_preload(TIM_TypeDef *timer,TIMER_CHANNEL_NUM_t channel_number, bool enable)
{
    if (!verify_oc_channel_assignment(timer, channel_number))
    {
        return;     // todo: handle some error message instead of returning silent
    }

    switch (channel_number)
    {
    case TIM_CH1:
        timer->CCMR1 &= ~TIM_CCMR1_OC1PE_Msk;   // reset to preload disabled
        if (enable) { timer->CCMR1 |= TIM_CCMR1_OC1PE; }
        break;
    case TIM_CH2:
        timer->CCMR1 &= ~TIM_CCMR1_OC2PE_Msk;   // reset to preload disabled
        if (enable) { timer->CCMR1 |= TIM_CCMR1_OC2PE; }
        break;
    case TIM_CH3:
        timer->CCMR2 &= ~TIM_CCMR2_OC3PE_Msk;   // reset to preload disabled
        if (enable) { timer->CCMR2 |= TIM_CCMR2_OC3PE; }
        break;
    case TIM_CH4:
        timer->CCMR2 &= ~TIM_CCMR2_OC4PE_Msk;   // reset to preload disabled
        if (enable) { timer->CCMR2 |= TIM_CCMR2_OC4PE; }
        break;
    }
}

void timer_set_oc_preload_value(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number, uint32_t value)
{
    if (!verify_oc_channel_assignment(timer, channel_number))
    {
        return;     // todo: handle some error message instead of returning silent
    }

    switch (channel_number)
    {
    case TIM_CH1:
        timer->CCR1 = value;
        break;
    case TIM_CH2:
        timer->CCR2 = value;
        break;
    case TIM_CH3:
        timer->CCR3 = value;
        break;
    case TIM_CH4:
        timer->CCR4 = value;
        break;
    }
}

void timer_enable_oc_channel(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number)
{
    if (!verify_oc_channel_assignment(timer, channel_number))
    {
        return;     // todo: handle some error message instead of returning silent
    }

    #define TIM_CCER_CCxE_OFFSET 4   // TIM_CH1 = 0, TIM_CH2 = 4, TIM_CH3 = 8, TIM_CH4 = 12, 
    timer->CCER |= (1U << (channel_number * TIM_CCER_CCxE_OFFSET));
}
