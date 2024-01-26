#include <stm32f4xx.h>

#include <gpio.h> // gpio functions from our mcal lib
#include <delay.h>

/* DEFINES */
#define LEDS_PORT GPIOA  // FOR ALL LEDS
#define LED_RED_PIN 6    // Red led on PA6
#define LED_YELLOW_PIN 7 // Yellow led on PA7
#define LED_GREEN_PIN 8  // Green led on PA8

#define BTN_PORT GPIOA
#define BTN_PIN 9 // Button on PA9

/* Function declaration */
void gpio_setup();
void switch_traffic_light();

int main()
{
    /* SETUP */
    gpio_setup();
    systick_init(1000); // to use delay_ms

    /* Loop */
    while (1)
    {
        /* Check if the button is pressed. ! Button is low active */
        if (LOW == (gpio_read_pin_state(BTN_PORT, BTN_PIN)))
        {
            switch_traffic_light();
        }
    }
}

void gpio_setup()
{
    /* turn on clock on GPIOA */
    gpio_enable_port_clock(GPIOA); // RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* set LEDs (PA6, PA7, PA8) to output mode */
    gpio_set_mode(LEDS_PORT, LED_GREEN_PIN, OUTPUT);  // GPIOA->MODER |=  (1 << LED_GREEN_PIN * 2);   // GREEN
    gpio_set_mode(LEDS_PORT, LED_RED_PIN, OUTPUT);    // GPIOA->MODER |=  (1 << LED_YELLOW_PIN * 2);  // YELLOW
    gpio_set_mode(LEDS_PORT, LED_YELLOW_PIN, OUTPUT); // GPIOA->MODER |=  (1 << LED_RED_PIN * 2);     // RED

    /* set Button (PA9) as Input */
    gpio_set_mode(BTN_PORT, BTN_PIN, INPUT); // GPIOA->MODER &= ~(0b11 << BTN_PIN * 2);
}

void switch_traffic_light()
{
    gpio_write_pin_state(LEDS_PORT, LED_RED_PIN, HIGH);
    systick_delay_ms(2000);
    gpio_write_pin_state(LEDS_PORT, LED_YELLOW_PIN, HIGH);
    systick_delay_ms(1000);
    gpio_write_pin_state(LEDS_PORT, LED_RED_PIN, LOW);
    gpio_write_pin_state(LEDS_PORT, LED_YELLOW_PIN, LOW);
    gpio_write_pin_state(LEDS_PORT, LED_GREEN_PIN, HIGH);
    systick_delay_ms(5000);
    gpio_write_pin_state(LEDS_PORT, LED_GREEN_PIN, LOW);
    gpio_write_pin_state(LEDS_PORT, LED_YELLOW_PIN, HIGH);
    systick_delay_ms(1000);
    gpio_write_pin_state(LEDS_PORT, LED_YELLOW_PIN, LOW);
    gpio_write_pin_state(LEDS_PORT, LED_RED_PIN, HIGH);
    systick_delay_ms(3000);
    gpio_write_pin_state(LEDS_PORT, LED_RED_PIN, LOW);
}
