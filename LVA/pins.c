#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "pins.h"
#include "utils.h"
#include "matrix.h"

/* sort by brightness */
/* todo: levels.c */
level_t levels_table[] = {
	{0, LEVEL_0},
	{1, LEVEL_1},
	{2, LEVEL_2},
	{3, LEVEL_3},
	{-1, 0},
};

/* BBB GPIOs*/
pin_t pins_table[] = {
	{"P8_03", BBBIO_GPIO1, BBBIO_GPIO_PIN_6},
	{"P8_04", BBBIO_GPIO1, BBBIO_GPIO_PIN_7},
	{"P8_05", BBBIO_GPIO1, BBBIO_GPIO_PIN_2},
	{"P8_06", BBBIO_GPIO1, BBBIO_GPIO_PIN_3},
	{"P8_07", BBBIO_GPIO2, BBBIO_GPIO_PIN_2},
	{"P8_08", BBBIO_GPIO2, BBBIO_GPIO_PIN_3},
	{"P8_09", BBBIO_GPIO2, BBBIO_GPIO_PIN_5},
	{"P8_10", BBBIO_GPIO2, BBBIO_GPIO_PIN_4},
	{"P8_11", BBBIO_GPIO1, BBBIO_GPIO_PIN_13},
	{"P8_12", BBBIO_GPIO1, BBBIO_GPIO_PIN_12},
	{"P8_13", BBBIO_GPIO0, BBBIO_GPIO_PIN_23},
	{"P8_14", BBBIO_GPIO0, BBBIO_GPIO_PIN_26},
	{"P8_15", BBBIO_GPIO1, BBBIO_GPIO_PIN_15},
	{"P8_16", BBBIO_GPIO1, BBBIO_GPIO_PIN_14},
	{"P8_17", BBBIO_GPIO0, BBBIO_GPIO_PIN_27},
	{"P8_18", BBBIO_GPIO2, BBBIO_GPIO_PIN_1},
	{"P8_19", BBBIO_GPIO0, BBBIO_GPIO_PIN_22},
	{"P8_20", BBBIO_GPIO1, BBBIO_GPIO_PIN_31},
	{"P8_21", BBBIO_GPIO1, BBBIO_GPIO_PIN_30},
	{"P8_22", BBBIO_GPIO1, BBBIO_GPIO_PIN_5},
	{"P8_23", BBBIO_GPIO1, BBBIO_GPIO_PIN_4},
	{"P8_24", BBBIO_GPIO1, BBBIO_GPIO_PIN_1},
	{"P8_25", BBBIO_GPIO1, BBBIO_GPIO_PIN_0},
	{"P8_26", BBBIO_GPIO1, BBBIO_GPIO_PIN_29},
	{"P8_27", BBBIO_GPIO2, BBBIO_GPIO_PIN_22},
	{"P8_28", BBBIO_GPIO2, BBBIO_GPIO_PIN_24},
	{"P8_29", BBBIO_GPIO2, BBBIO_GPIO_PIN_23},
	{"P8_30", BBBIO_GPIO2, BBBIO_GPIO_PIN_25},
	{"P8_31", BBBIO_GPIO0, BBBIO_GPIO_PIN_10},
	{"P8_32", BBBIO_GPIO0, BBBIO_GPIO_PIN_11},
	{"P8_33", BBBIO_GPIO0, BBBIO_GPIO_PIN_9},
	{"P8_34", BBBIO_GPIO2, BBBIO_GPIO_PIN_17},
	{"P8_35", BBBIO_GPIO0, BBBIO_GPIO_PIN_8},
	{"P8_36", BBBIO_GPIO2, BBBIO_GPIO_PIN_16},
	{"P8_37", BBBIO_GPIO2, BBBIO_GPIO_PIN_14},
	{"P8_38", BBBIO_GPIO2, BBBIO_GPIO_PIN_15},
	{"P8_39", BBBIO_GPIO2, BBBIO_GPIO_PIN_12},
	{"P8_40", BBBIO_GPIO2, BBBIO_GPIO_PIN_13},
	{"P8_41", BBBIO_GPIO2, BBBIO_GPIO_PIN_10},
	{"P8_42", BBBIO_GPIO2, BBBIO_GPIO_PIN_11},
	{"P8_43", BBBIO_GPIO2, BBBIO_GPIO_PIN_8},
	{"P8_44", BBBIO_GPIO2, BBBIO_GPIO_PIN_9},
	{"P8_45", BBBIO_GPIO2, BBBIO_GPIO_PIN_6},
	{"P8_46", BBBIO_GPIO2, BBBIO_GPIO_PIN_7},

	{"P9_11", BBBIO_GPIO0, BBBIO_GPIO_PIN_30},
	{"P9_12", BBBIO_GPIO1, BBBIO_GPIO_PIN_28},
	{"P9_13", BBBIO_GPIO0, BBBIO_GPIO_PIN_31},
	{"P9_14", BBBIO_GPIO1, BBBIO_GPIO_PIN_18},
	{"P9_15", BBBIO_GPIO1, BBBIO_GPIO_PIN_16},
	{"P9_16", BBBIO_GPIO1, BBBIO_GPIO_PIN_19},
	{"P9_17", BBBIO_GPIO0, BBBIO_GPIO_PIN_5},
	{"P9_18", BBBIO_GPIO0, BBBIO_GPIO_PIN_4},
	{"P9_19", BBBIO_GPIO0, BBBIO_GPIO_PIN_13},
	{"P9_20", BBBIO_GPIO0, BBBIO_GPIO_PIN_12},
	{"P9_21", BBBIO_GPIO0, BBBIO_GPIO_PIN_3},
	{"P9_22", BBBIO_GPIO0, BBBIO_GPIO_PIN_2},
	{"P9_23", BBBIO_GPIO1, BBBIO_GPIO_PIN_17},
	{"P9_24", BBBIO_GPIO0, BBBIO_GPIO_PIN_15},
	{"P9_25", BBBIO_GPIO3, BBBIO_GPIO_PIN_21},
	{"P9_27", BBBIO_GPIO0, BBBIO_GPIO_PIN_14},
	{"P9_28", BBBIO_GPIO3, BBBIO_GPIO_PIN_19},
	{"P9_28", BBBIO_GPIO3, BBBIO_GPIO_PIN_17},
	{"P9_29", BBBIO_GPIO3, BBBIO_GPIO_PIN_15},
	{"P9_30", BBBIO_GPIO3, BBBIO_GPIO_PIN_16},
	{"P9_31", BBBIO_GPIO3, BBBIO_GPIO_PIN_14},
	{NULL, 0, 0},
};

