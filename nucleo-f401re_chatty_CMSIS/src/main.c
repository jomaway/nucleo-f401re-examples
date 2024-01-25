#include <stm32f401xe.h>

#include <stdio.h>
#include <gpio.h>
#include <uart.h>

static void delay_ms(uint32_t ms);

int main()
{   // Enable Port A clock.
    gpio_enable_port_clock(GPIOA);
    gpio_set_mode(GPIOA, 5, OUTPUT);
    gpio_write_pin_state(GPIOA,5,HIGH);

    // Enable UART 
    uart_setup(USART2, 115200);
    uart_enable_isr(USART2);

    // Enable global interrupts
    __enable_irq();

    uint64_t ticks = 0;
    uint8_t byte = 0;
    
    while(1){
        //uart_write(USART2, "Hallo vom nucf401re. \n", 22);
        printf("LED: %d, tick: %llu\r\n", gpio_read_pin_state(GPIOA, 5), ticks);  // Write message
        delay_ms(1000);
        //GPIOA->ODR ^= (1 << 5);  // Toggle led
        ticks++;


        if(uart_data_available(USART2))
        {
            printf("detected new data \n");
            byte = uart_read_byte(USART2);
            if (byte == 'a'){
                gpio_write_pin_state(GPIOA,5,HIGH);
            } 
            else if (byte == 'b')
            {
                gpio_write_pin_state(GPIOA,5,LOW);
            }
            else
            {
                uart_write_byte(USART2, ++byte);
            }
        }
        printf("Data is: %d \n", byte); 
    }
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

