/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_TIMER_H
#define RDF_MCAL_TIMER_H

#include <stm32f4xx.h>
#include <stdbool.h>


/* Global variables */



/* Functions */
void timer_enable_bus_clock(TIM_TypeDef *timer);
void timer_set_prescaler(TIM_TypeDef *timer, uint16_t prescaler);
void timer_set_auto_reload_value(TIM_TypeDef *timer, uint16_t auto_reload_value);
void timer_set_interrupt_active(TIM_TypeDef *timer);
void timer_enable(TIM_TypeDef *timer);
bool timer_is_overflow(TIM_TypeDef *timer);
void timer_reset_uif(TIM_TypeDef *timer);

#endif // RDF_MCAL_TIMER_H