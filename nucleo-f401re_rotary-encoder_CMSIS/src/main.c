
#include <stm32f4xx.h>
#include <gpio.h>

#define ROTARY_ENCODER_A PIN_6  // PA6
#define ROTARY_ENCODER_B PIN_7  // PA7
#define ROTARY_ENCODER_PUSH PIN_9 // PA9

#define LED_PIN PIN_5 //PA5
#define LED_OFF LOW
#define LED_ON HIGH

volatile uint32_t led_state = LED_OFF;

int main()
{
    // LED SETUP
    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, LED_PIN, OUTPUT);

    // ROTARY ENCODER SETUP
    gpio_set_pin_mode(GPIOA, ROTARY_ENCODER_A, INPUT);
    gpio_set_pin_mode(GPIOA, ROTARY_ENCODER_B, INPUT);
    gpio_set_pull_type(GPIOA, ROTARY_ENCODER_A, PULLUP);
    gpio_set_pull_type(GPIOA, ROTARY_ENCODER_B, PULLUP);
    gpio_set_pin_mode(GPIOB, ROTARY_ENCODER_PUSH, INPUT);
    gpio_set_pull_type(GPIOB, ROTARY_ENCODER_PUSH, PULLUP);
    /* Turn interrupt on */
    /* Enable the clock for SYSCFG */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI6_PA;  // Set EXTI6 bits to Port A
    SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI9_PA;  // Set EXTI6 bits to Port B
    
    /* Enable EXTI for line 6 (PA6) - rotary A */
    EXTI->IMR |= EXTI_IMR_MR6;     // Enable EXTI interrupt
    EXTI->FTSR |= EXTI_FTSR_TR6;   // Enable falling edge trigger

    /* Enable EXTI for line 9 (PA9) - rotary push  */
    EXTI->IMR |= EXTI_IMR_MR9;     // Enable EXTI interrupt
    EXTI->RTSR |= EXTI_RTSR_TR9;   // Enable rising edge trigger

    /* Enable and set the EXTI interrupt in NVIC */
    NVIC_EnableIRQ(EXTI9_5_IRQn);
    NVIC_SetPriority(EXTI9_5_IRQn, 0);

    while(1)
    {
        gpio_write_pin_state(GPIOA,LED_PIN, led_state);
    }

}

void EXTI9_5_IRQHandler(void)
{
    /* check if the EXTI pin 13 interrupt flag is set (user button) */
    if (EXTI->PR & EXTI_PR_PR6) // ROTARY A trigger
    {
        __disable_irq();
        if (GPIOA->IDR & GPIO_IDR_ID7)
        {
            led_state = LED_ON;
        }
        else
        {
            led_state = LED_OFF;
        }

        //Clear the interrupt flag
        EXTI->PR |= EXTI_PR_PR6;
        __enable_irq();
    }
    else if (EXTI->PR & EXTI_PR_PR9)  // ROTARY PUSH trigger
    {
        led_state = !led_state;
        EXTI->PR |= EXTI_PR_PR9;
    }
}