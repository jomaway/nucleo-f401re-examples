/*
    nucleo-f401re exti rotary encoder example (MCAL version).
*/
#include <stm32f4xx.h>
#include <gpio.h>
#include <exti.h>

#define LED_PORT GPIOA
#define LED_PIN 5

#define RE_PORT GPIOA // For all three pins.
#define RE_PIN_A 6    // PA6
#define RE_PIN_B 7    // PA7
#define RE_PIN_BTN 9  // PA9

#define LED_OFF 0 // Pin is LOW
#define LED_ON 1  // Pin is HIGH

volatile uint32_t led_state = LED_OFF;

void gpio_setup()
{
    // LED SETUP
    /* turn on clock on GPIOA */
    gpio_enable_port_clock(GPIOA);

    /* set PA5 to output mode */
    gpio_set_mode(SERVO_PORT, SERVO_PIN, OUTPUT);

    // ROTARY ENCODER SETUP
    /* set PA6 (RE_PIN_A) as INPUT */
    gpio_set_mode(RE_PORT, RE_PIN_A, INPUT);

    /* set PA8 (RE_PIN_B) as INPUT */
    gpio_set_mode(RE_PORT, RE_PIN_B, INPUT);

    /* set PA9 (RE_PUSH_BUTTON) as INPUT */
    gpio_set_mode(RE_PORT, RE_PIN_BTN, INPUT);
}

void exti_setup()
{
    /* Turn interrupt on */
    /* Enable the clock for SYSCFG */
    exti_enable_syscfg_clock();

    /* connect exti lines to ports */
    exti_set_irq_input_source(EXTI_PIN6, GPIOA); // Set EXTI6 line to Port A
    exti_set_irq_input_source(EXTI_PIN9, GPIOA); // Set EXTI9 line to Port B

    /* Enable EXTI line 6 (PA6) - rotary A */
    exti_enable_irq(EXTI_PIN6);                     // Enable EXTI interrupt
    exti_set_trigger_edge(EXTI_PIN6, FALLING_EDGE); // Enable falling edge trigger

    /* Enable EXTI line 9 (PA9) - rotary push button */
    exti_enable_irq(EXTI_PIN9);                    // Enable EXTI interrupt
    exti_set_trigger_edge(EXTI_PIN9, RISING_EDGE); // Enable rising edge trigger

    /* Enable and set the EXTI interrupt in NVIC */
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 0);
}

int main()
{
    gpio_setup();
    exti_setup();

    while (1)
    {
        gpio_write_pin_state(SERVO_PORT, SERVO_PIN, led_state);
    }
}

void EXTI9_5_IRQHandler(void)
{
    /* check if the EXTI Line 6 interrupt flag is set (RE_PIN_A rising edge) */
    if (exti_is_irq_pending(EXTI_PIN6)) // RE_PIN_A triggered
    {
        __disable_irq();
        if (gpio_read_pin_state(RE_PORT, RE_PIN_B)) // RE_PIN_B state is HIGH -> clockwise rotation
        {
            led_state = LED_ON;
        }
        else // RE_PIN_B state is LOW -> counter clockwise rotation
        {
            led_state = LED_OFF;
        }

        // Clear the interrupt flag
        exti_reset_pending_bit(EXTI_PIN6);
        __enable_irq();
    }
    else if (exti_is_irq_pending(EXTI_PIN9)) // ROTARY PUSH BUTTON trigger
    {
        // Toggle LED state
        led_state = !led_state;

        // Clear the interrupt flag
        exti_reset_pending_bit(EXTI_PIN9);
    }
}