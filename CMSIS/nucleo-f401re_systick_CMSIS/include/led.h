#ifndef __LED_H
#define __LED_H

#include <stm32f4xx.h>

// Define LED states
#define LED_OFF 0
#define LED_ON 1

void led_setup(GPIO_TypeDef *port, uint8_t led_pin);
void led_set_on(GPIO_TypeDef *port, uint8_t led_pin);
void led_set_off(GPIO_TypeDef *port, uint8_t led_pin);
void led_toggle(GPIO_TypeDef *port, uint8_t led_pin);
void led_set_state(GPIO_TypeDef *port, uint8_t led_pin, uint32_t led_state);
uint32_t led_get_state(GPIO_TypeDef *port, uint8_t led_pin);

#endif // _LED_H