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

void setup_matrix(matrix_t *matrix, int n_rows, int n_cols);
void reset_matrix(matrix_t *matrix);
void toggle_pin(matrix_t *m, int i, int j);

void set_pattern_led_by_led_toggle(matrix_t *matrix);
void set_pattern_led_by_led(matrix_t *matrix);
void set_pattern_random(matrix_t *m);

#endif