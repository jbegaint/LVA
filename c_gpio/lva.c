#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <libconfig.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"

#include "matrix.h"
#include "patterns.h"
#include "pins.h"
#include "utils.h"

static int running = 1;

static const char **pins_rows_names;
static const char **pins_cols_names;

static matrix_t *led_matrix;
static pin_t *pins_rows;
static pin_t *pins_cols;

void handler_sigint(int sig)
{
	UNUSED(sig);

	running = 0;
	printf("Exiting...\n");
}

void parse_config(void)
{
	config_t cf[1];
	const config_setting_t *row, *col;

	const char *name;
	int x, y;
	int i;

	int n_cols, n_rows;

	config_init(cf);

	if (!config_read_file(cf, CFG_FILE)) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
	 
		die("error: can't open config file %s\n", CFG_FILE);
	}

	if (config_lookup_string(cf, "name", &name))
		printf("name: %s\n", name);

	if (config_lookup_int(cf, "pixels.x", &x))
		printf("pixels x: %d\n", x);

	if (config_lookup_int(cf, "pixels.y", &y))
		printf("pixels y: %d\n", y);

	row = config_lookup(cf, "pins.row");
	col = config_lookup(cf, "pins.col");
	n_rows = config_setting_length(row);
	n_cols = config_setting_length(col);

	pins_cols_names = calloc(n_cols, sizeof(char *));
	pins_rows_names = calloc(n_rows, sizeof(char *));

	printf("n_rows: %d\n", n_rows);
	for (i = 0; i < n_rows; ++i) {
		printf("\t#%d. %s\n", i + 1, config_setting_get_string_elem(row, i));
		pins_rows_names[i] = strdup(config_setting_get_string_elem(row, i));
	}

	printf("n_cols: %d\n", n_cols);
	for (i = 0; i < n_cols; ++i) {
		printf("\t#%d. %s\n", i + 1, config_setting_get_string_elem(col, i));
		pins_cols_names[i] = strdup(config_setting_get_string_elem(col, i));
	}

	config_destroy(cf);
}

void setup(void)
{
	/* parse config file */
	parse_config();

	/* get pins from pins names */
	pins_rows = get_pins_by_names(pins_rows_names, ARRAY_SIZE(pins_rows_names));
	pins_cols = get_pins_by_names(pins_cols_names, ARRAY_SIZE(pins_cols_names));
	
	/* init gpios */
	iolib_init();
	enable_gpios();

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

	/* free pins arrays */
	free(pins_cols_names);
	free(pins_rows_names);
	free(pins_cols);
	free(pins_rows);

	/* remaining frees */
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
