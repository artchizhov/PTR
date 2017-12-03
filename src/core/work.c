/*
 * work.c
 *
 * Created: 29.05.2015 20:06:19
 *  Author: Artyom
 */ 


#include <stdint.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "PTR.h"

#include "core/wires.h"

#include "modules/hd44780.h"
#include "modules/button.h"
#include "modules/relay.h"

#include "work.h"

uint8_t wrk_status = WRK_STATUS_STOP;

uint8_t wrk_step = 0;	// Текущий шаг.

uint8_t wrk_ds = 0;		// Отображаемое значение десятичной части общего времени.
uint16_t wrk_s = 0;		// Отображаемое значение общего времени.

uint32_t wrk_time_ms = 0;	// Общее время.
uint32_t wrk_step_ms = 0;	// Время в "шаге".

step_t EEMEM ee_wrk_steps[WRK_STEP_N];	// EEPROM
step_t wrk_steps[WRK_STEP_N];			// RAM

// Run every 2ms
ISR (TIMER0_COMPA_vect) {
	wrk_time_ms += 2;
	wrk_step_ms	+= 2;
}

uint8_t wrk_start(void) {
	// Временные значения времени.
	// Нужно для поиска моментов изменения времени,
	// что бы выводить измененные значения не постоянно,
	// а только после изменения значений. Как-то так...
	uint8_t  old_ds = 0;
	uint16_t old_s  = 0;
	
	// Сброс общего времени.
	wrk_time_ms	= 0;
	
	// Сброс отображаемых значений общего времени.
	wrk_s  = 0;
	wrk_ds = 0;
	
	wrk_show_time_s();
	wrk_show_time_ds();
	
	// Помечаем процесс как запущенный.
	wrk_status	= WRK_STATUS_RUN;
	wrk_show_state();
	
	// Инициализация: запуск реле синхронизации, для включения внешних устройств.
	rel_on(rel_sync);
	
	// Запуск таймера.
	// FIXME: Сбросить таймер.
	TCCR0B |= _BV(CS02);	// Set prescaler to 256 and start the timer
	
	for (wrk_step = 0; wrk_step<WRK_STEP_N; wrk_step++) {
		if (wrk_steps[wrk_step].time_delay_ms != 0) {	// Если время щага задано, т.е. не равно 0 миллисекунд, то:
			
			// Сброс счетчика, для подсчета времени работы "шага".
			wrk_step_ms = 0;
			
			// Установка реле в состояния, указанные в шаге.
			wrk_set_rel_step();
			
			// Не отображаем изменения на экран для коротких шагов,
			// т.к. вывод на экран отнимает время, из-за чего реле отключаются с опозданием.
			if (wrk_steps[wrk_step].time_delay_ms >= 100) {
				wrk_show_step();
				wrk_show_stime();
				
				// После изменения состояния реле отображаем это на экране.
				wrk_show_relay();
			}
			
			// Ожидание завершения шага.
			while (TRUE) {
				if (btn_get() == &btn_reset) {
					wrk_reset();
					return EXIT_OK;
				}
				
				// Ожидание наступления нужного момента времени.
				if (wrk_step_ms >= wrk_steps[wrk_step].time_delay_ms) break;
				
				// Не отображаем изменения на экран для коротких шагов,
				// т.к. вывод на экран отнимает время, из-за чего реле отключаются с опозданием.
				if (wrk_steps[wrk_step].time_delay_ms >= 100) {
					wrk_ds = (wrk_time_ms / 100) % 10;	// Подсчет десятых частей секунды.
					wrk_s  =  wrk_time_ms / 1000;		// Подсчет секунд.
				
					if (old_ds != wrk_ds) {
						old_ds = wrk_ds;
						wrk_show_time_ds();
					}
				
					if (old_s != wrk_s) {
						old_s = wrk_s;
						wrk_show_time_s();
					}
				}				
			}
		} else {
			break;	// Больше нет шагов.
		}
	}
	
	wrk_reset();
	
	return EXIT_OK;
}

