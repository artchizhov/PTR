/*
 * hd44780.c
 *
 * Created: 28.05.2015 19:44:39
 *  Author: Artyom
 */ 


#include "PTR.h"

#include <stdint.h>
#include <util/delay.h>

#include "hd44780.h"

#include "core/wires.h"
#include "core/delay.h"

hd44780_wire_t hd44780_wire = {
	.d = {
		NOWR,		// D0
		NOWR,		// D1
		NOWR,		// D2
		NOWR,		// D3
		{C, 3},		// D4
		{C, 2},		// D5
		{C, 1},		// D6
		{C, 0}		// D7
	},
	.e  = {C, 4},
	.rs = {C, 5},
	.rw = NOWR
};

/* PUT HIGH BITS */
void hd44780_highbits(uint8_t data) {
	if (data & 0x80) wire_set_high(hd44780_wire.d[7]); else wire_set_low(hd44780_wire.d[7]);
	if (data & 0x40) wire_set_high(hd44780_wire.d[6]); else wire_set_low(hd44780_wire.d[6]);
	if (data & 0x20) wire_set_high(hd44780_wire.d[5]); else wire_set_low(hd44780_wire.d[5]);
	if (data & 0x10) wire_set_high(hd44780_wire.d[4]); else wire_set_low(hd44780_wire.d[4]);
}

/* PUT LOW BITS */
void hd44780_lowbits(uint8_t data) {
	if (data & 0x08) wire_set_high(hd44780_wire.d[7]); else wire_set_low(hd44780_wire.d[7]);
	if (data & 0x04) wire_set_high(hd44780_wire.d[6]); else wire_set_low(hd44780_wire.d[6]);
	if (data & 0x02) wire_set_high(hd44780_wire.d[5]); else wire_set_low(hd44780_wire.d[5]);
	if (data & 0x01) wire_set_high(hd44780_wire.d[4]); else wire_set_low(hd44780_wire.d[4]);
}

/* INITIATE TRANSFER OF DATA/COMMAND TO LCD */
void hd44780_strobe(void) {
	wire_set_high(hd44780_wire.e);
	_delay_us(HD44780_STROBE_DELAY_US);
	
	wire_set_low(hd44780_wire.e);
	_delay_us(HD44780_STROBE_DELAY_US);
}

/* PUT DATA/COMMAND TO LCD */
void hd44780_cmd(uint8_t data) {
	hd44780_highbits(data);
	hd44780_strobe();
	
	hd44780_lowbits(data);
	hd44780_strobe();
}

/* CONFIGURE 4-BIT DISPLAY INTERFACE */
/* CONFIGURE THE DISPLAY */
void hd44780_configure(uint8_t param) {
	hd44780_highbits(param);	// 4-bit, two lines, 5x8 pixel
	hd44780_strobe();			// change 8-bit interface to 4-bit interface
	
	hd44780_strobe();		// init 4-bit interface
	hd44780_lowbits(param);
	hd44780_strobe();
}

/* GO TO SPECIFIED MEMORY ADDRESS */
void hd44780_goto(uint8_t line, uint8_t address) {
	uint8_t addr = 0x00;
	
	switch (line) {
		case 0: addr = 0x00; break;	// Starting address of 1st line
		case 1: addr = 0x40; break;	// Starting address of 2nd line
		case 2: addr = 0x14; break;	// Starting address of 3rd line
		case 3: addr = 0x54; break;	// Starting address of 4th line
		default: ;
	}
	
	addr += address;
	
	hd44780_cmd(0x80 | addr);
}

/* LCD CLEAR SCREEN */
void hd44780_clear(void) {
	hd44780_cmd(HD44780_CMD_SCREEN_CLEAR); // clear screen
}

/* LCD RETURN CURSOR */
void hd44780_return(void) {
	hd44780_cmd(HD44780_CMD_CURSOR_RETURN); // return cursor
}

/* WRITE A CHARACTER TO LCD */
void hd44780_print_char(uint8_t data) {
	wire_set_high(hd44780_wire.rs);
	hd44780_cmd(data);
	wire_set_low(hd44780_wire.rs);
}

