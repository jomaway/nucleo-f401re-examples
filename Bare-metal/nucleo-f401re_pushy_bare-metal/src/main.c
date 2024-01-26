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


    The Reference Manual says: 

    Boundary address            Bus         Peripheral
    -----------------------     ----        -----------
    0x40023800 - 0x40023BFF     AHB1        RCC (Clock)
    0x40020000 - 0x400203FF     AHB1        GPIOA (I/O)
    0x40020800 - 0x40020BFF     AHB1        GPIOC (I/0)


    !!! VOLATILE: 
    Without volatile, the compiler may optimize your code by assuming that the value of a non-volatile variable or register doesn't change 
    unless your code explicitly modifies it. This optimization could lead to unexpected behavior when dealing with hardware registers. 

    - Preventing Compiler Optimization. Those could lead to unexpected behaviour.
    - Ensuring Fresh Data: Volatile tells the compiler that data may has changed even if the code didn't change it. So it fetches the new data every time.
    - Preventing Register Writes: If you write to hardware registers like BSRR or AHB1ENR the compiler might optimize those away.

*/

#include <stdint.h>

/* Register Addresses */

#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL
#define GPIOC_BASE  0x40020800UL

/* Makro which wraps the memory address in a volatile pointer */
#define _REG(mem_addr) (*(volatile uint32_t *)(mem_addr))

/* Clock */
#define RCC_AHB1ENR     _REG(RCC_BASE + 0x30)

/* GPIO */
#define GPIOA_MODER     _REG(GPIOA_BASE + 0x00)
#define GPIOA_BSRR      _REG(GPIOA_BASE + 0x18)
#define GPIOC_MODER     _REG(GPIOC_BASE + 0x00)
#define GPIOC_IDR       _REG(GPIOC_BASE + 0x10)

int main(void)
{
    /* SETUP */
    /* turn on clock on GPIOA */
    RCC_AHB1ENR |= (1 << 0);
    
    /* set LED (PA5) to output mode */
    GPIOA_MODER  |=  (1 << 10);

    /* turn on clock on GPIOC */
    RCC_AHB1ENR |= (1 << 2);

    /* set USER Button (PC13) as Input */
    GPIOC_MODER &= ~(3U << 26U);

    /* LOOP */
    while(1) {

        /* Check if the button is pressed. ! Button is low active */
        if (!(GPIOC_IDR & (1<<13)))
        {
            /* set HIGH value on pin PA5 */
            GPIOA_BSRR   |=  (1 << (5));
        } else {
            /* set LOW value on pin PA5 */
            GPIOA_BSRR   |=  (1 << (5+16));
        }

    }
}