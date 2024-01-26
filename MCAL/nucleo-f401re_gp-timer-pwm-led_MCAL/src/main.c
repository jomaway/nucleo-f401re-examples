/*
    Using Timer 3 in pwm mode to dimm an LED.
*/

/* Includes */
#include <stm32f4xx.h>
#include <stdbool.h>
#include <gpio.h>
#include <timer.h>


/* Defines */
#define TIM3_PRESCALER 10 - 1
#define TIM3_AUTO_RELOAD_VALUE 10000 - 1

/* Global Variables */
volatile uint32_t btn_state = 0;

/* Function declaration */
void gpio_init();
void timer3_init();
bool btn_press_detected();

int main()
{
    gpio_init();
    timer3_init();


    uint32_t duty_cycle = 0;
    while(1)
    {
        // Do nothing
        if (btn_press_detected()) {
            duty_cycle += 1000;
            if (duty_cycle > TIM3_AUTO_RELOAD_VALUE){
                duty_cycle = 0;
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
    } else {
        if (1 == btn_state) {
            btn_state = 0;
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

    // Init PA_6 as TIMER 3 Channel 1 -> AF2
    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, PIN_6, ALTFUNC);  // Set PA6 to alternate function
    gpio_set_pin_af(GPIOA, PIN_6, AF2);  // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)
}

void timer3_init()
{

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
    //timer_set_oc_preload_value(TIM3, TIM_CH1, 0); 

    // Activate Channel 1 as output
    timer_enable_oc_channel(TIM3, TIM_CH1);

    // Start timer 3
    timer_enable(TIM3);
}