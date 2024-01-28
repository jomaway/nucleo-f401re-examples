// Using Timer 3 in pwm mode to control a servo.

/* Includes */
#include <stm32f4xx.h>
#include <gpio.h>
#include <timer.h>
#include <exti.h>
#include <stdio.h>
#include <uart.h>

/* Defines */
#define SERVO_PORT GPIOA
#define SERVO_PIN 6
#define SERVO_MIN_VAL 1400
#define SERVO_MAX_VAL 7000
#define SERVO_STEPS 100

#define RE_CLK_PORT GPIOA
#define RE_CLK_PIN 8
#define RE_CLK_EXTI_LINE RE_CLK_PIN
#define RE_DT_PORT GPIOA
#define RE_DT_PIN 9

#define TIM3_PRESCALER 5 - 1
#define TIM3_AUTO_RELOAD_VALUE 64000
#define TIM3_OC_PRELOAD_VALUE 3200

/* Global Variables */
volatile uint32_t duty_cycle = SERVO_MIN_VAL;

/* Function declaration */
void gpio_setup();
void encoder_setup();
void servo_setup();
int32_t clamp(int32_t value, int32_t min, int32_t max);

int main()
{
    servo_setup();
    encoder_setup();
    uart_setup(USART2, 115200);

    while (1)
    {
        printf("DC: %lu \n", duty_cycle);
    }
}

void encoder_setup()
{
    // Set CLK and DT as INPUT
    gpio_enable_port_clock(RE_CLK_PORT);
    gpio_enable_port_clock(RE_DT_PORT);
    gpio_set_mode(RE_CLK_PORT, RE_CLK_PIN, INPUT);
    gpio_set_mode(RE_DT_PORT, RE_DT_PIN, INPUT);

    // Set CLK as exti
    exti_enable_syscfg_clock();
    exti_set_source(RE_CLK_EXTI_LINE, RE_CLK_PORT);
    exti_set_trigger_edge(RE_CLK_EXTI_LINE, EXTI_FALLING_EDGE);
    exti_enable_irq(RE_CLK_EXTI_LINE);
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 0);
}

void servo_setup()
{
    gpio_enable_port_clock(SERVO_PORT);
    gpio_set_mode(SERVO_PORT, SERVO_PIN, ALTFUNC); // Set PA6 to alternate function
    gpio_set_af(SERVO_PORT, SERVO_PIN, AF2);       // AF-Modus: AF2 -> TIM3_CH1  (Table 9. Alternate function mapping)

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

int32_t clamp(int32_t value, int32_t min, int32_t max)
{
    if (value > max)
    {
        return max;
    }
    else if (value < min)
    {
        return min;
    }
    return value;
}

void EXTI9_5_IRQHandler()
{
    if (exti_is_irq_pending(RE_CLK_PIN))
    {

        __disable_irq();
        if (gpio_read_pin_state(RE_DT_PORT, RE_DT_PIN)) // RE_PIN_B state is HIGH -> clockwise rotation
        {
            duty_cycle += SERVO_STEPS;
        }
        else // RE_PIN_B state is LOW -> counter clockwise rotation
        {
            duty_cycle -= SERVO_STEPS;
        }
        duty_cycle = clamp(duty_cycle, SERVO_MIN_VAL, SERVO_MAX_VAL);
        timer_set_oc_preload_value(TIM3, TIM_CH1, duty_cycle);

        // Clear the interrupt flag
        exti_reset_pending_bit(RE_CLK_PIN);
        __enable_irq();
    }
}