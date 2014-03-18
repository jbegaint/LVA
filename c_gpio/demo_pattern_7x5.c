#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"

#include "matrix.h"
#include "patterns.h"
#include "pins.h"
#include "utils.h"

int next_frame = 1;

extern pattern_t patterns[];

static int running = 1;

static const char* pins_rows_names[] = {"P8_36", "P8_37", "P8_38", "P8_39", "P8_40", "P8_41", "P8_42"};
static const char* pins_cols_names[] = {"P8_11", "P8_12", "P8_15", "P8_16", "P8_26"};

static const pin_t *pins_rows; 
static const pin_t *pins_cols; 

static matrix_t *LED_MATRIX;

void (*ptrn_func_ptr)(matrix_t*);

void *set_pins_values(void *arg)
{
	UNUSED(arg);
	
	/* call pattern */
	while (1) {
		ptrn_func_ptr(LED_MATRIX);
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

void setup(int *ctrl)
{
	int p;
	int row_ctrl;

	/* init gpio */
	iolib_init();
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);

	/* rows gpio */
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO2);

	/* get pins ctrl */
	for (p = 0; p < N_COLS; ++p) {
		*ctrl |= (pins_cols[p]).id;
	}

	for (p = 0; p < N_ROWS; ++p) {
		row_ctrl |= (pins_rows[p]).id;
	}

	/* set pins as output */
	BBBIO_GPIO_set_dir(BBBIO_GPIO1, 0, *ctrl);
	BBBIO_GPIO_set_dir(BBBIO_GPIO2, 0, row_ctrl);

	/* set all pins to low */
	set_pins_row_off_by_gpio(BBBIO_GPIO1, *ctrl);
	unselect_row_by_id_and_gpio(BBBIO_GPIO2, row_ctrl);

	/* allocate matrix */
	LED_MATRIX = init_matrix(N_ROWS, N_COLS);
}

void cleanup(void)
{
	free_matrix(LED_MATRIX);
	iolib_free();
}

int get_pins_to_set_on(int row_id, int level_id)
{
	int out, p;
	out = 0;

	/* loop over pins */
	for (p = 0; p < N_COLS; ++p) {
		if ((LED_MATRIX->values)[row_id][p] <= level_id) {
			out |= (pins_cols[p]).id;
		}
	}

	return out;
}

void switch_leds(int ctrl)
{
	int l, out, row;

	while (running) {
		
		for (row = 0; row < N_ROWS; ++row) {
			select_row_by_id_and_gpio(BBBIO_GPIO2, pins_rows[row].id);

			/* loop over levels */
			/* N_LEVELS-2, indeed no need to check level 3 (off)*/
			for (l = N_LEVELS - 2; l >= 0 ; --l) {
	
				/* todo: fct to set on/off independent of gpios */
				out = get_pins_to_set_on(row, l);
	
				/* set pins values */
				set_pins_row_off_by_gpio(BBBIO_GPIO1, ctrl & (~out));
				set_pins_row_on_by_gpio(BBBIO_GPIO1, out);
	
				/*
					wait X us 
				 	we already waited for T/LEVELS[l-1], so let's subtract it
				*/
	
				level_sleep(l);
			}
			//set_pins_row_off_by_gpio(BBBIO_GPIO1, ctrl);

			unselect_row_by_id_and_gpio(BBBIO_GPIO2, pins_rows[row].id);

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
	int ctrl;
	pthread_t thread;

	if (argc != 2) {
		usage(argv[0]);
	}

	parse_arg(argv[1]);
	
	pins_rows = get_pins_by_names(pins_rows_names, ARRAY_SIZE(pins_rows_names));
	pins_cols = get_pins_by_names(pins_cols_names, ARRAY_SIZE(pins_cols_names));

	/* start thread */
	pthread_create(&thread, NULL, set_pins_values, NULL);

	setup(&ctrl);
	switch_leds(ctrl);

	signal(SIGINT, handler);

	return 0;
}
