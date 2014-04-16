#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "LVA/pins.h"

#define CONFIG_FILE "lva.cfg"

static const char **pins_rows_names;
static const char **pins_cols_names;
static pin_t *pins_rows;
static pin_t *pins_cols;

int main(int argc, char **argv)
{
	config_t cf[1];
	const config_setting_t *row, *col;

	const char *name;
	int x, y;
	int n_cols, n_rows;

	config_init(cf);

	if (!config_read_file(cf, "lva.cfg")) {
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(cf),
			config_error_line(cf),
			config_error_text(cf));
		config_destroy(cf);
	 
		return(EXIT_FAILURE);
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
	int i;
	for (i = 0; i < n_rows; ++i) {
		printf("\t#%d. %s\n", i + 1, config_setting_get_string_elem(row, i));
		pins_rows_names[i] = config_setting_get_string_elem(row, i);
	}

	printf("n_cols: %d\n", n_cols);
	for (i = 0; i < n_cols; ++i) {
		printf("\t#%d. %s\n", i + 1, config_setting_get_string_elem(col, i));
		pins_cols_names[i] = config_setting_get_string_elem(col, i);
	}

	pins_rows = get_pins_by_names(pins_rows_names, ARRAY_SIZE(pins_rows_names));
	pins_cols = get_pins_by_names(pins_cols_names, ARRAY_SIZE(pins_cols_names));

	free(pins_cols_names);
	free(pins_rows_names);
	free(pins_rows);
	free(pins_cols);

	config_destroy(cf);
	return 0;
}