void wrk_reset(void) {
	// Stop timer.
	TCCR0B &= ~_BV(CS00);
	TCCR0B &= ~_BV(CS01);
	TCCR0B &= ~_BV(CS02);
	
	rel_set(rel_load_1, OFF);
	rel_set(rel_load_2, OFF);
	rel_set(rel_load_3, OFF);
	rel_set(rel_load_4, OFF);
	rel_set(rel_load_5, OFF);
	rel_set(rel_load_6, OFF);
	
	// После изменения состояния реле отображаем это на экране.
	wrk_show_relay();
	
	rel_set(rel_sync, OFF);
	
	wrk_status = WRK_STATUS_STOP;
	wrk_show_state();
}

uint8_t wrk_show_frame(void) {
	hd44780_clear();
	
	hd44780_goto(0, 0);
	hd44780_print("TIME:");
	hd44780_goto(0, 13);
	hd44780_print_char('.');
	
	hd44780_goto(1, 0);
	hd44780_print("STEP:");
	
	hd44780_goto(2, 0);
	hd44780_print("ST.TIME:");
	hd44780_goto(2, 13);
	hd44780_print_char('.');
	
	hd44780_goto(3, 0);
	hd44780_print("RELAY:");
	
	return EXIT_OK;
}
uint8_t wrk_show_time_s(void) {
	hd44780_goto(0, 9);
	hd44780_print_num(wrk_s, 4);
	
	return EXIT_OK;
}
uint8_t wrk_show_time_ds(void) {
	hd44780_goto(0, 14);
	hd44780_print_int(wrk_ds);
	
	return EXIT_OK;
}
uint8_t wrk_show_state(void) {
	hd44780_goto(0, 16);
	switch (wrk_status) {
		case WRK_STATUS_RUN: {
			hd44780_print(" RUN");
			break;
		}
		case WRK_STATUS_STOP: {
			hd44780_print("STOP");
			break;
		}
		case WRK_STATUS_WAIT: {
			hd44780_print("WAIT");
			break;
		}
	}
	
	return EXIT_OK;
}
uint8_t wrk_show_step(void) {
	hd44780_goto(1, 9);
	hd44780_print_num(wrk_step + 1, 3);
	
	return EXIT_OK;
}

uint8_t wrk_show_stime(void) {
	hd44780_goto(2, 9);
	hd44780_print_num(wrk_steps[wrk_step].time_delay_ms/1000, 4);
	hd44780_goto(2, 14);
	hd44780_print_num(wrk_steps[wrk_step].time_delay_ms%1000, 3);
	
	return EXIT_OK;
}

uint8_t wrk_show_relay(void) {
	hd44780_goto(3, 9);
	hd44780_print_int(rel_check(rel_load_1));
	hd44780_print_int(rel_check(rel_load_2));
	hd44780_print_int(rel_check(rel_load_3));
	hd44780_print_int(rel_check(rel_load_4));
	hd44780_print_int(rel_check(rel_load_5));
	hd44780_print_int(rel_check(rel_load_6));
	
	return EXIT_OK;
}

uint8_t wrk_show(void) {
	wrk_show_frame();
	wrk_show_time_s();
	wrk_show_time_ds();
	wrk_show_state();
	wrk_show_step();
	wrk_show_stime();
	wrk_show_relay();
	
	return EXIT_OK;
}

uint8_t wrk_init(void) {
	TIMSK0 |= _BV(OCIE0A);	// Set the ISR COMPA vect
	TCCR0A |= _BV(WGM01);	// Set the Timer Mode to CTC
	OCR0A   = 0x7C;			// Set the value that you want to count to
	
	wrk_load_data_from_eeprom();
	
	wrk_show();
	
	return EXIT_OK;
}

uint8_t wrk_load_data_from_eeprom(void) {
	for (uint16_t i = 0; i<WRK_STEP_N; i++) {
		eeprom_read_block(&wrk_steps[i], &ee_wrk_steps[i], sizeof(step_t));
	}

	return EXIT_OK;
}

