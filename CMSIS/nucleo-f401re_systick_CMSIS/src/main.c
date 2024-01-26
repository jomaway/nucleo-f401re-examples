/*
    Using the internal SysTick Timer for a delay with msTicks counting.
    Depending on the max value of the msTicks variable the counter will overflow at some point.
    This should be noticed in a real project.
*/

// Define the microcontroller variant we use
#define STM32F401xE

/* Includes */
#include <stm32f4xx.h>

#include <led.h>
#include <sysTick.h>

/* Defines for our LEDs */
#define LED_1_PORT GPIOA
#define LED_1_PIN 5

#define LED_2_PORT GPIOA
#define LED_2_PIN 6

#define TIMER1_DELAY 500
#define TIMER2_DELAY 1000

// Setup all used gpios.
void gpio_setup()
{
    /* Turn on clock for GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    // LED_1 (see led.c for led_setup)
    led_setup(LED_1_PORT, LED_1_PIN);
    // LED_2
    led_setup(LED_2_PORT, LED_2_PIN);
}

int main()
{
    uint32_t led_1_state = LED_OFF, led_2_state = LED_OFF;
    uint32_t timer1, timer2;

    gpio_setup();
    systick_setup(1000);

    // starting timer
    systick_start_timer(&timer1, TIMER1_DELAY);
    systick_start_timer(&timer2, TIMER2_DELAY);

    /* LOOP */
    while (1)
    {
        if (systick_is_timer_expired(&timer1))
        {
            led_1_state = !led_1_state;
            systick_start_timer(&timer1, TIMER1_DELAY);
        }
        if (systick_is_timer_expired(&timer2))
        {
            led_2_state = !led_2_state;
            systick_start_timer(&timer2, TIMER2_DELAY);
        }

        // Update LED 1 state
        if (LED_ON == led_1_state)
        {
            led_set_on(LED_1_PORT, LED_1_PIN);
        }
        else
        {
            led_set_off(LED_1_PORT, LED_1_PIN);
        }

        // Update LED 2 state with led_set_state function
        led_set_state(LED_2_PORT, LED_2_PIN, led_2_state);
    }
}
