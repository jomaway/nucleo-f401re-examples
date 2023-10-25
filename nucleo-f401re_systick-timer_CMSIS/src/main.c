/* 
    Using the internal SysTick Timer for a delay
*/

// Define the microcontroller variant we use
#define STM32F401xE

/* Includes */
#include <stm32f4xx.h>

#include <gpio.h>
#include <sysTick.h>

/* Defines */


#define LED_PIN 5
#define LED2_PIN 6
#define OFF 0 // ON = 1

#define TIMER1_DELAY 500
#define TIMER2_DELAY 1000

int main()
{
    uint32_t timer1, timer2;
    bool led_state = OFF, led2_state = OFF;
    /* SETUP */
    systick_init(1000);  // SysTick = 1ms
    systick_set_timer(&timer1, TIMER1_DELAY);
    systick_set_timer(&timer2, TIMER2_DELAY);


    /* turn on clock on GPIOA */
    gpio_enable_port_clock(GPIOA);
    /* set PA5 to output mode */
    gpio_set_pin_mode(GPIOA, LED_PIN, OUTPUT);
    gpio_set_pin_mode(GPIOA, LED2_PIN, OUTPUT);


    /* LOOP */
    while(1)
    {
        if(systick_is_timer_expired(&timer1))
        {
            led_state = !led_state;
            if(led_state)
            {
                gpio_write_pin_state(GPIOA, LED_PIN, HIGH);
            }
            else
            {
                gpio_write_pin_state(GPIOA, LED_PIN, LOW);
            }
            systick_set_timer(&timer1, TIMER1_DELAY);
        }
        if(systick_is_timer_expired(&timer2))
        {
            led2_state = !led2_state;
            if(led2_state)
            {
                gpio_write_pin_state(GPIOA, LED2_PIN, HIGH);
            }
            else
            {
                gpio_write_pin_state(GPIOA, LED2_PIN, LOW);
            }
            systick_set_timer(&timer2, TIMER2_DELAY);
        }
    }
}


