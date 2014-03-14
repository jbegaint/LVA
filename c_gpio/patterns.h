#ifndef _PATTERNS_H_
#define _PATTERNS_H_

#define N_ROWS 7
#define N_COLS 5

#define N_LEVELS 4

#define LED_BY_LED_TOGGLE 0
#define LED_BY_LED 1

typedef struct matrix_t {
	int **values;
	int n_rows;
	int n_cols;
} matrix_t;

typedef struct pattern_t {
	int id;
	const char* desc;
	void (*pattern_fct_ptr)(matrix_t *);
} pattern_t;


matrix_t *setup_matrix(int n_rows, int n_cols);
void reset_matrix(matrix_t *matrix);

void toggle_pin(matrix_t *m, int i, int j);

void set_pattern_led_by_led_toggle(matrix_t *matrix);
void set_pattern_led_by_led(matrix_t *matrix);
void set_pattern_random(matrix_t *m);
void set_pattern_row_by_row(matrix_t *m);
void set_pattern_col_by_col(matrix_t *m);

#endif