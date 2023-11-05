/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_SYSTICK_H
#define RDF_MCAL_SYSTICK_H

#include <stm32f4xx.h>
#include <inttypes.h>
#include <stdbool.h>



typedef struct {
	uint32_t delay;
	uint32_t timestamp;
} Delay_TypeDef;


// SysTick Delay (only ms)
void systick_init();
void delay_ms(uint32_t ms);


// DWT Delay (ms, us)
void DWT_init(void);
uint32_t DWT_tick(void);
uint32_t DWT_millis(void);
uint32_t DWT_micros(void);
void DWT_delay_ms(uint32_t delay);
void DWT_delay_us(uint32_t delay);
// DWT Non Blocking Delay (ms, us)
uint8_t DWT_nb_timeout(Delay_TypeDef *dt);
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay);
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay);

#endif // RDF_MCAL_SYSTICK_H