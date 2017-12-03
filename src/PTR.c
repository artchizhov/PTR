/*
 * PTR.c
 *
 * Created: 27.05.2015 19:09:18
 *  Author: Artyom
 */


#include "PTR.h"

#include <avr/interrupt.h>

#include "modules/hd44780.h"
#include "modules/button.h"
#include "modules/relay.h"

#include "core/work.h"

int main(void) {
	setup();
	
	uint8_t counter = 0;
	
	while (TRUE) {
		loop_rt();
		counter++;
		if (counter == 0xFF) loop_slow();
	}
}

uint8_t setup(void) {
	hd44780_init();
	btn_init();
	rel_init();
	
	wrk_init();
	
	sei();	// Enable interrupts
	
	return EXIT_OK;
}

void loop_rt(void) {
	wire_t *btn = btn_get();
	
	if (btn == &btn_start) {
		wrk_start();
		return;
	}
	/*if (btn == &btn_reset) {
		wrk_reset();
		return;
	}*/
	
	/*if ((btn == &btn_back) && (btn_cntr == BTN_LONG)) {
		// Очистка шагов.
		for (int i=0; i<WRK_STEP_N; i++) {
			wrk_steps[i].rel_load_1_state = 0;
			wrk_steps[i].rel_load_2_state = 0;
			wrk_steps[i].rel_load_3_state = 0;
			wrk_steps[i].rel_load_4_state = 0;
			wrk_steps[i].rel_load_5_state = 0;
			wrk_steps[i].rel_load_6_state = 0;
			
			wrk_steps[i].time_delay_ms = 0;
		}
		return;
	}*/
	if (btn == &btn_ok) {
		wrk_sett();
		return;
	}
	
	if ((btn == &btn_left) && (btn_cntr == BTN_ONCE)) {
		wrk_step_dec();
		return;
	}
	if ((btn == &btn_right) && (btn_cntr == BTN_ONCE)) {
		wrk_step_inc();
		return;
	}
}

void loop_slow(void) {
	
}