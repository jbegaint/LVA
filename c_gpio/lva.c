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

static int running = 1;

/* 14 rows */
static const char* pins_rows_names[] =
{
	"P8_39",
	"P8_41",
	"P8_43",
	"P8_45",
	"P8_46",
	"P8_44",
	"P8_42",
	"P8_40",
	"P8_38",
	"P8_36",
	"P8_32",
	"P8_30",
	"P8_28",
	"P8_26",
};

/* 25 columns */
static const char* pins_cols_names[] =
{
	"P8_18", 
	"P8_16", 
	"P8_14", 
	"P8_12", 
	"P8_10",
	"P8_08",
	"P8_37",
	"P8_33",
	"P8_31",
	"P8_29",
	"P8_27",
	"P8_19",
	"P8_17",
	"P8_15",
	"P8_13",
	"P8_11",
	"P8_09",
	"P8_07",
	"P9_25",
	"P9_12",
	"P9_14",
	"P9_24",
	"P9_23",
	"P9_13",
	"P9_11",
};

static matrix_t *led_matrix;
static pin_t *pins_rows;
static pin_t *pins_cols;

void handler_sigint(int sig)
{
	UNUSED(sig);

	running = 0;
	printf("Exiting...\n");
}

void setup(void)
{
	/* init gpios */
	iolib_init();
	enable_gpios();

	/* get pins from pins names */
	pins_rows = get_pins_by_names(pins_rows_names, ARRAY_SIZE(pins_rows_names));
	pins_cols = get_pins_by_names(pins_cols_names, ARRAY_SIZE(pins_cols_names));

	/* set direction as output */
	set_dir_pins_output(pins_rows, N_ROWS);
	set_dir_pins_output(pins_cols, N_COLS);

	/* set all pins off */
	set_pins_off(pins_cols, pins_rows, N_COLS);

	/* allocate matrix */
	led_matrix = init_matrix(N_ROWS, N_COLS);
}


void cleanup(void)
{
	/* set all pins off */
	set_pins_off(pins_cols, pins_rows, N_COLS);

	/* frees */
	free_matrix(led_matrix);
	iolib_free();
}

void *set_matrix_values(void *arg)
{
	UNUSED(arg);

	while (running) {
		set_pattern_from_xn(led_matrix, running);
		usleep(100 * 1000);
	}
}

void set_pins_values(void)
{
	int l, row;

	while (running) {
		
		for (row = 0; row < N_ROWS; ++row) {

			select_row_by_pin(&pins_rows[row]);

			/* loop over levels */
			/* N_LEVELS-2, indeed no need to check level 3 (off) */
			for (l = N_LEVELS - 2; l >= 0 ; --l) {
	
				set_pins_row_off(pins_cols, N_COLS);
				set_pins_row_on_for_level(led_matrix, pins_cols, row, l);
	
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
	pthread_create(&thread, NULL, set_matrix_values, NULL);
	set_pins_values();

	pthread_join(thread, NULL);
	cleanup();
	
	return 0;
}
