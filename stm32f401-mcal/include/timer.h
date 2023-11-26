/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_TIMER_H
#define RDF_MCAL_TIMER_H

#include <stm32f4xx.h>
#include <stdbool.h>


typedef enum
{
    CHN_FROZEN,
    CHN_ACTIVE_ON_MATCH,
    CHN_INACTIVE_ON_MATCH,
    CHN_TOGGLE,
    CHN_FORCE_LOW,
    CHN_FORCE_HIGH,
    CHN_PWM_1,
    CHN_PWM_2,

}TIMER_OC_MODE_t;

typedef enum
{
    TIM_CH1 = 0,
    TIM_CH2,
    TIM_CH3,
    TIM_CH4,
}TIMER_CHANNEL_NUM_t;


/* Functions */
void timer_enable_bus_clock(TIM_TypeDef *timer);
void timer_set_prescaler(TIM_TypeDef *timer, uint16_t prescaler);
void timer_set_auto_reload_value(TIM_TypeDef *timer, uint16_t auto_reload_value);
void timer_set_interrupt_active(TIM_TypeDef *timer);
void timer_enable(TIM_TypeDef *timer);
bool timer_is_overflow(TIM_TypeDef *timer);
void timer_reset_uif(TIM_TypeDef *timer);

/* Setting the timers output compare mode in the TIMx->CCMRx */
void timer_set_oc_mode(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number, TIMER_OC_MODE_t mode);
/* Enable output compare preload by setting the corresponding OCxPE in TIMy->CCMRx */
void timer_enable_oc_preload(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number, bool preload);
/* Set a preload value in TIMx->CCRx (default: 0x0000)*/
void timer_set_oc_preload_value(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number, uint32_t value);
/* Enable output compare channel by setting the corresponding CCxE in the TIMx->CCER */
void timer_enable_oc_channel(TIM_TypeDef *timer, TIMER_CHANNEL_NUM_t channel_number);

#endif // RDF_MCAL_TIMER_H