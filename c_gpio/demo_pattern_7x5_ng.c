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

/* patterns table */
extern pattern_t patterns[];

static int running = 1;

static const char* pins_rows_names[] = {"P8_26", "P8_37", "P8_38", "P8_39", "P8_40", "P8_41", "P8_42"};
static const char* pins_cols_names[] = {"P8_11", "P8_12", "P8_15", "P8_16", "P8_26"};

static const pin_t *pins_rows; 
static const pin_t *pins_cols; 

static matrix_t *LED_MATRIX;

void (*ptrn_func_ptr)(matrix_t*);

void *set_pins_values(void *arg)
{
	UNUSED(arg);
	
	while (1) {
		/* call pattern */
		ptrn_func_ptr(LED_MATRIX);
		/* sleep 100 ms */
		usleep(100 * 1000);
	}

	return NULL;
}

void usage(char *argv0)
{
	printf("usage: %s pattern_id\n", argv0);
	exit(EXIT_FAILURE);
}

void handler(int sig)
{
	/* yes i know, we only catch SIGINT ATM.. */
	if (sig == SIGINT) {
		running = 0;
		printf("Exiting...\n");
	}
}

void setup(void)
{
	/* init gpios */
	iolib_init();
	enable_gpios();

	pins_rows = get_pins_by_names(pins_rows_names, 
			ARRAY_SIZE(pins_rows_names));
	pins_cols = get_pins_by_names(pins_cols_names, 
			ARRAY_SIZE(pins_cols_names));

	/* set dir as output */
	set_dir_pins_output(pins_rows, N_ROWS);
	set_dir_pins_output(pins_cols, N_COLS);

	/* set all pins to low */
	set_pins_row_off(pins_cols, N_COLS);
	unselect_rows(pins_rows, N_ROWS);

	/* allocate matrix */
	LED_MATRIX = init_matrix(N_ROWS, N_COLS);
}

void cleanup(void)
{
	/* set all pins to low */
	set_pins_row_off(pins_cols, N_COLS);
	unselect_rows(pins_rows, N_ROWS);

	free_matrix(LED_MATRIX);
	iolib_free();
}

void switch_leds(void)
{
	int l, row;

	while (running) {
		
		for (row = 0; row < N_ROWS; ++row) {

			select_row_by_pin(&pins_rows[row]);

			/* loop over levels */
			/* N_LEVELS-2, indeed no need to check level 3 (off)*/
			for (l = N_LEVELS - 2; l >= 0 ; --l) {
	
				set_pins_row_off(pins_cols, N_COLS);
				set_pins_row_on_for_level(LED_MATRIX, pins_cols, row, l);
	
				/*
					wait X us 
				 	we already waited for T/LEVELS[l-1], so let's subtract it
				*/
	
				level_sleep(l);
			}
			unselect_row_by_pin(&pins_rows[row]);
		}
	}
	/* end of loop, exit */
	cleanup();
}

void parse_arg(char *arg)
{
	int pattern_id;
	int flag_error = 1;
	pattern_t *ptrn;

	pattern_id = atoi(arg);

	printf("loading pattern_id: %d", pattern_id);

	for (ptrn = patterns; ptrn->desc; ++ptrn) {
		if (pattern_id == ptrn->id) {
			ptrn_func_ptr = ptrn->func_ptr;
			flag_error = 0;
			printf("selected pattern: %s\n", ptrn->desc);
		}
	}

	if (flag_error) {
		printf("Unknown pattern id: %s %d\n", arg, pattern_id);
		print_patterns();
		die(NULL);
	}
}

int main(int argc, char **argv)
{
	pthread_t thread;

	if (argc != 2) {
		usage(argv[0]);
	}

	parse_arg(argv[1]);
	
	setup();

	/* start thread */
	pthread_create(&thread, NULL, set_pins_values, NULL);
	switch_leds();

	signal(SIGINT, handler);

	return 0;
}
