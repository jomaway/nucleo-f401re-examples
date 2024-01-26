#include <uart.h>
#include <stdio.h>

int _write(int fd, char *ptr, int len)
{
    (void)fd;
    if (fd == 1)
    {
        return uart_write(USART2, (uint8_t *)ptr, (uint32_t)len);  
        // return the number of bytes written.
    }
    return -1;
}