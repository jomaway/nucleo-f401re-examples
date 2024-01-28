/*
    Using Timer 3 in pwm mode to dimm an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>
#include <gpio.h>
#include <timer.h>
#include <uart.h>
#include <stdio.h>

/* Defines */
#define LED_PORT GPIOA
#define LED_PIN 6

#define BTN_PORT GPIOC
#define BTN_PIN 13

#define TIM3_PRESCALER 10 - 1
#define TIM3_AUTO_RELOAD_VALUE 10000 - 1

/* Global Variables */
volatile uint32_t btn_state = 0;

/* Function declaration */
void gpio_setup();
void timer_setup();

int main()
{
    gpio_setup();
    timer_setup();
    uart_setup(USART2, 115200);

    uint32_t duty_cycle = 0;
    while (1)
    {
        if (LOW == gpio_read_pin_state(BTN_PORT, BTN_PIN))
        {
            duty_cycle += 2;
            if (duty_cycle > TIM3_AUTO_RELOAD_VALUE)
            {
                duty_cycle = 0;
            }

            printf("DC: %lu\n", duty_cycle);
            timer_set_oc_preload_value(TIM3, TIM_CH1, duty_cycle);
        }
    }
}

void gpio_setup()
{
    // Set PC_13 as input push button
    gpio_enable_port_clock(BTN_PORT);
    gpio_set_mode(BTN_PORT, BTN_PIN, INPUT);

    // Set PA_6 as TIMER 3 Channel 1 -> AF2
    gpio_enable_port_clock(LED_PORT);
    gpio_set_mode(LED_PORT, LED_PIN, ALTFUNC); // Set PA6 to alternate function
    gpio_set_af(LED_PORT, LED_PIN, AF2);       // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)
}

void timer_setup()
{
    // Enable clock for TIM3 on APB1 Bus
    timer_enable_clock(TIM3);

    // Set Prescaler for TIM3
    timer_set_prescaler(TIM3, TIM3_PRESCALER);

    // Set Auto Relaod Register
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);

    // Set PWM_1 mode.
    timer_set_oc_mode(TIM3, TIM_CH1, CHN_PWM_1);

    // Enable Preload
    timer_enable_oc_preload(TIM3, TIM_CH1, true);

    // Set Preload Value
    timer_set_oc_preload_value(TIM3, TIM_CH1, 0);

    // Activate Channel 1 as output
    timer_enable_oc_channel(TIM3, TIM_CH1);

    // Start timer 3
    timer_enable(TIM3);
}