#include "sysTick.h"





void systick_init(uint16_t divider)
{
    // This function updates the variable SystemCoreClock (uint32_t) with the System Clock Frequency (Core Clock)
    // This is done by checking the CFGR-Register about HSI, HSE or other PLL clock
    SystemCoreClockUpdate();
    // Initializes the System Timer and its interrupt, and starts the System Tick Timer
    // Clock frequency is given in cycles * 1/s by dividing through 1000 we get cycles * 1/ms 
    SysTick_Config(SystemCoreClock / divider ); // SysTick = 1ms
    msTicks = 0;
}

void systick_set_timer(uint32_t *timer, uint16_t delay)
{
    *timer = msTicks + delay;
}

bool systick_is_timer_expired(uint32_t *timer)
{
    return msTicks > *timer;
}

/* 
*   Interrupt-Service-Routine des SysTick-Timers
*/
void SysTick_Handler(void)
{
    ++msTicks;
}