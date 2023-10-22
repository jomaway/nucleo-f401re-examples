// Define the microcontroller variant we use
#define STM32401xE

/* Includes */
#include <stm32f4xx.h>

#include <gpio.h>  // gpio functions from our mcal lib

/* DEFINES */
#define LED_RED_PIN 6       // Red led on PA6
#define LED_YELLOW_PIN 7    // Yellow led on PA7
#define LED_GREEN_PIN 8     // Green led on PA8
#define BTN_PIN 9           // Button on PA9


/* Function declaration */
static void delay_ms(uint32_t ms);
void switch_traffic_light();