uint8_t wrk_set_rel_step(void) {
	rel_set(rel_load_1, wrk_steps[wrk_step].rel_load_1_state);
	rel_set(rel_load_2, wrk_steps[wrk_step].rel_load_2_state);
	rel_set(rel_load_3, wrk_steps[wrk_step].rel_load_3_state);
	rel_set(rel_load_4, wrk_steps[wrk_step].rel_load_4_state);
	rel_set(rel_load_5, wrk_steps[wrk_step].rel_load_5_state);
	rel_set(rel_load_6, wrk_steps[wrk_step].rel_load_6_state);
	
	return EXIT_OK;
}

uint8_t wrk_step_inc(void) {
	if ((wrk_step < WRK_STEP_N-1) && (wrk_steps[wrk_step + 1].time_delay_ms != 0)) {
		wrk_step++;
		
		wrk_set_rel_step();
		
		wrk_show_step();
		wrk_show_stime();
		wrk_show_relay();
	}
	
	return EXIT_OK;
}
uint8_t wrk_step_dec(void) {
	if (wrk_step > 0) {
		wrk_step--;
		
		wrk_set_rel_step();
		
		wrk_show_step();
		wrk_show_stime();
		wrk_show_relay();
	}
	
	return EXIT_OK;
}

uint8_t wrk_sett_show_frame(void) {
	hd44780_goto(0, 0);
	hd44780_print("STEP:");
	
	hd44780_goto(1, 0);
	hd44780_print("ST.TIME:");
	hd44780_goto(1, 13);
	hd44780_print_char('.');
	
	hd44780_goto(2, 0);
	hd44780_print("RELAY:");
	
	return EXIT_OK;
}

uint8_t wrk_sett_show_step(void) {
	hd44780_goto(0, 9);
	hd44780_print_num(wrk_step + 1, 3);
	
	return EXIT_OK;
}

uint8_t wrk_sett_show_stime(void) {
	hd44780_goto(1, 9);
	hd44780_print_num(wrk_steps[wrk_step].time_delay_ms/1000, 4);
	
	hd44780_goto(1, 14);
	hd44780_print_num(wrk_steps[wrk_step].time_delay_ms%1000, 3);
	
	return EXIT_OK;
}

uint8_t wrk_sett_show_rel(void) {
	hd44780_goto(2, 9);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_1_state);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_2_state);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_3_state);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_4_state);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_5_state);
	hd44780_print_int(wrk_steps[wrk_step].rel_load_6_state);
	
	return EXIT_OK;
}

uint8_t wrk_sett(void) {
	wire_t *btn;
	
	// Очистка экрана.
	hd44780_clear();
	
	// Подготовка "каркаса" интерфейса.
	wrk_sett_show_frame();
	
	// Заполнение начальными значениями.
	wrk_sett_show_step();
	wrk_sett_show_stime();
	wrk_sett_show_rel();
	
	while (TRUE) {
		btn = btn_get();
		
		if ((btn == &btn_back) && (btn_cntr == BTN_ONCE)) {
			break;
		}
		
		if ((btn == &btn_left) && (btn_cntr == BTN_ONCE)) {
			if (wrk_step > 0) {
				wrk_step--;
				wrk_sett_show_step();
				wrk_sett_show_stime();
				wrk_sett_show_rel();
			}				
			continue;
		}
		if ((btn == &btn_right) && (btn_cntr == BTN_ONCE)) {
			if (wrk_step < WRK_STEP_N-1) {
				wrk_step++;
				wrk_sett_show_step();
				wrk_sett_show_stime();
				wrk_sett_show_rel();
			}				
			continue;
		}
		
		if ((btn == &btn_ok) && (btn_cntr == BTN_ONCE)) {
			wrk_sett_step();
			continue;
		}
	}
	
	// Возврат к обычному режиму.
	wrk_show();
	
	return EXIT_OK;
}

