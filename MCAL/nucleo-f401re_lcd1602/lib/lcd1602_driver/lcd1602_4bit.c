/*
 * lcd1602_4bit.c
 */

#include "lcd1602_4bit.h"

// -------------------------------------------
// Display init
// -------------------------------------------
void LCD1602_Init(void) {

	gpio_enable_port_clock(LCD1602_E_Port);
	gpio_enable_port_clock(LCD1602_RS_Port);
	gpio_enable_port_clock(LCD1602_RS_Port);
	gpio_enable_port_clock(LCD1602_D4_Port);
	gpio_enable_port_clock(LCD1602_D5_Port);
	gpio_enable_port_clock(LCD1602_D6_Port);
	gpio_enable_port_clock(LCD1602_D7_Port);

	DWT_init();

    gpio_set_mode(LCD1602_E_Port, LCD1602_E_Pin, OUTPUT);   // E
	gpio_set_mode(LCD1602_RS_Port, LCD1602_RS_Pin, OUTPUT);	// RS
	gpio_set_mode(LCD1602_D4_Port, LCD1602_D4_Pin, OUTPUT);	// D4
	gpio_set_speed(LCD1602_D4_Port, LCD1602_D4_Pin, HIGH_SPEED);
	gpio_set_mode(LCD1602_D5_Port, LCD1602_D5_Pin, OUTPUT);	// D5
	gpio_set_speed(LCD1602_D5_Port, LCD1602_D5_Pin, HIGH_SPEED);
	gpio_set_mode(LCD1602_D6_Port, LCD1602_D6_Pin, OUTPUT);	// D6
	gpio_set_speed(LCD1602_D6_Port, LCD1602_D6_Pin, HIGH_SPEED);
	gpio_set_mode(LCD1602_D7_Port, LCD1602_D7_Pin, OUTPUT);	// D7
	gpio_set_speed(LCD1602_D7_Port, LCD1602_D7_Pin, HIGH_SPEED);

	gpio_write_pin_state(LCD1602_E_Port, LCD1602_E_Pin, LOW);	// E  LOW
	gpio_write_pin_state(LCD1602_RS_Port, LCD1602_RS_Pin, LOW);	// RS LOW

	DWT_delay_ms(50);

	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x30);			// 8-bit mode
	LCD1602_SendCommand_8bit(0x20);			// 4-bit mode

	LCD1602_SendCommand(0x2C);				// 4-bit mode, 2 lines, font 5x7
	LCD1602_SendCommand(0x08);				// display off
	LCD1602_Clear();						// display clear
	LCD1602_SendCommand(0x06);				// cursor increment, no shift
	LCD1602_SendCommand(0x0C);				// display on, cursor off, blinking off
}

// -------------------------------------------
// Main send function
// -------------------------------------------
void LCD1602_Send(LCD1602_Data_Type dataType, uint8_t data) {

	gpio_write_pin_state(LCD1602_E_Port, LCD1602_E_Pin, LOW);				// E LOW
	gpio_write_pin_state(LCD1602_RS_Port, LCD1602_RS_Pin, dataType);		// Set RS command or data

	gpio_write_pin_state(LCD1602_D4_Port, LCD1602_D4_Pin, data & 0x10);	// Check and set bit 4
	gpio_write_pin_state(LCD1602_D5_Port, LCD1602_D5_Pin, data & 0x20);	// Check and set bit 5
	gpio_write_pin_state(LCD1602_D6_Port, LCD1602_D6_Pin, data & 0x40);	// Check and set bit 6
	gpio_write_pin_state(LCD1602_D7_Port, LCD1602_D7_Pin, data & 0x80);	// Check and set bit 7

	// Strob
	gpio_write_pin_state(LCD1602_E_Port, LCD1602_E_Pin, HIGH);
	DWT_delay_us(500);
	gpio_write_pin_state(LCD1602_E_Port, LCD1602_E_Pin, LOW);
}

// -------------------------------------------
// Send command in 4-bit mode
// -------------------------------------------
void LCD1602_SendCommand(uint8_t cmd) {

	LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
	DWT_delay_ms(1);
	LCD1602_Send(LCD1602_COMMAND, (cmd << 4) & 0xF0);
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send command in 8-bit mode
// -------------------------------------------
void LCD1602_SendCommand_8bit(uint8_t cmd) {

	LCD1602_Send(LCD1602_COMMAND, cmd & 0xF0);
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send char
// -------------------------------------------
void LCD1602_SendChar(char chr) {

	LCD1602_Send(LCD1602_DATA, (uint8_t)(chr & 0xF0));
	DWT_delay_ms(1);
	LCD1602_Send(LCD1602_DATA, (uint8_t)((chr << 4) & 0xF0));
	DWT_delay_ms(1);
}

// -------------------------------------------
// Send string
// -------------------------------------------
void LCD1602_SendString(char *str) {

	while (*str) {
		LCD1602_SendChar(*str++);
	}
}

// -------------------------------------------
// Clear display
// -------------------------------------------
void LCD1602_Clear(void) {

	LCD1602_SendCommand(0x01);		// display clear
	DWT_delay_ms(2);
}

// -------------------------------------------
// Set cursor position
// -------------------------------------------
void LCD1602_CursorPos(uint8_t x, uint8_t y) {

	uint8_t addr = 0;

	switch(x) {
		case 0: addr = 0x00; break;
		case 1: addr = 0x40; break;
		case 2: addr = 0x14; break;		// 2004
		case 3: addr = 0x54; break;		// 2004
	}

	addr += y;

	LCD1602_SendCommand(0x80 | addr);
}

// -------------------------------------------
// Create custom char
// -------------------------------------------
void LCD1602_CreateChar(uint8_t *chr, uint8_t addr) {

	LCD1602_SendCommand(0x40 + (addr * 8));

	for (uint8_t i = 0; i < 8; ++i) {
		LCD1602_SendChar(chr[i]);
	}
}

#ifdef LCD1602_LED_Port && LCD1602_LED_Pin
void LCD1602_Led(LCD1602_Led_Type led_state)
{
    gpio_write_pin_state(LCD1602_LED_Port, LCD1602_LED_Pin, led_state);
}
#endif
