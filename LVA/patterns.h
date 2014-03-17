#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#include "matrix.h"

typedef struct pattern_t {
	int id;
	const char *desc;
	void (*func_ptr)(matrix_t *);
} pattern_t;

typedef struct thread_info_t {
	matrix_t *matrix;
	const char *filepath;
} thread_info_t;

void matrix_toggle_pin(matrix_t *m, int i, int j);

void set_pattern_led_by_led_toggle(matrix_t *matrix);
void set_pattern_led_by_led(matrix_t *matrix);
void set_pattern_random(matrix_t *m);
void set_pattern_row_by_row(matrix_t *m);
void set_pattern_col_by_col(matrix_t *m);
void set_pattern_from_oni(matrix_t *m);
void set_pattern_from_pgm(matrix_t *m);

void print_patterns(void);

#endif