/* DISPLAY n-DIGIT INTEGER NUMER */
void hd44780_print_num(uint32_t val, uint8_t n_digit) {
	switch (n_digit) {
		case 8: hd44780_print_char ((val/10000000)    + '0');
		case 7: hd44780_print_char(((val/1000000)%10) + '0');
		case 6: hd44780_print_char(((val/100000)%10)  + '0');
		case 5: hd44780_print_char(((val/10000)%10)   + '0');
		case 4: hd44780_print_char(((val/1000)%10)    + '0');
		case 3: hd44780_print_char(((val/100)%10)     + '0');
		case 2: hd44780_print_char(((val/10)%10)      + '0');
		case 1: hd44780_print_char ((val%10)          + '0');
	}
}

/* DISPLAY A INTEGER NUMER: +/- 2147483647 */
void hd44780_print_int(int32_t val) {
	int32_t i;

	if (val<0) {
		hd44780_print_char('-');
		val = -val;
	}
	for (i=1; (val/i)>9; i*=10) // Видимо, считается количество знаков числа. (i)
	;
	hd44780_print_char(val/i + '0');
	i /= 10;
	while (i) {
		hd44780_print_char((val%(i*10))/i + '0');
		i /= 10;
	}
}

/* WRITE A STRING TO LCD */
void hd44780_print(const char *c) {
	while (*c) {
		wire_set_high(hd44780_wire.rs);
		hd44780_cmd(*c++);
		wire_set_low(hd44780_wire.rs);
	}
}

/* LOAD USER-DEFINED CHARACTER IN CGRAM */
/* USE CGRAM CHAR SPACE: 0 to 7 */
void hd44780_load(uint8_t *vector, uint8_t position) {
	uint8_t i;
	
	hd44780_goto(HD44780_CGRAM, position * HD44780_DRAW_CHAR_SIZE);
	
	wire_set_high(hd44780_wire.rs);
	for (i=0; i<HD44780_DRAW_CHAR_SIZE; i++) {
		hd44780_cmd(vector[i]);
	}
	wire_set_low(hd44780_wire.rs);
}

/* DISPLAY USER-DEFINED CHARACTER ON DDRAM */
/* USE CGRAM CHAR SPACE */
void hd44780_draw_char(uint8_t* vector,
uint8_t position,
uint8_t line,
uint8_t address) {
	hd44780_load(vector, position);
	hd44780_goto(line, address);
	hd44780_print_char(position);
}

/* ERASE LEFT CHAR */
void hd44780_backspace(void) {
	hd44780_cmd(HD44780_CMD_CURSOR_SHIFT_LEFT);
	hd44780_print_char(' ');
	hd44780_cmd(HD44780_CMD_CURSOR_SHIFT_LEFT);
}

/* SHIFT CURSOR TO SPECIFIED DIRECTION */
void hd44780_cursor_shift(uint8_t direction) {
	switch (direction) {
		case RIGHT: hd44780_cmd(HD44780_CMD_CURSOR_SHIFT_RIGHT); break;
		case LEFT : hd44780_cmd(HD44780_CMD_CURSOR_SHIFT_LEFT);  break;
	}
}

/* SCROLL DISPLAY TO SPECIFIED DIRECTION */
void hd44780_screen_scroll(uint8_t direction) {
	switch (direction) {
		case RIGHT: hd44780_cmd(HD44780_CMD_SCREEN_SCROLL_RIGHT); break;
		case LEFT : hd44780_cmd(HD44780_CMD_SCREEN_SCROLL_LEFT);  break;
	}
}

/* INITIALIZE ENTIRE DISPLAY */
uint8_t hd44780_init(void) {
	// Настройка выводов
	wire_set_output(hd44780_wire.rs);
	wire_set_output(hd44780_wire.e);
	for (int i=4; i<8; i++) {
		wire_set_output(hd44780_wire.d[i]);
	}
	
	hd44780_configure(HD44780_DISPCONF_DEFAULT);	// 1, Data Lenght, Number of lines, character font
	
	hd44780_cmd(HD44780_DISPCTRL_DEFAULT);			// 1, lcd, cursor, blink
	hd44780_cmd(HD44780_ENTRY_MODE_DEFAULT);		// 1, increment/decrement, display shift on/off
	
	hd44780_clear();
	
	return TRUE;
}