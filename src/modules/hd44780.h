/*
 * hd44780.h
 *
 * Created: 28.05.2015 19:44:30
 *  Author: Artyom
 */ 


#ifndef HD44780_H_
#define HD44780_H_

#include "PTR.h"
#include <stdint.h>
#include "core/wires.h"

#define RIGHT	'>'
#define LEFT	'<'

// Команды HD44780
#define HD44780_CMD_SCREEN_CLEAR				0b00000001
#define HD44780_CMD_SCREEN_SCROLL_RIGHT			0b00011100
#define HD44780_CMD_SCREEN_SCROLL_LEFT			0b00011000
#define HD44780_CMD_CURSOR_RETURN				0b00000010
#define HD44780_CMD_CURSOR_SHIFT_RIGHT			0b00010100
#define HD44780_CMD_CURSOR_SHIFT_LEFT			0b00010000
// Команды-макросы HD44780
#define HD44780_CMD_CURSOR_MOVE_TO_DISP_L1		0b10000000
#define HD44780_CMD_CURSOR_MOVE_TO_DISP_L2		0b11000000
#define HD44780_CMD_CURSOR_MOVE_TO_CGRAM		0b01000000

#define HD44780_CGRAM							5	// used in "hd44780_goto(HD44780_CGRAM, address)" function
#define HD44780_DRAW_CHAR_SIZE					8	// 8 or 11 | ???

// Настройка дисплея
#define HD44780_DISPCONF_4BIT_2L_5X8			0x2C	// Use 4-bit interface, 2 Lines, 5x8 pixel resolution | 0x02
#define HD44780_DISPCONF_4BIT_1L_5X10			0x24	// Use 4-bit interface, 1 Line, 5x10 pixel resolution
#define HD44780_DISPCONF_DEFAULT				HD44780_DISPCONF_4BIT_2L_5X8

// Режим ввода
#define HD44780_ENTRY_MODE_DEC_NO_SHIFT			0x04	// справа на лево, без сдвига видимой части DDRAM памяти
#define HD44780_ENTRY_MODE_DEC_WITH_SHIFT		0x05	// справа на лево, со сдвигом видимой части DDRAM памяти
#define HD44780_ENTRY_MODE_INC_NO_SHIFT			0x06	// слева на право, без сдвига видимой части DDRAM памяти
#define HD44780_ENTRY_MODE_INC_WITH_SHIFT		0x07	// слева на право, со сдвигом видимой части DDRAM памяти
#define HD44780_ENTRY_MODE_DEFAULT				HD44780_ENTRY_MODE_INC_NO_SHIFT

// Управление дисплеем
#define HD44780_DISPCTRL_DISPON_BLKON_CRSON		0x0F	// Display On, Blink On, Cursor On
#define HD44780_DISPCTRL_DISPON_BLKOFF_CRSON	0x0E	// Display On, Blink Off, Cursor On
#define HD44780_DISPCTRL_DISPON_BLKON_CRSOFF	0x0D	// Display On, Blink On, Cursor Off
#define HD44780_DISPCTRL_DISPON_BLKOFF_CRSOFF	0x0C	// Display On, Blink Off, Cursor Off
#define HD44780_DISPCTRL_DISPOFF_BLKOFF_CRSOFF	0x08	// Display Off, Blink Off, Cursor Off
#define HD44780_DISPCTRL_DEFAULT				HD44780_DISPCTRL_DISPON_BLKOFF_CRSOFF

#define HD44780_STROBE_DELAY_US					1000		// Задержка в мкс перед ?началом отправки данных? в LCD

typedef struct {
	wire_t d[8];
	wire_t e;
	wire_t rs;
	wire_t rw;
} hd44780_wire_t;

extern hd44780_wire_t hd44780_wire;

void	hd44780_highbits		(uint8_t data);
void	hd44780_lowbits			(uint8_t data);
void	hd44780_strobe			(void);
void	hd44780_cmd				(uint8_t data);
void	hd44780_configure		(uint8_t param);

void	hd44780_load			(uint8_t *vector, uint8_t position);
void	hd44780_draw_char		(uint8_t* vector, uint8_t position, uint8_t line, uint8_t address);

void	hd44780_print_char		(uint8_t data);
void	hd44780_print_num		(uint32_t val, uint8_t n_digit);
void	hd44780_print_int		(int32_t val);
void	hd44780_print			(const char *c);

void	hd44780_goto			(uint8_t line, uint8_t address);
void	hd44780_clear			(void);
void	hd44780_return			(void);
void	hd44780_backspace		(void);

void	hd44780_cursor_shift	(uint8_t direction);
void	hd44780_screen_scroll	(uint8_t direction);

uint8_t	hd44780_init			(void);


#endif /* HD44780_H_ */