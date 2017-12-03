/*
 * relay.h
 *
 * Created: 27.05.2015 19:58:30
 *  Author: Artyom
 */ 


#ifndef RELAY_H_
#define RELAY_H_

#include "PTR.h"

#include <stdint.h>

#include "core/wires.h"

extern wire_t rel_load_1;
extern wire_t rel_load_2;
extern wire_t rel_load_3;
extern wire_t rel_load_4;
extern wire_t rel_load_5;
extern wire_t rel_load_6;
extern wire_t rel_sync;
extern wire_t rel_reserved;

void	rel_set		(wire_t w, bool_t state);
void	rel_on		(wire_t w);
void	rel_off		(wire_t w);
void	rel_toggle	(wire_t w);
uint8_t	rel_check	(wire_t w);

uint8_t	rel_init	(void);


#endif /* RELAY_H_ */