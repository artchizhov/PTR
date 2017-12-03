/*
 * PTR.h
 *
 * Created: 27.05.2015 19:18:34
 *  Author: Artyom
 */ 


#ifndef PTR_H_
#define PTR_H_

#define F_CPU 16000000UL	// 1 MHz for Proteus | 16 MHz for real atmega328p

#define TRUE	1
#define FALSE	0

#define ON	1
#define OFF	0

#define YES	1
#define NO	0

#define EXIT_OK		0
#define EXIT_ERR	0xFF

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <avr/sfr_defs.h>

typedef uint8_t bool_t;

uint8_t	setup	(void);

void	loop_rt		(void);		// Обработка команд в реальном времени,
void	loop_slow	(void);

#endif /* PTR_H_ */