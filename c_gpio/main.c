#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"

#include "matrix.h"
#include "patterns.h"
#include "pins.h"
#include "utils.h"

static int running;

static const char* pins_rows_names[] =
{

}

static const char* pins_cols_names[] =
{

}

static matrix_t *led_matrix;
static const pin_t *pins_rows;
static const pin_t *pins_cols;

void handler_sigint(int sig)
{
	running = 0;
	printf("Exiting...\n")
}

void setup(void)
{
	/* init gpios */
	iolib_init();
	enable_gpios();

	/* get pins from pins names */
	pins_rows = get_pins_by_names(pins_rows_names);
	pins_cols = get_pins_by_names(pins_cols_names);

	/* set direction as output */
	set_dir_pins_output(pins_rows, N_ROWS);
	set_dir_pins_output(pins_cols, N_COLS);

	/* set all pins off */
	set_pins_row_off(pins_cols, N_COLS);
	unselect_rows(pins_rows, N_ROWS);

	/* allocate matrix */
	led_matrix = init_matrix(N_ROWS, N_COLS);
}


void cleanup(void)
{
	/* set all pins off */
	set_pins_row_off(pins_cols, N_COLS);
	unselect_rows(pins_rows, N_ROWS);

	/* frees */
	free_matrix(led_matrix);
	iolib_free();
}

void *set_pins_values(void *arg)
{
	UNUSED(arg);

	while (1) {
		set_pattern_from_xtion(led_matrix);
		usleep(100 * 1000);
	}
}

void set_led_values(void)
{
	int l, row;

	while (running) {
		
		for (row = 0; row < N_ROWS; ++row) {

			select_row_by_pin(&pins_rows[row]);

			/* loop over levels */
			/* N_LEVELS-2, indeed no need to check level 3 (off) */
			for (l = N_LEVELS - 2; l >= 0 ; --l) {
	
				set_pins_row_off(pins_cols, N_COLS);
				set_pins_row_on_for_level(LED_MATRIX, pins_cols, row, l);
	
				level_sleep(l);
			}
			unselect_row_by_pin(&pins_rows[row]);
		}
	}
}

int main(void)
{
	pthread_t thread;

	/* trap signal */
	signal(SIGINT, handler_sigint);

	setup();

	/* start thread */
	pthread_create(&thread, NULL, set_pins_values, NULL);
	set_led_values();

	pthread_join(thread, NULL);
	cleanup();
	
	return 0;
}
