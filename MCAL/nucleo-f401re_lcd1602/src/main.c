/* Includes */
#include <stm32f4xx.h>
#include <stdio.h>
#include <lcd1602_4bit.h>


int main()
{

    char lcd_buffer[50];

    LCD1602_Init();
    
    LCD1602_CursorPos(0, 0);
    LCD1602_SendString("Welcome!");
    LCD1602_CursorPos(1, 0);
    LCD1602_SendString("to MCR");

    LCD1602_CursorPos(0, 0);
    sprintf(lcd_buffer, "%.2f", 123.45f);
    LCD1602_SendString(lcd_buffer);

    while(1)
    {
        
    }
}