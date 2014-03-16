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
		led_y = ((led_y + 1) % N_ROWS);
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
		led_y = (led_y + 1) % N_ROWS;
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
	row_id = (row_id + 1) % N_ROWS;
}

void set_pattern_col_by_col(matrix_t *m)
{
	static int col_id = 0;

	reset_matrix(m);

	for (int i = 0; i < N_ROWS; ++i) {
		(m->values)[i][col_id] = 0;
	}

	col_id++;
	col_id = col_id % N_COLS;
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
