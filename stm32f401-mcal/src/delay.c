#include "delay.h"

#define MS_COEFF (SystemCoreClock / 1000)
#define US_COEFF (SystemCoreClock / 1000000)

volatile uint32_t sysTick_delay_counter;  // needs to be volatile !

// -----------------------------------------------
// SysTick Delay (only ms)
// -----------------------------------------------
void systick_init()
{
    // This function updates the variable SystemCoreClock (uint32_t) with the System Clock Frequency (Core Clock)
    // This is done by checking the CFGR-Register about HSI, HSE or other PLL clock
    SystemCoreClockUpdate();
    // Initializes the System Timer and its interrupt, and starts the System Tick Timer
    // Clock frequency is given in cycles * 1/s by dividing through 1000 we get cycles * 1/ms 
    SysTick_Config(SystemCoreClock / 1000 ); // SysTick = 1ms
    sysTick_delay_counter = 0;
}


/*
    Blocking delay_ms function using the SysTick timer.
    Need to call systick_init() once to work correctly.
*/
void delay_ms(uint32_t ms)
{
    sysTick_delay_counter = ms;
    while(sysTick_delay_counter > 0)
    {
        __NOP();         // do nothing
    }
}

/* 
*   Interrupt-Service-Routine des SysTick-Timers
*/
void SysTick_Handler(void)
{
    if (sysTick_delay_counter > 0) { sysTick_delay_counter--; };
}

// -----------------------------------------------
// DWT Init
// -----------------------------------------------
void DWT_init(){
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;		// Enable TRC
	DWT->CYCCNT = 0;									// Reset counter
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;				// Enable counter

	SystemCoreClockUpdate();							// Update SystemCoreClock variable
}

// -----------------------------------------------
// DWT Get Ticks
// -----------------------------------------------
uint32_t DWT_tick() {
	return DWT->CYCCNT;
}

// -----------------------------------------------
// DWT Get Milliseconds
// -----------------------------------------------
uint32_t DWT_millis(void) {
	return DWT_tick() / MS_COEFF;						// tick to ms
}

// -----------------------------------------------
// DWT Get Microseconds
// -----------------------------------------------
uint32_t DWT_micros(void) {
	return DWT_tick() / US_COEFF;						// tick to us
}

// -----------------------------------------------
// DWT Blocking Delay (ms)
// -----------------------------------------------
void DWT_delay_ms(uint32_t delay){
	delay *= MS_COEFF;									// ms to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Blocking Delay (us)
// -----------------------------------------------
void DWT_delay_us(uint32_t delay){
	delay *= US_COEFF;									// us to tick
	uint32_t t0 = DWT_tick();
	while((DWT_tick() - t0) < delay);
}

// -----------------------------------------------
// DWT Non Blocking Timeout (ticks)
// -----------------------------------------------
uint8_t DWT_nb_timeout(Delay_TypeDef *dt) {
	return ((DWT_tick() - dt->timestamp) > dt->delay);
}

// -----------------------------------------------
// DWT Non Blocking Delay (ms)
// -----------------------------------------------
void DWT_nb_delay_ms(Delay_TypeDef *dt, uint32_t delay) {
	dt->delay = delay * MS_COEFF;						// ms to tick
	dt->timestamp = DWT_tick();
}

// -----------------------------------------------
// DWT Non Blocking Delay (us)
// -----------------------------------------------
void DWT_nb_delay_us(Delay_TypeDef *dt, uint32_t delay) {
	dt->delay = delay * US_COEFF;						// us to tick
	dt->timestamp = DWT_tick();
}