/**
 * @brief Get the time for a level struct
 * 
 * @param level the level struct
 * 
 * @return a level time
 */
int get_level_time(level_t level)
{
	return level.tm;
}

/**
 * @brief Get the id for a level struct
 * 
 * @param level the level struct
 * 
 * @return a level id
 */
int get_level_id(level_t level)
{
	return level.id;
}

/**
 * @brief Find a level in levels_table
 * 
 * @param level_id the level id to search 
 * 
 * @return a pointer to the level if found, NULL otherwise
 */
level_t *get_level_by_id(int level_id)
{
	level_t *l;

	for (l = levels_table; l->id >= 0; ++l) {
		if (l->id == level_id) {
			return l;
		}
	}
	return NULL;
}

/**
 * @brief Get the level time for a level in levels_table
 * 
 * @param level_id the level id to search 
 * 
 * @return the level time if found, -1 otherwise
 */
int get_level_time_by_id(int level_id)
{
	level_t *l;

	if ((l = get_level_by_id(level_id))) {
		return l->tm;
	}
	else {
		return -1;
	}
}

/**
 * @brief Sleep between levels, calculate time to sleep between the current 
 * level and the previous one
 * 
 * @param level_id the current level id  
 */
void level_sleep(int level_id)
{
	int sleep0, sleep1;
	double tmp;

	sleep0 = get_level_time_by_id(level_id);
	sleep1 = get_level_time_by_id(level_id + 1);
	
	if (sleep1 == 0)
		tmp = T / (double) sleep0;
	else
		tmp = T / (double) sleep0 - T / (double) sleep1;
	
	level_usleep(tmp * 1000);
}

/*
 * @brief Sleep, nanosleep wrapper, so it's quite accurate
 *
 * @param us the number of micro-seconds to sleep for
 */
