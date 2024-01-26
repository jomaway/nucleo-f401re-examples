/*
    nucleo-f401re exti rotary encoder example (CMSIS version).

GPIO SETUP:
    Part of the GPIO port mode register (GPIOx_MODER) taken from the STM32F401RE reference manual.
            +---------------------+---------------------+---------------------+
        ... |        PIN 7        |        PIN 6        |        PIN 5        | ...
            +----------+----------+----------+----------+----------+----------+
    BIT     |    15    |    14    |    13    |    12    |    11    |    10    |
            +----------+----------+----------+----------+----------+----------+
    NAME    | MODER7_1 | MODER7_0 | MODER6_1 | MODER6_0 | MODER5_1 | MODER5_0 |
            +----------+----------+----------+----------+----------+----------+
    VALUE   |    0     |     0    |     0    |     0    |     0    |     1    |
            +----------+----------+----------+----------+----------+----------+
    MODE    |       INPUT         |         INPUT       |       OUTPUT        |
            +---------------------+---------------------+---------------------+
    USE FOR |       RE_PIN B      |        RE PIN B     |     BOARD  LED      |
            +---------------------+---------------------+---------------------+
*/
#include <stm32f4xx.h>

#define LED_OFF 0 // Pin is LOW
#define LED_ON 1  // Pin is HIGH

volatile uint32_t led_state = LED_OFF;

void gpio_setup()
{
    // LED SETUP
    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set PA5 to output mode */
    GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk; // (3 << 10)
    GPIOA->MODER |= GPIO_MODER_MODER5_0;    // (1 << 10)

    // ROTARY ENCODER SETUP
    /* set PA6 (RE_PIN_A) as INPUT */
    GPIOA->MODER &= ~GPIO_MODER_MODER6_Msk; // (3 << 12)
    /* set PA8 (RE_PIN_B) as INPUT */
    GPIOA->MODER &= ~GPIO_MODER_MODER7_Msk; // (3 << 14)
    /* set PA9 (RE_PUSH_BUTTON) as INPUT */
    GPIOA->MODER &= ~GPIO_MODER_MODER9_Msk; // (3 << 18)
}

void exti_setup()
{
    /* Turn interrupt on */
    /* Enable the clock for SYSCFG */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /* connect exti lines to ports */
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA; // Set EXTI6 line to Port A
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PA; // Set EXTI6 line to Port B

    /* Enable EXTI line 6 (PA6) - rotary A */
    EXTI->IMR |= EXTI_IMR_MR6;   // Enable EXTI interrupt
    EXTI->FTSR |= EXTI_FTSR_TR6; // Enable falling edge trigger

    /* Enable EXTI line 9 (PA9) - rotary push button */
    EXTI->IMR |= EXTI_IMR_MR9;   // Enable EXTI interrupt
    EXTI->RTSR |= EXTI_RTSR_TR9; // Enable rising edge trigger

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
        if (LED_OFF == led_state)
        {
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
        }
        else // LED_ON
        {
            GPIOA->BSRR |= GPIO_BSRR_BS_5;
        }
    }
}

void EXTI9_5_IRQHandler(void)
{
    /* check if the EXTI Line 6 interrupt flag is set (RE_PIN_A rising edge) */
    if (EXTI->PR & EXTI_PR_PR6) // RE_PIN_A triggered
    {
        __disable_irq();
        if (GPIOA->IDR & GPIO_IDR_ID7) // RE_PIN_B state is HIGH -> clockwise rotation
        {
            led_state = LED_ON;
        }
        else // RE_PIN_B state is LOW -> counter clockwise rotation
        {
            led_state = LED_OFF;
        }

        // Clear the interrupt flag
        EXTI->PR |= EXTI_PR_PR6;
        __enable_irq();
    }
    else if (EXTI->PR & EXTI_PR_PR9) // ROTARY PUSH BUTTON trigger
    {
        led_state = !led_state;
        EXTI->PR |= EXTI_PR_PR9;
    }
}