/*
    Using Timer 3 in pwm mode to dimm an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>
#include <gpio.h>
#include <timer.h>
#include <delay.h>

/* Global Variables */
volatile uint32_t duty_cycle = 1400;
volatile uint32_t btn_state = 0;

/* Function declaration */
void gpio_init();
void timer3_init();
bool btn_press_detected();

int main()
{

    systick_init();
    gpio_init();
    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, PIN_6, ALTFUNC);  // Set PA6 to alternate function
    gpio_set_pin_af(GPIOA, PIN_6, AF2);  // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)

    timer3_init();

    while(1)
    {
        // Do nothing
        if (btn_press_detected()) {
            duty_cycle += 100;
            if (duty_cycle > 7000){
                duty_cycle = 1400;
            }
        }
        timer_set_oc_preload_value(TIM3,TIM_CH1, duty_cycle);
    }
}

bool btn_press_detected()
{
    if (LOW == gpio_read_pin_state(GPIOC, PIN_13)) 
    {
        btn_state = 1;
        delay_ms(50);   // debounce button
    } else {
        if (1 == btn_state) {
            btn_state = 0;
            delay_ms(50);   // debounce button
            return true;
        }
    }
    return false;
}

void gpio_init()
{
    // Init PC_13 as input push button
    gpio_enable_port_clock(GPIOC);
    gpio_set_pin_mode(GPIOC, PIN_13, INPUT);
}

void timer3_init()
{
    #define TIM3_PRESCALER 5 - 1
    #define TIM3_AUTO_RELOAD_VALUE 64000
    #define TIM3_OC_PRELOAD_VALUE 3200

     // Enable clock for TIM3 on APB1 Bus
    timer_enable_bus_clock(TIM3);

    // Set Prescaler for TIM3 
    timer_set_prescaler(TIM3, TIM3_PRESCALER);

    // Set Auto Relaod Register
    timer_set_auto_reload_value(TIM3, TIM3_AUTO_RELOAD_VALUE);

    // Set PWM_1 mode.
    timer_set_oc_mode(TIM3, TIM_CH1, CHN_PWM_1);

    // Enable Preload
    timer_enable_oc_preload(TIM3, TIM_CH1, true);

    // Set Preload Value
    timer_set_oc_preload_value(TIM3, TIM_CH1, TIM3_OC_PRELOAD_VALUE); 

    // Activate Channel 1 as output
    timer_enable_oc_channel(TIM3, TIM_CH1);

    // Start timer 3
    timer_enable(TIM3);
}