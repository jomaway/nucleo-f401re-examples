/* gpio.h is part of the rdf_mcal lib */

#ifndef __SYSTICK_H
#define __SYSTICK_H

#include <stm32f4xx.h>
#include <stdbool.h>

void systick_setup(uint16_t divider);
void systick_start_timer(uint32_t *timer, uint16_t delay);
bool systick_is_timer_expired(uint32_t *timer);
void delay_ms(uint32_t ms);

#endif // __SYSTICK_H