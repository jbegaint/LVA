#include <stdlib.h>
#include <pthread.h>

#include "pins.h"
#include "patterns.h"
#include "oni_utils.h"

#define FILE_ONI_TEST "./oni_files/Captured.oni"

extern int next_frame;

const pattern_t patterns[] = {
	{0, "led by led", set_pattern_led_by_led},
	{1, "led by led toggle", set_pattern_led_by_led_toggle},
	{2, "random", set_pattern_random},
	{3, "row by row", set_pattern_row_by_row},
	{4, "col by col", set_pattern_col_by_col},
	{5, "oni file", set_pattern_from_oni},
	{.desc = NULL},
};

void set_pattern_led_by_led(matrix_t *m)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	/* a bit overkill atm */
	reset_matrix(m);

	for (i = 0; i < m->n_rows; ++i) {
		for (j = 0; j < m->n_cols; ++j) {
			if (i == led_y && j == led_x) {
				toggle_pin(m, i, j);
			}
		}		
	}

	if ((++led_x) == m->n_cols) {
		led_x = 0;
		led_y = ((led_y + 1) % m->n_rows);
	}
}

void set_pattern_led_by_led_toggle(matrix_t *m)
{
	static int led_x = 0;
	static int led_y = 0;

	int i, j;

	for (i = 0; i < m->n_rows; ++i) {
		for (j = 0; j < m->n_cols; ++j) {
			if (i == led_y && j == led_x) {
				toggle_pin(m, i, j);
			}
		}		
	}

	if ((++led_x) == m->n_cols) {
		led_x = 0;
		led_y = (led_y + 1) % m->n_rows;
	}
}

void set_pattern_random(matrix_t *m)
{
	for (int i = 0; i < m->n_rows; ++i) {
		for (int j = 0; j < m->n_cols; ++j) {
			(m->values)[i][j] = rand() % 4;
		}
	}
}

void set_pattern_row_by_row(matrix_t *m)
{
	static int row_id = 0;

	reset_matrix(m);

	for (int j = 0; j < m->n_cols; ++j) {
		(m->values)[row_id][j] = 0;
	}
	row_id = (row_id + 1) % m->n_rows;
}

void set_pattern_col_by_col(matrix_t *m)
{
	static int col_id = 0;

	reset_matrix(m);

	for (int i = 0; i < m->n_rows; ++i) {
		(m->values)[i][col_id] = 0;
	}

	col_id++;
	col_id = col_id % m->n_cols;
}

void set_pattern_from_oni(matrix_t *m)
{
	static matrix_t *oni_matrix;
	static int first_run = 1;
	static pthread_t conversion_thread;
	static thread_info_t thread_info[1];

	if (first_run) {
		oni_matrix = init_matrix(PIXELS_Y, PIXELS_X);

		thread_info->matrix = oni_matrix;
		thread_info->filepath = FILE_ONI_TEST;
		/* launch thread with convert_frames */
		/* todo catch errors */
		pthread_create(&conversion_thread, NULL, convert_frames, (void *) &thread_info);
		first_run = 0;
	}

	m = get_led_matrix(thread_info->matrix);

	next_frame = 1;
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
