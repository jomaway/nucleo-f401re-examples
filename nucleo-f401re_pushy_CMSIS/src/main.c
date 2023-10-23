/*
    Nucleo-F401RE Blinky (CMSIS version)

    The Common Microcontroller Software Interface Standard (CMSIS) is a vendor-independent 
    abstraction layer for microcontrollers that are based on Arm Cortex processors. 
    CMSIS defines generic tool interfaces and enables consistent device support. 
    This layer is adopted by leading semiconductor companies, embedded software and tools vendors across the Arm architecture. 
    This means you can find a CMSIS-Pack for your target MCU if the MCU’s vendor is registered with ARM.

    This Program should turn ON the USER LED (PA5) on the nucleo f401re Board, based if the USER BUTTON is pressed or not.

    SETUP:
    1. Activate the clock for gpio portA, by setting the corresponding bit in the RCC_AHB1ENR (RCC AHB1 peripheral clock enable register).
    2. Set pin PA5 as output in the GPIOA_MODER (GPIOA mode register).
    3. Activate the clock for gpio portC by setting the corresponding bit in the RCC_AHB1ENR (RCC AHB1 peripheral clock enable register).
    4. Set pin PC13 as input in the GPIOC_MODER (GPIOC mode register).

    LOOP:
    1. Check if the USER BUTTON (PC13) is pressed. As the Button is low active, bit 13 in the GPIOC_IDR (gpio port input data register) must be 0. 
    2.IF TRUE: Set pin PA5 HIGH to turn LED ON. This is achieve by setting bit 5 in the GPIO_BSRR (GPIO port bit/reset register).
    3.IF FALSE: Set pin PA5 LOW to turn LED OFF. This is achieve by setting bit 21 (5+16) in the GPIO_BSRR (GPIO port bit/reset register).
*/

// Define the microcontroller variant we use
#define STM32F401xE

// include generic header files
#include <stm32f4xx.h>


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

    /* LOOP */
    while(1) {

        /* Check if the button is pressed. ! Button is low active */
        if (!(GPIOC->IDR & GPIO_IDR_ID13)) 
        {
            /* set HIGH value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BS_5;
        } else {
            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
        }

    }
}