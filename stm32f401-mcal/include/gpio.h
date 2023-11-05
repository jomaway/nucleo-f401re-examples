/* gpio.h is part of the rdf_mcal lib */

#ifndef RDF_MCAL_GPIO_H
#define RDF_MCAL_GPIO_H

#include <stm32f4xx.h>

/* GPIO Pin definitions */
typedef enum
{
	PIN_0 = 0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7,
	PIN_8,
	PIN_9,
	PIN_10,
	PIN_11,
	PIN_12,
	PIN_13,
	PIN_14,
	PIN_15
} GPIO_PIN_t;

/* Pin Mode definitions */
typedef enum
{
	INPUT = 0,
	OUTPUT,
	ALTFUNC,
	ANALOG
} GPIO_PIN_MODE_t;

typedef enum
{
	PUSHPULL = 0,
	OPENDRAIN
} GPIO_OUTPUT_TYPE_t;

typedef enum
{
	NONE = 0,
	PULLUP,
	PULLDOWN,
} GPIO_PULL_TYPE_t;

/* Pin State definition */
typedef enum
{
	LOW = 0,
	HIGH = 1,
} GPIO_PIN_STATE_t;

typedef enum
{
	LOW_SPEED = 0,
	MEDIUM_SPEED,
	HIGH_SPEED,
	VERY_HIGH_SPEED,
} GPIO_SPEED_t;

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
void gpio_set_output_type(GPIO_TypeDef *port, uint8_t pin, GPIO_OUTPUT_TYPE_t type);
void gpio_set_pull_type(GPIO_TypeDef *port, uint8_t pin, GPIO_PULL_TYPE_t type);
void gpio_set_pin(GPIO_TypeDef *port, uint8_t pin);
void gpio_reset_pin(GPIO_TypeDef *port, uint8_t pin);
void gpio_write_pin_state(GPIO_TypeDef *port, uint8_t pin, GPIO_PIN_STATE_t state);
GPIO_PIN_STATE_t gpio_read_pin_state(GPIO_TypeDef *port, uint8_t pin);
void gpio_toggle_pin_state(GPIO_TypeDef *port, GPIO_PIN_t pin);
void gpio_set_pin_speed(GPIO_TypeDef *port, uint8_t pin, GPIO_SPEED_t speed);
void gpio_set_pin_af(GPIO_TypeDef *port, uint8_t pin, GPIO_ALT_FUNC_t af);
#endif