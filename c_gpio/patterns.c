#include <stdlib.h>

#include "pins.h"
#include "patterns.h"

void set_pattern_led_by_led(matrix_t *m)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	/* a bit overkill, but we'll do for now */
	reset_matrix(m);

	for (i = 0; i < N_ROWS; i++) {
		for (j = 0; j < N_COLS; j++) {
			if (i == led_y && j == led_x) {
				toggle_pin(m, i, j);
			}
		}		
	}

	if ((++led_x) == N_COLS) {
		led_x = 0;
		led_y = (++led_y) % N_ROWS;
	}
}

void set_pattern_led_by_led_toggle(matrix_t *m)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	for (i = 0; i < N_ROWS; i++) {
		for (j = 0; j < N_COLS; j++) {
			if (i == led_y && j == led_x) {
				toggle_pin(m, i, j);
			}
		}		
	}

	if ((++led_x) == N_COLS) {
		led_x = 0;
		led_y = (++led_y) % N_ROWS;
	}
}

void set_pattern_random(matrix_t *m)
{
	for (int i = 0; i < N_ROWS; i++) {
		for (int j = 0; j < N_COLS; j++) {
			(m->values)[i][j] = rand() % 4;
		}
	}
}

void toggle_pin(matrix_t *m, int i, int j)
{
	if ((m->values)[i][j] == 0) {
		(m->values)[i][j] = 3;
	}
	else {
		(m->values)[i][j] = 0;
	}
}

void setup_matrix(matrix_t *matrix, int n_rows, int n_cols)
{
	int **m;

	/* todo: check for alloc error */
	m = (int **) calloc(n_rows, sizeof(*m));
	*m = (int *) calloc(n_rows*n_cols, sizeof(**m));

	for (int i = 1; i < n_rows; ++i) {
		m[i] = m[i-1] + n_cols;
	}

	matrix->n_rows = n_rows;
	matrix->n_cols = n_cols;
	matrix->values = m;

	/* off value is 3 so we need to init matrix values at 3 */
	reset_matrix(matrix);
}

void reset_matrix(matrix_t *matrix)
{
	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols ; ++j) {
			(matrix->values)[i][j] = 3;
		}		
	}
}