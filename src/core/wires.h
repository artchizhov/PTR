/*
 * wires.h
 *
 * Created: 28.05.2015 19:20:18
 *  Author: Artyom
 */ 


#ifndef WIRES_H_
#define WIRES_H_

#include <stdint.h>
#include <avr/io.h>

// Ports
#define A	'A'
#define B	'B'
#define C	'C'
#define D	'D'

#define NOWR	{0xFF, 0xFF}	// no wire
#define NOPORT	0xFF

typedef struct {
	uint8_t port;	// A, B, C, D
	uint8_t pin;	// 0-7
} wire_t;

void wire_set_input(wire_t w);
void wire_set_output(wire_t w);

void wire_set_high(wire_t w);
void wire_set_low(wire_t w);

void wire_toggle(wire_t w);

uint8_t wire_check(wire_t w);

uint8_t wire_get(wire_t w);

#endif /* WIRES_H_ */