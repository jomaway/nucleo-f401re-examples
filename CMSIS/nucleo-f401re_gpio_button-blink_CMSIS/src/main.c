/*
    Nucleo-F401RE gpio button blink led (CMSIS version)

    This Program should start/stop blinking the USER LED (PA5) on the nucleo f401re Board, if the USER BUTTON (PC13) is pressed.

*/

// include generic header files
#include <stm32f4xx.h>

#define BLINK_OFF 0
#define BLINK_ON 1

#define BTN_PRESSED 0 /* USER BUTTON was pressed */

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

void gpio_setup()
{
    /* turn on clock on GPIOA */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set LED (PA5) to output mode */
    GPIOA->MODER |= GPIO_MODER_MODER5_0;

    /* turn on clock on GPIOC */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    /* set USER Button (PC13) as Input */
    GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;
}

int main(void)
{
    /* SETUP */
    gpio_setup();

    /* create some state variables */
    uint32_t btn_state = !BTN_PRESSED;
    uint32_t blink_state = BLINK_OFF;

    /* LOOP */
    while (1)
    {

        /* Check if the button is pressed. ! Button is low active */
        if (!(GPIOC->IDR & GPIO_IDR_ID13))
        {
            btn_state = BTN_PRESSED;
            delay_ms(50); // debounce button
        }
        else
        {
            if (BTN_PRESSED == btn_state)
            {
                blink_state = !blink_state;
            }
            btn_state = !BTN_PRESSED;
            delay_ms(50); // debounce button
        }

        if (BLINK_ON == blink_state)
        {
            /* set HIGH value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BS_5;
            delay_ms(500);

            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
            delay_ms(500);
        }
        else
        {
            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
        }
    }
}