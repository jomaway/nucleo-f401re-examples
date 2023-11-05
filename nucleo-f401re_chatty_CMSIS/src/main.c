#include <stm32f401xe.h>

#include <stdlib.h>
#include <gpio.h>
#include <uart.h>

static void delay_ms(uint32_t ms);

int main()
{   // Enable Port A clock.
    gpio_enable_port_clock(GPIOA);
    gpio_set_pin_mode(GPIOA, 5, OUTPUT);
    gpio_write_pin_state(GPIOA,5,HIGH);

    // Enable UART 
    uart_setup(USART2, 115200);
    uart_enable_isr(USART2);

    // Enable global interrupts
    __enable_irq();

    uint64_t ticks = 0;
    uint8_t data = 0;
    
    while(1){
        //uart_write(USART2, "Hallo vom nucf401re. \n", 22);
        printf("LED: %d, tick: %lu\r\n", gpio_read_pin_state(GPIOA, 5), ticks);  // Write message
        delay_ms(1000);
        //GPIOA->ODR ^= (1 << 5);  // Toggle led
        ticks++;


        if(uart_read(USART2,(uint8_t *) &data,1))
        {
            printf("detected new data \n");
            if (data == 'a'){
                gpio_write_pin_state(GPIOA,5,HIGH);
            } 
            else if (data == 'b')
            {
                gpio_write_pin_state(GPIOA,5,LOW);
            }
            else
            {
                uart_write_byte(USART2, ++data);
            }
        }
        printf("Data is: %d \n", data); 
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

