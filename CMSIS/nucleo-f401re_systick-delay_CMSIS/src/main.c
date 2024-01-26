/*
    Nucleo F401RE systick delay (CMSIS version)
    Using the internal SysTick Timer for a delay
*/

/* Includes */
#include <stm32f4xx.h>

/* Function declaration */
static void delay_ms(uint32_t ms);

/* Global variables */
volatile uint32_t sysTick_delay_counter = 0; // needs to be volatile !

void gpio_setup()
{
    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    /* set pin for buildin led (PA5) as output*/
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
}

void systick_setup()
{
    // This function updates the variable SystemCoreClock (uint32_t) with the System Clock Frequency (Core Clock)
    // This is done by checking the CFGR-Register about HSI, HSE or other PLL clock
    SystemCoreClockUpdate();
    // Initializes the System Timer and its interrupt, and starts the System Tick Timer
    // Clock frequency is given in cycles * 1/s by dividing through 1000 we get cycles * 1/ms
    SysTick_Config(SystemCoreClock / 1000); // SysTick = 1ms
}

int main()
{
    /* SETUP */
    gpio_setup();
    systick_setup();

    /* LOOP */
    while (1)
    {
        // toggle the led
        GPIOA->ODR ^= GPIO_ODR_OD5;
        // wait for 500 ms;
        delay_ms(500);
    }
}

/* a more accurate delay method - but still blocking! */
static void delay_ms(uint32_t ms)
{
    // Check if currentTicks < targetTicks. Not correct on overflow.
    sysTick_delay_counter = ms;
    while (sysTick_delay_counter > 0)
    {
        // do nothing
        __NOP(); // No operation
    }
}

/*
 *   Interrupt-Service-Routine des SysTick-Timers
 */
void SysTick_Handler(void)
{
    if (sysTick_delay_counter > 0)
    {
        sysTick_delay_counter--;
    };
}