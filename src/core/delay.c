/*
 * delay.c
 *
 * Created: 28.05.2015 19:34:51
 *  Author: Artyom
 */ 


#include "delay.h"

void delay_us(uint32_t us) {
	while (us--) _delay_us(1);
}

void delay_ms(uint32_t ms) {
	while (ms--) _delay_ms(1);
}