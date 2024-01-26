/* Nucleo F401RE uart echo example (MCAL version)*/
#include <stm32f401xe.h>
#include <stdio.h>
#include <gpio.h>

#ifdef DEBUG
    #define LOG( msg... ) printf( msg );
#else
    #define LOG( msg... ) ;
#endif

// global variable
#define PUFFER_SIZE 50
volatile char puffer[PUFFER_SIZE];   // Puffer für 50 Zeichen
volatile uint8_t puffer_index = 0; // Index Zähler für den Puffer

void uart2_setup();

int main()
{   // Enable Port A clock.

    // Enable UART 
    uart2_setup();
    char byte = '\0';
    
    LOG("UART2 set up successfully\n");

    while(1){
        // if (USART2->SR & USART_SR_RXNE)
        // {
        //     byte = USART2->DR & 0x1FF;  // nur Bits 8:0 sind mögliche Daten
        //     // transmit the received byte back.
        //     // while (!(USART2->SR & USART_SR_TXE));
        //     // USART2->DR = byte & 0x1FF;  // zur Sicherheit auch hier nur Bits 8:0 schreiben.
        //     LOG("RX: %c\r\n", byte);
        // }
    }
}


void uart_set_baudrate(USART_TypeDef *usart, uint64_t baud)
{
    uint8_t over8_rate = 8 * ( 2 - ( usart->CR1 & USART_CR1_OVER8 ) );
    SystemCoreClockUpdate();  // Update SystemCoreClock
    uint32_t uartdiv = SystemCoreClock / baud;
    uint16_t mantissa = uartdiv / over8_rate;
    uint8_t fraction = uartdiv % over8_rate;
    usart->BRR = ( (mantissa << USART_BRR_DIV_Mantissa_Pos) | fraction ) ;  // For the moment overbit is always 0.
}

void uart2_setup()
{
    // Takt für GPIOA aktivieren
    gpio_enable_port_clock(GPIOA);
    // Set pins PA2 und PA3 als Alternate function mode
    gpio_set_pin_mode(GPIOA, 2, ALTFUNC);
    gpio_set_pin_mode(GPIOA, 3, ALTFUNC);

    // Definiere PA2 und PA3 als AF 7 -> siehe Alternate function mapping Tabelle.
    gpio_set_pin_af(GPIOA, 2, AF7);
    gpio_set_pin_af(GPIOA, 3, AF7);

    // Takt für USART2 aktivieren
    RCC->APB1ENR |=  RCC_APB1ENR_USART2EN;  // (1 << 17)

    // Baudrate auf 115200 festlegen -> in PlatformIO.ini den selben Wert angeben.
    uart_set_baudrate(USART2, 115200);

    // UART2 aktivieren Receive, Transmit und Allgemein.
    USART2->CR1 |= (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);

    // Aktiviere Interrupt zum Empfangen von Bytes.
    USART2->CR1 |= USART_CR1_RXNEIE;
    // ISR im NVIC registrieren.
    NVIC_EnableIRQ(USART2_IRQn);
}


// Hilfsfunktion um 1 Byte über UART2 zu senden.
void uart_write_byte(USART_TypeDef *usart, uint8_t byte)
{
    while(!(usart->SR & USART_SR_TXE)); // warte solange der mc noch nicht bereit zum senden ist. Blocking!
    usart->DR = byte & 0x1FF;
}

int _write(int fd, char *ptr, int len)
{
    uint32_t size = len;
    while(len-- > 0)
    {
        uart_write_byte(USART2, *(ptr++));
    }
    return size;  // return the number of bytes written.
}

// Hilfsfunktion zum lesen von einem byte
uint8_t uart_read_byte(USART_TypeDef *usart)
{
    return (uint8_t) (usart->DR & 0x1FF);
}

void USART2_IRQHandler()
{
    // make sure data is available.
    if (USART2->SR & USART_SR_RXNE)
    {
        char byte = uart_read_byte(USART2);
        // LOG input if enter 0x13 is received.
        puffer[puffer_index++] = byte;

        if (byte == '\n')
        {
            LOG("RX: %s", puffer);
            puffer_index = 0;
            for (int i=0; i<PUFFER_SIZE; i++)
            {
                puffer[i] = '\0';
            }
        }
    }
}

