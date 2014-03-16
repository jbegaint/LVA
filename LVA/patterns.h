#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "matrix.h"

#define LED_BY_LED_TOGGLE 0
#define LED_BY_LED 1

typedef struct pattern_t {
	int id;
	const char* desc;
	void (*func_ptr)(matrix_t *);
} pattern_t;

void toggle_pin(matrix_t *m, int i, int j);

void set_pattern_led_by_led_toggle(matrix_t *matrix);
void set_pattern_led_by_led(matrix_t *matrix);
void set_pattern_random(matrix_t *m);
void set_pattern_row_by_row(matrix_t *m);
void set_pattern_col_by_col(matrix_t *m);

#endif