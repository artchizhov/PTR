/*
 * work.h
 *
 * Created: 29.05.2015 20:05:33
 *  Author: Artyom
 */ 


#ifndef WORK_H_
#define WORK_H_

#include <stdint.h>
#include "PTR.h"

#define WRK_STATUS_RUN	0
#define WRK_STATUS_STOP	1
#define WRK_STATUS_WAIT	2
#define WRK_STATUS_FAST	3

#define WRK_STEP_N	100

//  оличество редактируемых позиций.
#define WRK_EDIT_POS_N	13
// (вместо enum...)
#define WRK_EDIT_S1000	0
#define WRK_EDIT_S100	1
#define WRK_EDIT_S10	2
#define WRK_EDIT_S1		3

#define WRK_EDIT_MS100	4
#define WRK_EDIT_MS10	5
#define WRK_EDIT_MS1	6

#define WRK_EDIT_R1		7
#define WRK_EDIT_R2		8
#define WRK_EDIT_R3		9
#define WRK_EDIT_R4		10
#define WRK_EDIT_R5		11
#define WRK_EDIT_R6		12

extern uint8_t	wrk_status;
extern uint8_t	wrk_step;
extern uint8_t	wrk_ds;
extern uint16_t	wrk_s;
extern uint32_t	wrk_time_ms;
extern uint32_t	wrk_step_ms;

// —труктура данных, описывающа€ временной интервал.
typedef struct {
	// —осто€ние реле, при выполнении данного "шага".
	bool_t		rel_load_1_state;
	bool_t		rel_load_2_state;
	bool_t		rel_load_3_state;
	bool_t		rel_load_4_state;
	bool_t		rel_load_5_state;
	bool_t		rel_load_6_state;
	
	// ƒлительность выполнени€ данного "шага" в мс.
	uint32_t	time_delay_ms;
} step_t;

extern step_t wrk_steps[];

uint8_t		wrk_start		(void);
void		wrk_reset		(void);

uint8_t		wrk_show		(void);
uint8_t		wrk_show_frame	(void);
uint8_t		wrk_show_time_s	(void);
uint8_t		wrk_show_time_ds(void);
uint8_t		wrk_show_state	(void);
uint8_t		wrk_show_step	(void);
uint8_t		wrk_show_stime	(void);
uint8_t		wrk_show_relay	(void);

uint8_t		wrk_init		(void);
uint8_t		wrk_load_data_from_eeprom(void);

uint8_t		wrk_set_rel_step(void);
uint8_t		wrk_step_inc	(void);
uint8_t		wrk_step_dec	(void);

uint8_t		wrk_sett			(void);
uint8_t		wrk_sett_show_frame	(void);
uint8_t		wrk_sett_show_step	(void);
uint8_t		wrk_sett_show_stime	(void);
uint8_t		wrk_sett_show_rel	(void);
uint8_t		wrk_sett_step		(void);
uint8_t		wrk_sett_edit_chval	(uint8_t *val, uint8_t min, uint8_t max);
uint8_t		wrk_sett_edit_mvcurs(uint8_t pos);

#endif /* WORK_H_ */