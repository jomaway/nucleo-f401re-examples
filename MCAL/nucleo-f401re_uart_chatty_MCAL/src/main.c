#include <stm32f401xe.h>

#include <stdio.h>
#include <gpio.h>
#include <uart.h>
#include <delay.h>

#define LED_PORT GPIOA
#define LED_PIN 5
#define BTN_PORT GPIOC
#define BTN_PIN 13

void gpio_setup()
{
    gpio_enable_port_clock(SERVO_PORT);
    gpio_set_mode(SERVO_PORT, SERVO_PIN, OUTPUT);

    gpio_enable_port_clock(BTN_PORT);
    gpio_set_mode(BTN_PORT, BTN_PIN, INPUT);
    gpio_set_pull_type(BTN_PORT, BTN_PIN, PULLUP);
}

int main()
{
    __disable_irq();
    gpio_setup();
    uart_setup(USART2, 115200);
    systick_init();
    __enable_irq();

    uint32_t ticks = 0;
    printf("Setup ready. \n");
    printf("LED state: %d \n", gpio_read_pin_state(SERVO_PORT, SERVO_PIN));
    while (1)
    {
        // systick_delay_ms(100);
        // printf("Tick: %lu \n", ticks);

        if (LOW == gpio_read_pin_state(BTN_PORT, BTN_PIN))
        {
            printf("BTN pressed! \n");
            gpio_write_pin_state(SERVO_PORT, SERVO_PIN, LOW);
            printf("LED state: %d \n", gpio_read_pin_state(SERVO_PORT, SERVO_PIN));
        }

        if (systick_is_timer_expired(&ticks))
        {
            systick_set_timer(&ticks, 3000);
            printf("Reset LED. \n");
            gpio_write_pin_state(SERVO_PORT, SERVO_PIN, HIGH);
            printf("LED state: %d \n", gpio_read_pin_state(SERVO_PORT, SERVO_PIN));
        }
    }
}
