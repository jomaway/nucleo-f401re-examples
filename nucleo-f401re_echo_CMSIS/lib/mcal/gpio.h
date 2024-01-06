/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_GPIO_H
#define RDF_MCAL_GPIO_H

#include <stm32f4xx.h>

/* Pin Mode definitions */
typedef enum
{
	INPUT = 0,
	OUTPUT,
	ALTFUNC,
	ANALOG
} GPIO_PIN_MODE_t;

/* Pin State definition */
typedef enum
{
	LOW = 0,
	HIGH = 1,
} GPIO_PIN_STATE_t;

typedef enum 
{
	AF0 = 0,
	AF1,
	AF2,
	AF3,
	AF4,
	AF5,
	AF6,
	AF7,
	AF8,
	AF9,
	AF10,
	AF11,
	AF12,
	AF13,
	AF14,
	AF15,
} GPIO_ALT_FUNC_t;

void gpio_enable_port_clock(GPIO_TypeDef *port);
void gpio_set_pin_mode(GPIO_TypeDef *port, uint8_t pin, GPIO_PIN_MODE_t mode);
void gpio_write_pin_state(GPIO_TypeDef *port, uint8_t pin, GPIO_PIN_STATE_t state);
GPIO_PIN_STATE_t gpio_read_pin_state(GPIO_TypeDef *port, uint8_t pin);
void gpio_set_pin_af(GPIO_TypeDef *port, uint8_t pin, GPIO_ALT_FUNC_t af);
#endif