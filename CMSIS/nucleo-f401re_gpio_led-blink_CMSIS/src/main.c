/*
    Nucleo-F401RE gpio led blink (CMSIS version)

    The Common Microcontroller Software Interface Standard (CMSIS) is a vendor-independent
    abstraction layer for microcontrollers that are based on Arm Cortex processors.
    CMSIS defines generic tool interfaces and enables consistent device support.
    This layer is adopted by leading semiconductor companies, embedded software and tools vendors across the Arm architecture.
    This means you can find a CMSIS-Pack for your target MCU if the MCU’s vendor is registered with ARM.

    This Program should blink the USER LED (PA5) on the nucleo f401re Board.

    Setup:
    1. Activate the clock for gpio portA, by setting the corresponding bit in the RCC_AHB1ENR (RCC AHB1 peripheral clock enable register).
    2. Set pin PA5 as output in the GPIOA_MODER (GPIOA mode register).

    LOOP:
    1. Set pin PA5 HIGH to turn LED ON. This is achieve by setting bit 5 in the GPIO_BSRR (GPIO port bit/reset register).
    2. wait a second
    3. Set pin PA5 LOW to turn LED OFF. This is achieve by setting bit 21 (5+16) in the GPIO_BSRR (GPIO port bit/reset register).
    4. wait a second
*/
#include <stm32f4xx.h>

/* a very bad delay method */
static void delay_ms(uint32_t ms)
{
    /* Running at 16MHz it needs around 16000 cycles to reach 1ms.
        Because each loop takes around 4 cycles we need to devide by 4.
    */
    ms = ms * 16000 / 4;
    for (; ms > 0; ms--)
    {
        __NOP(); // No operation
    }
}

int main(void)
{
    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set PA5 to output mode */
    GPIOA->MODER &= ~GPIO_MODER_MODER5_1;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    while (1)
    {
        /* set HIGH value on pin PA5 */
        GPIOA->BSRR |= GPIO_BSRR_BS_5;
        delay_ms(1000);

        /* set LOW value on pin PA5 */
        GPIOA->BSRR |= GPIO_BSRR_BR_5;
        delay_ms(1000);
    }
}
