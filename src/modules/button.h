/*
 * button.h
 *
 * Created: 27.05.2015 19:58:49
 *  Author: Artyom
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_

#define BTN_ONCE	1
#define BTN_LONG	1000

#include <stdint.h>

#include "core/wires.h"

extern wire_t btn_null;

extern wire_t btn_start;
extern wire_t btn_reset;

extern wire_t btn_back;
extern wire_t btn_ok;
extern wire_t btn_right;
extern wire_t btn_left;

extern wire_t  *btn_prev;
extern uint16_t btn_cntr;

uint8_t	btn_init	(void);
wire_t*	btn_get		(void);

#endif /* BUTTON_H_ */