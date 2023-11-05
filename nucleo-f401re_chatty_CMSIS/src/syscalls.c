#include <uart.h>
#include <stdlib.h>

int _write(int fd, char *ptr, int len)
{
    (void) fd;
    if (fd == 1) uart_write(USART2, (uint8_t *) ptr, (uint32_t) len);
    return -1;
}