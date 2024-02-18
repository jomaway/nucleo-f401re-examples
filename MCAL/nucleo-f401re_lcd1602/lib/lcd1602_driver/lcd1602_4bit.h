/*
 * lcd1602_4bit.h
 *
 *  Based on https://github.com/gkiryaziev/STM32-CMSIS_Libraries/blob/master/Lib/LCD1602/lcd1602_4bit.h
 */

#ifndef RDF_DRIVERS_LCD1602_H
#define RDF_DRIVERS_LCD1602_H

#include "stm32f4xx.h"
#include <gpio.h>
#include <delay.h>

#ifndef LCD1602_E_Port
    #define LCD1602_E_Port		GPIOA
#endif
#ifndef LCD1602_E_Pin
	#define LCD1602_E_Pin		8
#endif
#ifndef LCD1602_RS_Port
	#define LCD1602_RS_Port		GPIOA
#endif
#ifndef LCD1602_RS_Pin
	#define LCD1602_RS_Pin		9
#endif

#define LCD1602_D4_Port		GPIOA
#define LCD1602_D4_Pin		5
#define LCD1602_D5_Port		GPIOA
#define LCD1602_D5_Pin		6
#define LCD1602_D6_Port		GPIOA
#define LCD1602_D6_Pin		7
#define LCD1602_D7_Port		GPIOB
#define LCD1602_D7_Pin		6

typedef enum {
	LCD1602_COMMAND = 0,
	LCD1602_DATA	= 1
} LCD1602_Data_Type;




void LCD1602_Init(void);
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data);
void LCD1602_SendCommand(uint8_t cmd);
void LCD1602_SendCommand_8bit(uint8_t cmd);
void LCD1602_SendChar(char chr);
void LCD1602_SendString(char *str);
void LCD1602_Clear(void);
void LCD1602_CursorPos(uint8_t x, uint8_t y);
void LCD1602_CreateChar(uint8_t *chr, uint8_t addr);

#ifdef LCD1602_LED_Port && LCD1602_LED_Pin
typedef enum {
	LCD1602_OFF = LOW,
	LCD1602_ON	= HIGH
} LCD1602_Led_Type;

void LCD1602_Led(LCD1602_Led_Type led_state);
#endif


#endif /* RDF_DRIVERS_LCD1602_H */