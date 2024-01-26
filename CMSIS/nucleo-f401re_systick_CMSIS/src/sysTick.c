#include "sysTick.h"

/* Global variables */
volatile uint32_t systick_msTicks; // needs to be volatile !

/// @brief setup systick timer to tick at a
/// @param divider devides the SystemCoreClock by the given number.
void systick_setup(uint16_t divider)
{
    // This function updates the variable SystemCoreClock (uint32_t) with the System Clock Frequency (Core Clock)
    // This is done by checking the CFGR-Register about HSI, HSE or other PLL clock
    SystemCoreClockUpdate();
    // Initializes the System Timer and its interrupt, and starts the System Tick Timer
    // Clock frequency is given in cycles * 1/s by dividing through 1000 we get cycles * 1/ms
    SysTick_Config(SystemCoreClock / divider); // SysTick = 1ms
    systick_msTicks = 0;
}

/// @brief starting a timer.
/// @param timer pointer to a uint32_t value which holds the current timer count value.
/// @param delay time in milliseconds.
void systick_start_timer(uint32_t *timer, uint16_t delay)
{
    *timer = systick_msTicks + delay;
}

bool systick_is_timer_expired(uint32_t *timer)
{
    return systick_msTicks > *timer;
}

void delay_ms(uint32_t ms)
{
    // Check if currentTicks < targetTicks. Not correct on overflow.
    uint32_t targetTicks = systick_msTicks + ms;
    while (systick_msTicks < targetTicks)
    {
        __NOP(); // do nothing
    }
}

/// @brief Interrupt-Service-Routine (ISR) des SysTick-Timers
void SysTick_Handler(void)
{
    ++systick_msTicks;
}