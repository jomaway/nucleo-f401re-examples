/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_SYSTICK_H
#define RDF_MCAL_SYSTICK_H

#include <stm32f4xx.h>
#include <stdbool.h>


/* Global variables */
volatile uint32_t msTicks;  // needs to be volatile !


/* Functions */
void systick_init(uint16_t divider);
void systick_set_timer(uint32_t *timer, uint16_t delay);
bool systick_is_timer_expired(uint32_t *timer);



#endif // RDF_MCAL_SYSTICK_H