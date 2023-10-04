/*
    Nucleo-F401RE Blinky (CMSIS version)

    This Program should turn on/off the USER LED (PA5) on the nucleo f401re Board, if the USER BUTTON (PC13) is pressed.

*/

// Define the microcontroller variant we use
#define STM32401xE

// include generic header files
#include <stm32f4xx.h>

#define LED_OFF 0
#define LED_ON 1

#define BTN_PRESSED 0 /* USER BUTTON was pressed */


/* a very bad delay method */
static void delay_ms(uint32_t ms)
{
    /* Running at 16MHz it needs around 16000 cycles to reach 1ms.
        Because each loop takes around 4 cycles we need to devide by 4.
    */
    ms = ms * 16000 / 4;
    for(;ms > 0;ms--)
    {
        __NOP(); // No operation 
    }
}

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
    GPIOC->MODER &= GPIO_MODER_MODER13_Msk;

    /* create some state variables */
    uint32_t btn_state = !BTN_PRESSED;
    uint32_t led_state = LED_OFF;

    /* LOOP */
    while(1) {

        /* Check if the button is pressed. ! Button is low active */
        if (!(GPIOC->IDR & GPIO_IDR_ID13)) 
        {
            btn_state = BTN_PRESSED;
            delay_ms(50);   // debounce button
        } else {
            if (BTN_PRESSED == btn_state) {
                led_state = !led_state;
            }
            btn_state = !BTN_PRESSED;
            delay_ms(50);   // debounce button
        }

        if (LED_ON == led_state)
        {
            /* set HIGH value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BS_5;
        } else {
            /* set LOW value on pin PA5 */
            GPIOA->BSRR |= GPIO_BSRR_BR_5;
        }

    }
}