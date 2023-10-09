/* 
    Using the internal SysTick Timer for a delay
*/

// Define the microcontroller variant we use
#define STM32401xE

/* Includes */
#include <stm32f4xx.h>

/* Function declaration */
static void delay_ms(uint32_t ms);

/* Global variables */
volatile uint32_t msTicks = 0;  // needs to be volatile !


int main()
{
    /* SETUP */

    // This function updates the variable SystemCoreClock (uint32_t) with the System Clock Frequency (Core Clock)
    // This is done by checking the CFGR-Register about HSI, HSE or other PLL clock
    SystemCoreClockUpdate();
    // Initializes the System Timer and its interrupt, and starts the System Tick Timer
    // Clock frequency is given in cycles * 1/s by dividing through 1000 we get cycles * 1/ms 
    SysTick_Config(SystemCoreClock / 1000 ); // SysTick = 1ms

    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set PA5 to output mode */
    GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
    GPIOA->MODER |=  GPIO_MODER_MODE5_0;



    /* LOOP */
    while(1)
    {
        GPIOA->BSRR |= GPIO_BSRR_BS_5;
        delay_ms(500);
        GPIOA->BSRR |= GPIO_BSRR_BR_5;
        delay_ms(500);
    }
}

/* a more accurate delay method - but still blocking! */
static void delay_ms(uint32_t ms)
{
    // Check if currentTicks < targetTicks. Not correct on overflow.
    uint32_t targetTicks = msTicks + ms;
    while(msTicks < targetTicks)
    {
        // do nothing
        __asm volatile ("nop");
    }
}


/* 
*   Interrupt-Service-Routine des SysTick-Timers
*/
void SysTick_Handler(void)
{
    ++msTicks;
}