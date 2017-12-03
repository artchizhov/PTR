/*
 * wires.c
 *
 * Created: 28.05.2015 19:20:32
 *  Author: Artyom
 */ 


#include "wires.h"

// DDR,P = бунд
void wire_set_input(wire_t w) {
	switch (w.port) {
		#ifdef DDRA
		case A: DDRA &= ~_BV(w.pin); break;
		#endif
		
		#ifdef DDRB
		case B: DDRB &= ~_BV(w.pin); break;
		#endif
		
		#ifdef DDRC
		case C: DDRC &= ~_BV(w.pin); break;
		#endif
		
		#ifdef DDRD
		case D:	DDRD &= ~_BV(w.pin); break;
		#endif
	}
}

// DDR,P = бшунд
void wire_set_output(wire_t w) {
	switch (w.port) {
		#ifdef DDRA
		case A: DDRA |= _BV(w.pin); break;
		#endif
		
		#ifdef DDRB
		case B: DDRB |= _BV(w.pin); break;
		#endif
		
		#ifdef DDRC
		case C: DDRC |= _BV(w.pin); break;
		#endif
		
		#ifdef DDRD
		case D:	DDRD |= _BV(w.pin); break;
		#endif
	}
}

// PORT,P = 1
void wire_set_high(wire_t w) {
	switch (w.port) {
		#ifdef PORTA
		case A: PORTA |= _BV(w.pin); break;
		#endif
		
		#ifdef PORTB
		case B: PORTB |= _BV(w.pin); break;
		#endif
		
		#ifdef PORTC
		case C: PORTC |= _BV(w.pin); break;
		#endif
		
		#ifdef PORTD
		case D:	PORTD |= _BV(w.pin); break;
		#endif
	}
}

// PORT,P = 0
void wire_set_low(wire_t w) {
	switch (w.port) {
		#ifdef PORTA
		case A: PORTA &= ~_BV(w.pin); break;
		#endif
		
		#ifdef PORTB
		case B: PORTB &= ~_BV(w.pin); break;
		#endif
		
		#ifdef PORTC
		case C: PORTC &= ~_BV(w.pin); break;
		#endif
		
		#ifdef PORTD
		case D:	PORTD &= ~_BV(w.pin); break;
		#endif
	}
}

// PORT,P TOGGLE
void wire_toggle(wire_t w) {
	switch (w.port) {
		#ifdef PORTA
		case A: PORTA ^= _BV(w.pin); break;
		#endif
		
		#ifdef PORTB
		case B: PORTB ^= _BV(w.pin); break;
		#endif
		
		#ifdef PORTC
		case C: PORTC ^= _BV(w.pin); break;
		#endif
		
		#ifdef PORTD
		case D:	PORTD ^= _BV(w.pin); break;
		#endif
	}
}

// PORT,P CHECK BIT
uint8_t wire_check(wire_t w) {
	switch (w.port) {
		#ifdef PORTA
		case A: return (PORTA & _BV(w.pin)); break;
		#endif
		
		#ifdef PORTB
		case B: return (PORTB & _BV(w.pin)); break;
		#endif
		
		#ifdef PORTC
		case C: return (PORTC & _BV(w.pin)); break;
		#endif
		
		#ifdef PORTD
		case D:	return (PORTD & _BV(w.pin)); break;
		#endif
	}
	return NOPORT;
}

// PIN,P GET BIT
uint8_t wire_get(wire_t w) {
	switch (w.port) {
		#ifdef PINA
		case A: return (PINA & _BV(w.pin)); break;
		#endif
		
		#ifdef PINB
		case B: return (PINB & _BV(w.pin)); break;
		#endif
		
		#ifdef PINC
		case C: return (PINC & _BV(w.pin)); break;
		#endif
		
		#ifdef PIND
		case D:	return (PIND & _BV(w.pin)); break;
		#endif
	}
	return NOPORT;
}