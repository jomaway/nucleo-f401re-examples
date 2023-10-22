#include "main.h"

int main()
{
    /* SETUP */
    /* turn on clock on GPIOA */
    gpio_enable_port_clock(GPIOA);  // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set LEDs (PA6, PA7, PA8) to output mode */
    gpio_set_pin_mode(GPIOA, LED_GREEN_PIN, OUTPUT);    // GPIOA->MODER |=  (1 << LED_GREEN_PIN * 2);   // GREEN
    gpio_set_pin_mode(GPIOA,LED_RED_PIN, OUTPUT);       // GPIOA->MODER |=  (1 << LED_YELLOW_PIN * 2);  // YELLOW
    gpio_set_pin_mode(GPIOA,LED_YELLOW_PIN, OUTPUT);    // GPIOA->MODER |=  (1 << LED_RED_PIN * 2);     // RED


    /* set Button (PA9) as Input */
    gpio_set_pin_mode(GPIOA, BTN_PIN, INPUT);           // GPIOA->MODER &= ~(0b11 << BTN_PIN * 2);

    /* Loop */
    while(1)
    {
        /* Check if the button is pressed. ! Button is low active */
        if (LOW == (gpio_read_pin_state(GPIOA, BTN_PIN))) 
        {
            switch_traffic_light();
        }
    }

}



void switch_traffic_light()
{
    //GPIOA->BSRR = (1 << LED_RED_PIN);
    gpio_write_pin_state(GPIOA, LED_RED_PIN, HIGH); 
    delay_ms(2000);
    // GPIOA->BSRR = (1 << LED_YELLOW_PIN);
    gpio_write_pin_state(GPIOA, LED_YELLOW_PIN,HIGH);
    delay_ms(1000);
    // GPIOA->BSRR = (1 << (LED_RED_PIN + 16)) | (1 << (LED_YELLOW_PIN + 16)) | (1 << LED_GREEN_PIN);
    gpio_write_pin_state(GPIOA, LED_RED_PIN, LOW);
    gpio_write_pin_state(GPIOA, LED_YELLOW_PIN, LOW);
    gpio_write_pin_state(GPIOA, LED_GREEN_PIN, HIGH);
    delay_ms(5000);
    // GPIOA->BSRR = (1 << (LED_GREEN_PIN + 16)) | (1 << LED_YELLOW_PIN);
    gpio_write_pin_state(GPIOA, LED_GREEN_PIN, LOW);
    gpio_write_pin_state(GPIOA, LED_YELLOW_PIN, HIGH);
    delay_ms(1000);
    // GPIOA->BSRR = (1 << (LED_YELLOW_PIN + 16)) | (1 << LED_RED_PIN);
    gpio_write_pin_state(GPIOA, LED_YELLOW_PIN, LOW);
    gpio_write_pin_state(GPIOA, LED_RED_PIN, HIGH);
    delay_ms(3000);
    // GPIOA->BSRR = (1 << (LED_RED_PIN + 16));
    gpio_write_pin_state(GPIOA, LED_RED_PIN, LOW);
}


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