void level_usleep(double us)
{
	struct timespec wait;

	wait.tv_sec = 0;
	wait.tv_nsec = us * 1000;
	nanosleep(&wait, NULL);
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

/**
 * @brief Retrieve a pin structure in pins_table by name
 * 
 * @param name the pin name
 * 
 * @return the pin structure if found, NULL otherwise
 */
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

/**
 * @brief Retrieve the pin gpio by name
 * 
 * @param name the pin name
 * 
 * @return the pin gpio, -1 otherwise
 */
int get_pin_gpio_by_name(const char *name)
{
	pin_t *p;

	for (p = pins_table; p->name; ++p) {
		if (strcmp(p->name, name) == 0) {
			return p->gpio;
		}
	}

	return -1;
}

/**
 * @brief Retrieve the pin id by name
 * 
 * @param name the pin name
 * 
 * @return the pin id, -1 otherwise
 */
int get_pin_id_by_name(const char *name)
{
	pin_t *p;

	for (p = pins_table; p->name; ++p) {
		if (strcmp(p->name, name) == 0) {
			return p->id;
		}
	}

	return -1;
}

/**
 * @brief Display a pin structure
 * 
 * @param pin the pin structure
 */
void print_pin(pin_t *pin)
{
	int i;

	printf("%s %4d ", pin->name, pin->gpio);
	for (i = 0; i < 31; ++i) {
		if ((pin->id >> i) & 1)
			break;
	}
	printf("%3d\n", i);
}

/**
 * @brief Display an array of pin structures
 * 
 * @param pins the pin structures array
 * @param n_pins the number of pins in the array
*/
void print_pins(pin_t **pins, int n_pins)
{
	printf("NAME  GPIO PIN\n");
	for (int i = 0; i < n_pins; ++i) {
		print_pin((*pins)+i);
	}
}

void set_pins_row_on_by_gpio(int gpio, int pins)
{
	BBBIO_GPIO_low(gpio, pins);
}

void set_pins_row_off_by_gpio(int gpio, int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void select_row_by_id_and_gpio(int gpio, int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void unselect_row_by_id_and_gpio(int gpio, int pins)
{
	printf("gpios: %d pins: %d \n", gpio, pins);
	BBBIO_GPIO_low(gpio, pins);
}

/* Generic */

void enable_gpios(void)
{
	for (int i = 0; i < N_GPIOS; ++i) {
		BBBIO_sys_Enable_GPIO(i);
	}
}

void set_ctrls(int *ctrls, pin_t *pins, int n_pins)
{
	/* set ctrl int to 0 by default */
	for (int i = 0; i < n_pins; ++i) {
		ctrls[i] = 0;
	}

	/* set bits for pins in ctrls */
	for (int i = 0; i < n_pins; ++i) {
		ctrls[pins[i].gpio] |= pins[i].id;
	}
}

void set_dir_pins_output(pin_t *pins, int n_pins)
{
	int ctrls[4];

	set_ctrls(ctrls, pins, n_pins);

	for (int i = 0; i < N_GPIOS; ++i){
		printf("%d ", ctrls[i]);
	}

	/* set pins as output for each gpio */
	for (int i = 0; i < N_GPIOS; ++i) {
		if (ctrls[i] != 0)
			BBBIO_GPIO_set_dir(i, 0, ctrls[i]);
	}
}

void select_row_by_pin(pin_t *pin)
{
	BBBIO_GPIO_high(pin->gpio, pin->id);
}

void unselect_row_by_pin(pin_t *pin)
{
	BBBIO_GPIO_low(pin->gpio, pin->id);
}

void unselect_rows(pin_t *pins, int n_pins)
{
	int ctrls[4];

	set_ctrls(ctrls, pins, n_pins);

	/* set pins off for each gpio gpio */
	for (int i = 0; i < N_GPIOS; ++i) {
		if (ctrls[i] != 0)
			unselect_row_by_id_and_gpio(i, ctrls[i]);
	}
}

void set_pins_row_on_for_level(matrix_t *m, pin_t *pins, int row_id, int level_id)
{
	/* we can't use set_ctrls here, as we need to check the level */
	int ctrls[4] = {0, 0, 0, 0};

	/* get pins to set on for each gpio */
	for (int i = 0; i < N_COLS; ++i) {
		if ((m->values)[row_id][i] <= level_id) {
			ctrls[pins[i].gpio] |= pins[i].id;
		}
	}

	/* set pins on by gpio */
	for (int i = 0; i < N_GPIOS; ++i) {
		set_pins_row_on_by_gpio(i, ctrls[i]);
	}
}

void set_pins_row_off(pin_t *pins, int n_pins)
{
	int ctrls[4];

	set_ctrls(ctrls, pins, n_pins);

	/* set pins off for each gpio */
	for (int i = 0; i < N_GPIOS; ++i) {
		set_pins_row_off_by_gpio(i, ctrls[i]);
	}
}
