/*
    Nucleo-F401RE Blinky (CMSIS version)

    This Program should start/stop blinking the USER LED (PA5) on the nucleo f401re Board, if the USER BUTTON (PC13) is pressed.
    But instead of polling the state of the butten this version uses an interrupt to trigger a butten press.

    Therefore we need to do a bit of a setup.
    
    SETUP for the interrupt:
    1. Enable the clock for the system configuration controller (SYSCFG)
    2. Set port c as exti in SYSCFG_EXTICR4 for pin 13.
    3. Enable the interrupt in the interrupt mask register (EXTI_IMR)
    4. Select falling edge as a trigger for pin 13 in the falling trigger selection register (EXTI_FTSR), -> because button is low active.
    5. Enable the exti with the function NVIC_EnableIRQ(EXTI15_10_IRQn);
    6. Write an ISR for the interrupt -> void EXTI15_10_IRQHandler(void)
        a. Check if pin 13 is pending in the pending register (EXTI_PR)
        b. toggle led_state
        c. reset pending bit for pin 13
*/

// Define the microcontroller variant we use
#define STM32F401xE

// include generic header files
#include <stm32f4xx.h>

#define BLINK_OFF 0
#define BLINK_ON 1

/* create a global blink_state variable */
uint32_t blink_state = BLINK_OFF;

/* a very bad delay method */
static void delay_ms(uint32_t ms)
{
    /* Running at 16MHz it needs around 16000 cycles to reach 1ms.
        Because each loop takes around 4 cycles we need to devide by 4.
    */
    ms = ms * 16000 / 4;
    for(;ms > 0;ms--)
    {
        __NOP(); // No operation 
    }
}

int main(void)
{
    /* SETUP */
    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set LED (PA5) to output mode */
    GPIOA->MODER |=  GPIO_MODER_MODE5_0;

    /* turn on clock on GPIOC */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;   

    /* set USER Button (PC13) as Input */
    GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;

    /* Turn interrupt on */
    /* Enable the clock for SYSCFG */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    /* Select gpio port c pin13 as external interrupt */
    SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;    // Clear EXTI13 bits
    SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;  // Set EXTI13 bits to PC

    /* Enable EXTI for line 13 (PC13) */
    EXTI->IMR |= EXTI_IMR_MR13;     // Enable EXTI interrupt
    EXTI->FTSR |= EXTI_FTSR_TR13;   // Enable falling edge trigger

    /* Enable and set the EXTI interrupt in NVIC */
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    NVIC_SetPriority(EXTI15_10_IRQn, 0);

    // Enable global interrupts
    __enable_irq();

    /* LOOP */
    while(1) {


        if (BLINK_ON == blink_state)
        {
            /* set HIGH value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BS_5;
            delay_ms(500);

            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
            delay_ms(500);
        } else {
            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
        }

    }
}

void EXTI15_10_IRQHandler(void)
{
    /* check if the EXTI pin 13 interrupt flag is set (user button) */
    if (EXTI->PR & EXTI_PR_PR13)
    {
        //Handle button press
        blink_state = !blink_state;

        //Clear the interrupt flag
        EXTI->PR |= EXTI_PR_PR13;
    }
}