uint8_t wrk_sett_step(void) {
	wire_t *btn;
	
	uint8_t		pos = 0;		// редактируемая позиция.
	
	uint8_t	ed_s1000, ed_s100, ed_s10, ed_s1;	// seconds
	uint8_t	ed_ms100, ed_ms10, ed_ms1;			// ms
	
	uint8_t	ed_r1, ed_r2, ed_r3, ed_r4, ed_r5, ed_r6;	// rels
	
	// Считывание исходных значений.
	ed_s1000	= (wrk_steps[wrk_step].time_delay_ms/1000000)%10;
	ed_s100		= (wrk_steps[wrk_step].time_delay_ms/100000)%10;
	ed_s10		= (wrk_steps[wrk_step].time_delay_ms/10000)%10;
	ed_s1		= (wrk_steps[wrk_step].time_delay_ms/1000)%10;
	
	ed_ms100	= (wrk_steps[wrk_step].time_delay_ms/100)%10;
	ed_ms10		= (wrk_steps[wrk_step].time_delay_ms/10)%10;
	ed_ms1		=  wrk_steps[wrk_step].time_delay_ms%10;
	
	ed_r1		= wrk_steps[wrk_step].rel_load_1_state;
	ed_r2		= wrk_steps[wrk_step].rel_load_2_state;
	ed_r3		= wrk_steps[wrk_step].rel_load_3_state;
	ed_r4		= wrk_steps[wrk_step].rel_load_4_state;
	ed_r5		= wrk_steps[wrk_step].rel_load_5_state;
	ed_r6		= wrk_steps[wrk_step].rel_load_6_state;
	
	// TEST
	hd44780_cmd(HD44780_DISPCTRL_DISPON_BLKOFF_CRSON);
	wrk_sett_edit_mvcurs(pos);
	
	while (TRUE) {
		btn = btn_get();
		
		if (btn == &btn_back) {
			break;
		}
		
		if ((btn == &btn_left) && (btn_cntr == BTN_ONCE)) {
			if (pos > 0) {
				pos--;
				wrk_sett_edit_mvcurs(pos);
			}
			continue;
		}
		if ((btn == &btn_right) && (btn_cntr == BTN_ONCE)) {
			if (pos < WRK_EDIT_POS_N-1) {
				pos++;
				wrk_sett_edit_mvcurs(pos);
			}
			continue;
		}
		
		if ((btn == &btn_ok) && (btn_cntr == BTN_ONCE)) {
			switch (pos) {
				case WRK_EDIT_S1000: {
					wrk_sett_edit_chval(&ed_s1000, 0, 9);
					hd44780_goto(1, 9);
					hd44780_print_int(ed_s1000);
					break;
				}
				case WRK_EDIT_S100: {
					wrk_sett_edit_chval(&ed_s100, 0, 9);
					hd44780_goto(1, 10);
					hd44780_print_int(ed_s100);
					break;
				}
				case WRK_EDIT_S10: {
					wrk_sett_edit_chval(&ed_s10, 0, 9);
					hd44780_goto(1, 11);
					hd44780_print_int(ed_s10);
					break;
				}
				case WRK_EDIT_S1: {
					wrk_sett_edit_chval(&ed_s1, 0, 9);
					hd44780_goto(1, 12);
					hd44780_print_int(ed_s1);
					break;
				}
				
				case WRK_EDIT_MS100: {
					wrk_sett_edit_chval(&ed_ms100, 0, 9);
					hd44780_goto(1, 14);
					hd44780_print_int(ed_ms100);
					break;
				}
				case WRK_EDIT_MS10: {
					wrk_sett_edit_chval(&ed_ms10, 0, 9);
					hd44780_goto(1, 15);
					hd44780_print_int(ed_ms10);
					break;
				}
				case WRK_EDIT_MS1: {
					wrk_sett_edit_chval(&ed_ms1, 0, 9);
					hd44780_goto(1, 16);
					hd44780_print_int(ed_ms1);
					break;
				}
				
				case WRK_EDIT_R1: {
					wrk_sett_edit_chval(&ed_r1, 0, 1);
					hd44780_goto(2, 9);
					hd44780_print_int(ed_r1);
					break;
				}
				case WRK_EDIT_R2: {
					wrk_sett_edit_chval(&ed_r2, 0, 1);
					hd44780_goto(2, 10);
					hd44780_print_int(ed_r2);
					break;
				}
				case WRK_EDIT_R3: {
					wrk_sett_edit_chval(&ed_r3, 0, 1);
					hd44780_goto(2, 11);
					hd44780_print_int(ed_r3);
					break;
				}
				case WRK_EDIT_R4: {
					wrk_sett_edit_chval(&ed_r4, 0, 1);
					hd44780_goto(2, 12);
					hd44780_print_int(ed_r4);
					break;
				}
				case WRK_EDIT_R5: {
					wrk_sett_edit_chval(&ed_r5, 0, 1);
					hd44780_goto(2, 13);
					hd44780_print_int(ed_r5);
					break;
				}
				case WRK_EDIT_R6: {
					wrk_sett_edit_chval(&ed_r6, 0, 1);
					hd44780_goto(2, 14);
					hd44780_print_int(ed_r6);
					break;
				}
			}
			wrk_sett_edit_mvcurs(pos); // FIXME Hack
			continue;
		}
	}
	
	// Вывод измененного значения.
	
	/*
	/ (uint32_t) - исправляет проблему с преобразованием типов,
	/ без этого, введенное число 4150000 ошибочно считается как 4084464.
	/ Ошибка возникает при введении цифры более "3" в сегменте "десяток" (0040.000, 0050.000...),
	/ Возможная причина - это неправильное приведение типов, т.к.:
	/ при умножении 4*10000 компилятор ожидает число размера int16, т.к. число 10000 меньше 32 тысяч,
	/ и при умножении возникает переполнение: 4*10000 = 40000 - это больше 32768, поэтому число (результат умножения) принимает отрицательное значение и все портит.
	*/
	
	wrk_steps[wrk_step].time_delay_ms = \
		  (uint32_t)ed_s1000*1000000 \
		+ (uint32_t)ed_s100*100000 \
		+ (uint32_t)ed_s10*10000 \
		+ (uint32_t)ed_s1*1000 \
		+ (uint32_t)ed_ms100*100 \
		+ (uint32_t)ed_ms10*10 \
		+ (uint32_t)ed_ms1;
	
	wrk_steps[wrk_step].rel_load_1_state = ed_r1;
	wrk_steps[wrk_step].rel_load_2_state = ed_r2;
	wrk_steps[wrk_step].rel_load_3_state = ed_r3;
	wrk_steps[wrk_step].rel_load_4_state = ed_r4;
	wrk_steps[wrk_step].rel_load_5_state = ed_r5;
	wrk_steps[wrk_step].rel_load_6_state = ed_r6;
	
	eeprom_write_block(&wrk_steps[wrk_step], &ee_wrk_steps[wrk_step], sizeof(step_t));
	
	hd44780_cmd(HD44780_DISPCTRL_DISPON_BLKOFF_CRSOFF);
	
	return EXIT_OK;
}

