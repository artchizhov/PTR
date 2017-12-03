/*
 * button.c
 *
 * Created: 27.05.2015 19:58:54
 *  Author: Artyom
 */ 


#include "PTR.h"

#include <stdint.h>

#include "core/wires.h"

#include "button.h"

wire_t btn_null = NOWR;

wire_t btn_start = {B, 0};
wire_t btn_reset = {B, 1};

wire_t btn_back  = {B, 2};
wire_t btn_ok    = {B, 3};
wire_t btn_right = {B, 4};
wire_t btn_left  = {B, 5};

wire_t  *btn_prev = &btn_null;
uint16_t btn_cntr = 0;

uint8_t btn_init(void) {
	wire_set_input(btn_start);
	wire_set_input(btn_reset);
	
	wire_set_input(btn_back);
	wire_set_input(btn_ok);
	wire_set_input(btn_right);
	wire_set_input(btn_left);
	
	return EXIT_OK;
}

wire_t* btn_get(void) {
	wire_t* btn = &btn_null;
	
	if (!wire_get(btn_start))	btn = &btn_start;
	if (!wire_get(btn_reset))	btn = &btn_reset;
	
	if (!wire_get(btn_back))	btn = &btn_back;
	if (!wire_get(btn_ok))		btn = &btn_ok;
	if (!wire_get(btn_right))	btn = &btn_right;
	if (!wire_get(btn_left))	btn = &btn_left;
	
	if (btn != &btn_null) {
		if (btn == btn_prev) {
			btn_cntr++;
		} else {
			btn_cntr = 1;
		}
	}
	
	btn_prev = btn;

	return btn;
}