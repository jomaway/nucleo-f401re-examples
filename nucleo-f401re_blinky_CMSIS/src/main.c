/*
    Nucleo-F401RE Blinky (CMSIS version)

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

// Define the microcontroller variant we use
#define STM32401xE

#include <stdint.h>
#include <stm32f4xx.h>

/* a very bad delay method */
static void delay_ms(uint32_t ms)
{
    ms = ms * 1600;
    for(;ms > 0;ms--)
    {
        __NOP(); // No operation 
    }
}

int main(void)
{
  /* turn on clock on GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

  /* set PA5 to output mode */
  GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
  GPIOA->MODER |=  GPIO_MODER_MODE5_0;

  while(1) {
      /* set HIGH value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BS_5;
      delay_ms(1000);

      /* set LOW value on pin PA5 */
      GPIOA->BSRR |= GPIO_BSRR_BR_5;
      delay_ms(1000);
    }
}



   