uint8_t wrk_sett_edit_chval(uint8_t *val, uint8_t min, uint8_t max) {
	if (*val < max) {
		(*val)++;
	} else if (*val >= max) {
		*val = min;
	}
	
	return EXIT_OK;
}

uint8_t wrk_sett_edit_mvcurs(uint8_t pos) {
	switch (pos) {
		case WRK_EDIT_S1000:	hd44780_goto(1, 9);		break;
		case WRK_EDIT_S100:		hd44780_goto(1, 10);	break;
		case WRK_EDIT_S10:		hd44780_goto(1, 11);	break;
		case WRK_EDIT_S1:		hd44780_goto(1, 12);	break;
		case WRK_EDIT_MS100:	hd44780_goto(1, 14);	break;
		case WRK_EDIT_MS10:		hd44780_goto(1, 15);	break;
		case WRK_EDIT_MS1:		hd44780_goto(1, 16);	break;
		case WRK_EDIT_R1:		hd44780_goto(2, 9);		break;
		case WRK_EDIT_R2:		hd44780_goto(2, 10);	break;
		case WRK_EDIT_R3:		hd44780_goto(2, 11);	break;
		case WRK_EDIT_R4:		hd44780_goto(2, 12);	break;
		case WRK_EDIT_R5:		hd44780_goto(2, 13);	break;
		case WRK_EDIT_R6:		hd44780_goto(2, 14);	break;
	}
	
	return EXIT_OK;
}