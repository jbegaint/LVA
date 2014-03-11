#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "pins.h"
#include "utils.h"



/* to be continued */
pin_t pins_table[] = {
	{"P9_11", BBBIO_GPIO0, BBBIO_GPIO_PIN_30},
	{"P9_12", BBBIO_GPIO1, BBBIO_GPIO_PIN_28},
	{"P9_13", BBBIO_GPIO0, BBBIO_GPIO_PIN_31},
	{"P9_14", BBBIO_GPIO1, BBBIO_GPIO_PIN_18},
	{"P9_28", BBBIO_GPIO3, BBBIO_GPIO_PIN_17},
	{"P9_29", BBBIO_GPIO3, BBBIO_GPIO_PIN_15},
	{"P9_30", BBBIO_GPIO3, BBBIO_GPIO_PIN_16},
	{"P9_31", BBBIO_GPIO3, BBBIO_GPIO_PIN_14},
	{NULL, 0, 0},
};

int pin_sys_delay_us(unsigned long usec)
{
	struct timespec a;

	a.tv_nsec = (usec) * 1000L;
	a.tv_sec = 0;
	
	if (nanosleep(&a, NULL) != 0) {
		fprintf(stderr, "delay_ms error: %s\n", strerror(errno));
	}
	return 0;
}

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
