#include <stdlib.h>

#include "pins.h"
#include "patterns.h"

const pattern_t patterns[] = {
	{0, "led by led", set_pattern_led_by_led},
	{1, "led by led toggle", set_pattern_led_by_led_toggle},
	{2, "random", set_pattern_random},
	{3, "row by row", set_pattern_row_by_row},
	{4, "col by col", set_pattern_col_by_col},
	{.desc = NULL},
};

/**
 * @brief Allocate a matrix structure
 *
 * @param n_rows number of rows
 * @param n_cols number of columns
 *
 * @return a pointer to the allocated matrix
 */
matrix_t *setup_matrix(int n_rows, int n_cols)
{
	int **m;
	matrix_t *matrix;

	matrix = calloc(1, sizeof(*matrix));

	/* todo: check for alloc error */
	m = (int **) calloc(n_rows, sizeof(*m));
	*m = (int *) calloc(n_rows*n_cols, sizeof(**m));

	for (int i = 1; i < n_rows; ++i) {
		m[i] = m[i-1] + n_cols;
	}

	matrix->n_rows = n_rows;
	matrix->n_cols = n_cols;
	matrix->values = m;

	/* off value being 3, we need to set levels values to off */
	reset_matrix(matrix);

	return matrix;
}

/**
 * @brief Set the values of a matrix structure to OFF
 *
 * @param matrix the matrix to alter
 *
 */
void reset_matrix(matrix_t *matrix)
{
	for (int i = 0; i < matrix->n_rows; ++i) {
		for (int j = 0; j < matrix->n_cols ; ++j) {
			(matrix->values)[i][j] = 3;
		}		
	}
}

void set_pattern_led_by_led(matrix_t *m)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	/* a bit overkill atm */
	reset_matrix(m);

	for (i = 0; i < N_ROWS; ++i) {
		for (j = 0; j < N_COLS; ++j) {
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

	for (i = 0; i < N_ROWS; ++i) {
		for (j = 0; j < N_COLS; ++j) {
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
	for (int i = 0; i < N_ROWS; ++i) {
		for (int j = 0; j < N_COLS; ++j) {
			(m->values)[i][j] = rand() % 4;
		}
	}
}

void set_pattern_row_by_row(matrix_t *m)
{
	static int row_id = 0;

	reset_matrix(m);

	for (int j = 0; j < N_COLS; ++j) {
		(m->values)[row_id][j] = 0;
	}

	row_id = (++row_id) % N_ROWS;
}

void set_pattern_col_by_col(matrix_t *m)
{
	static int col_id = 0;

	reset_matrix(m);

	for (int i = 0; i < N_ROWS; ++i) {
		(m->values)[i][col_id] = 0;
	}

	col_id = (++col_id) % N_COLS;
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
