/*
 * relay.c
 *
 * Created: 27.05.2015 19:58:25
 *  Author: Artyom
 */ 


#include <stdint.h>

#include "core/wires.h"

#include "relay.h"

wire_t rel_load_1	=	{D, 0};
wire_t rel_load_2	=	{D, 1};
wire_t rel_load_3	=	{D, 2};
wire_t rel_load_4	=	{D, 3};
wire_t rel_load_5	=	{D, 4};
wire_t rel_load_6	=	{D, 5};
wire_t rel_sync		=	{D, 6};
wire_t rel_reserved	=	{D, 7};

void rel_set(wire_t w, bool_t state) {
	switch (state) {
		case ON:	wire_set_high(w);	break;
		case OFF:	wire_set_low(w);	break;
	}
}

// Включение реле
void rel_on(wire_t w) {
	wire_set_high(w);
}

// Выключение реле
void rel_off(wire_t w) {
	wire_set_low(w);
}

// Переключение состояния реле (ВКЛ->ВЫКЛ, ВЫКЛ->ВКЛ, ...)
void rel_toggle(wire_t w) {
	wire_toggle(w);
}

// Проверка состояния реле
uint8_t rel_check(wire_t w) {
	if (wire_check(w)) {
		return ON;
	} else {
		return OFF;
	}
}

uint8_t rel_init(void) {
	// Подаем 0 на пины реле, это предварительно ВЫКЛЮЧАЕТ реле.
	wire_set_low(rel_load_1);
	wire_set_low(rel_load_2);
	wire_set_low(rel_load_3);
	wire_set_low(rel_load_4);
	wire_set_low(rel_load_5);
	wire_set_low(rel_load_6);
	wire_set_low(rel_sync);
	wire_set_low(rel_reserved);
	
	// Пины как выходы для управления реле.
	wire_set_output(rel_load_1);
	wire_set_output(rel_load_2);
	wire_set_output(rel_load_3);
	wire_set_output(rel_load_4);
	wire_set_output(rel_load_5);
	wire_set_output(rel_load_6);
	wire_set_output(rel_sync);
	wire_set_output(rel_reserved);
	
	return TRUE;
}