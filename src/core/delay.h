/*
 * delay.h
 *
 * Created: 28.05.2015 19:34:43
 *  Author: Artyom
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include "PTR.h"

#include <stdint.h>
#include <util/delay.h>

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#endif /* DELAY_H_ */