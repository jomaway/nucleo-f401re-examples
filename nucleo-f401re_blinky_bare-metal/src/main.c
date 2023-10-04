/*
    Nucleo-F401RE Blinky (Bare Metal version)

    This Program should blink the USER LED (PA5) on the nucleo f401re Board.

    Setup:
    1. Activate the clock for gpio portA, by setting the corresponding bit in the RCC_AHB1ENR (RCC AHB1 peripheral clock enable register).
    2. Set pin PA5 as output in the GPIOA_MODER (GPIOA mode register).

    LOOP:
    1. Set pin PA5 HIGH to turn LED ON. This is achieve by setting bit 5 in the GPIO_BSRR (GPIO port bit/reset register).
    2. wait a second
    3. Set pin PA5 LOW to turn LED OFF. This is achieve by setting bit 21 (5+16) in the GPIO_BSRR (GPIO port bit/reset register).
    4. wait a second

 
    The Reference Manual says: 

    Boundary address            Bus         Peripheral
    -----------------------     ----        -----------
    0x40023800 - 0x40023BFF     AHB1        RCC (Clock)
    0x40020000 - 0x400203FF     AHB1        GPIOA (I/O)

    CLock Settings
*/

#include <stdint.h>

/* Register Addresses */

#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL

/* Clock */
#define RCC_AHB1ENR     RCC_BASE + 0x30

/* GPIO */
#define GPIOA_MODER     GPIOA_BASE + 0x00
#define GPIOA_BSRR      GPIOA_BASE + 0x18


/* a very bad delay method */
static void delay_ms(uint32_t ms)
{
    /* Running at 16MHz it needs around 16000 cycles to reach 1ms.
        Because each loop takes around 4 cycles we need to devide by 4.
    */
    ms = ms * 16000 / 4;
    for(;ms > 0;ms--)
    {
        // do nothing
        __asm volatile ("nop");
    }
}


int main(void)
{
    /* turn on clock on gpioA 
       -> set bit GPIOAEN to 1 in the RCC_AHB1ENR */ 
    *(uint32_t *)(RCC_AHB1ENR) |= (1 << 0);

    /* set PA5 to output mode */
    //*(uint32_t *)(GPIOA_MODER)  &= ~(1 << 11);
    *(uint32_t *)(GPIOA_MODER)  |=  (1 << 10);

    while(1)
    {
        /* set HIGH value on pin PA5 -> LED ON */
        *(uint32_t *)(GPIOA_BSRR)   |=  (1 << (5   ));
        delay_ms(500);

        /* set LOW value on pin PA5 -> LED OFF */
        *(uint32_t *)(GPIOA_BSRR)   |=  (1 << (5+16));
        delay_ms(500);
    }
}

