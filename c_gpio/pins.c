#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "pins.h"
#include "utils.h"

/* to be continued */
pin_t pins_table[] = {
	{"P8_03", BBBPIO_GPIO1, BBBIO_GPIO_PIN_6},
	{"P8_04", BBBPIO_GPIO1, BBBIO_GPIO_PIN_7},
	{"P8_05", BBBPIO_GPIO1, BBBIO_GPIO_PIN_2},
	{"P8_06", BBBPIO_GPIO1, BBBIO_GPIO_PIN_3},
	{"P8_07", BBBPIO_GPIO2, BBBIO_GPIO_PIN_2},
	{"P8_08", BBBPIO_GPIO2, BBBIO_GPIO_PIN_3},
	{"P8_09", BBBPIO_GPIO2, BBBIO_GPIO_PIN_5},
	{"P8_10", BBBPIO_GPIO2, BBBIO_GPIO_PIN_4},
	{"P8_11", BBBPIO_GPIO1, BBBIO_GPIO_PIN_13},
	{"P8_12", BBBPIO_GPIO1, BBBIO_GPIO_PIN_12},
	{"P8_13", BBBPIO_GPIO0, BBBIO_GPIO_PIN_23},
	{"P8_14", BBBPIO_GPIO0, BBBIO_GPIO_PIN_26},
	{"P8_15", BBBPIO_GPIO1, BBBIO_GPIO_PIN_15},
	{"P8_16", BBBPIO_GPIO1, BBBIO_GPIO_PIN_14},
	{"P8_17", BBBPIO_GPIO0, BBBIO_GPIO_PIN_27},
	{"P8_18", BBBPIO_GPIO2, BBBIO_GPIO_PIN_1},
	{"P8_19", BBBPIO_GPIO0, BBBIO_GPIO_PIN_22},
	{"P8_20", BBBPIO_GPIO1, BBBIO_GPIO_PIN_31},
	{"P8_21", BBBPIO_GPIO1, BBBIO_GPIO_PIN_30},
	{"P8_22", BBBPIO_GPIO1, BBBIO_GPIO_PIN_5},
	{"P8_23", BBBPIO_GPIO1, BBBIO_GPIO_PIN_4},
	{"P8_24", BBBPIO_GPIO1, BBBIO_GPIO_PIN_1},
	{"P8_25", BBBPIO_GPIO1, BBBIO_GPIO_PIN_0},
	{"P8_26", BBBPIO_GPIO1, BBBIO_GPIO_PIN_29},
	{"P8_27", BBBPIO_GPIO2, BBBIO_GPIO_PIN_22},
	{"P8_28", BBBPIO_GPIO2, BBBIO_GPIO_PIN_24},
	{"P8_29", BBBPIO_GPIO2, BBBIO_GPIO_PIN_23},
	{"P8_30", BBBPIO_GPIO2, BBBIO_GPIO_PIN_25},
	{"P8_31", BBBPIO_GPIO0, BBBIO_GPIO_PIN_10},
	{"P8_32", BBBPIO_GPIO0, BBBIO_GPIO_PIN_11},
	{"P8_33", BBBPIO_GPIO0, BBBIO_GPIO_PIN_9},
	{"P8_34", BBBPIO_GPIO2, BBBIO_GPIO_PIN_17},
	{"P8_35", BBBPIO_GPIO0, BBBIO_GPIO_PIN_8},
	{"P8_36", BBBPIO_GPIO2, BBBIO_GPIO_PIN_16},
	{"P8_37", BBBPIO_GPIO2, BBBIO_GPIO_PIN_14},
	{"P8_38", BBBPIO_GPIO2, BBBIO_GPIO_PIN_15},
	{"P8_39", BBBPIO_GPIO2, BBBIO_GPIO_PIN_12},
	{"P8_40", BBBPIO_GPIO2, BBBIO_GPIO_PIN_13},
	{"P8_41", BBBPIO_GPIO2, BBBIO_GPIO_PIN_10},
	{"P8_42", BBBPIO_GPIO2, BBBIO_GPIO_PIN_11},
	{"P8_43", BBBPIO_GPIO2, BBBIO_GPIO_PIN_8},
	{"P8_44", BBBPIO_GPIO2, BBBIO_GPIO_PIN_9},
	{"P8_45", BBBPIO_GPIO2, BBBIO_GPIO_PIN_6},
	{"P8_46", BBBPIO_GPIO2, BBBIO_GPIO_PIN_7},

	{"P9_11", BBBPIO_GPIO0, BBBIO_GPIO_PIN_30},
	{"P9_12", BBBPIO_GPIO1, BBBIO_GPIO_PIN_28},
	{"P9_13", BBBPIO_GPIO0, BBBIO_GPIO_PIN_31},
	{"P9_14", BBBPIO_GPIO1, BBBIO_GPIO_PIN_18},
	{"P9_15", BBBPIO_GPIO1, BBBIO_GPIO_PIN_16},
	{"P9_16", BBBPIO_GPIO1, BBBIO_GPIO_PIN_19},
	{"P9_17", BBBPIO_GPIO0, BBBIO_GPIO_PIN_5},
	{"P9_18", BBBPIO_GPIO0, BBBIO_GPIO_PIN_4},
	{"P9_19", BBBPIO_GPIO0, BBBIO_GPIO_PIN_13},
	{"P9_20", BBBPIO_GPIO0, BBBIO_GPIO_PIN_12},
	{"P9_21", BBBPIO_GPIO0, BBBIO_GPIO_PIN_3},
	{"P9_22", BBBPIO_GPIO0, BBBIO_GPIO_PIN_2},
	{"P9_23", BBBPIO_GPIO1, BBBIO_GPIO_PIN_17},
	{"P9_24", BBBPIO_GPIO0, BBBIO_GPIO_PIN_15,
	{"P9_25", BBBIO_GPIO3, BBBIO_GPIO_PIN_21},
	{"P9_27", BBBPIO_GPIO0, BBBIO_GPIO_PIN_14},
	{"P9_28", BBBIO_GPIO3, BBBIO_GPIO_PIN_19},
	{"P9_28", BBBIO_GPIO3, BBBIO_GPIO_PIN_17},
	{"P9_29", BBBIO_GPIO3, BBBIO_GPIO_PIN_15},
	{"P9_30", BBBIO_GPIO3, BBBIO_GPIO_PIN_16},
	{"P9_31", BBBIO_GPIO3, BBBIO_GPIO_PIN_14},
	{NULL, 0, 0},
};

pin_t *get_pins_by_names(const char **names, int n_pins)
{
	pin_t *table;
	pin_t *p;
	int i;

	table = calloc(n_pins, sizeof(*table));

	for (i = 0; i < n_pins; ++i) {
		p = get_pin_by_name(names[i]);

		if (!p) {
			die("unknown pin: %s", names[i]);
		}

		memcpy(table+i, p, sizeof(*p));
	}

	return table;
}

pin_t *get_pin_by_name(const char *name)
{
	pin_t *p;

	for (p = pins_table; p->name; ++p) {
		if (strcmp(p->name, name) == 0) {
			return p;
		}
	}

	return NULL;
}

int get_gpio_by_name(const char *name)
{
	pin_t *p;

	for (p = pins_table; p->name; ++p) {
		if (strcmp(p->name, name) == 0) {
			return p->gpio;
		}
	}

	return -1;
}

int get_id_by_name(const char *name)
{
	pin_t *p;

	for (p = pins_table; p->name; ++p) {
		if (strcmp(p->name, name) == 0) {
			return p->id;
		}
	}

	return -1;
}

void print_pin(pin_t *pin)
{
	printf("%s %d %d\n", pin->name, pin->gpio, pin->id);
}

void set_pins_row_on(int gpio, int pins)
{
	BBBIO_GPIO_low(gpio, pins);
}

void set_pins_row_off(int gpio, int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void set_pins_col_on(int gpio, int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void set_pins_col_off(int gpio, int pins)
{
	BBBIO_GPIO_low(gpio, pins);
}
