#ifndef _PINS_H_
#define _PINS_H_

#include "matrix.h"

#define T 1.42
#define N_LEVELS 4

#define LEVEL_0 1
#define LEVEL_1 4
#define LEVEL_2 16
#define LEVEL_3 0

typedef struct pin_t {
    const char *name;
    int gpio;
    int id;
} pin_t;

typedef struct level_t {
	int id;
	int tm;
} level_t;

void level_sleep(int level_id);
void level_usleep(double us);

int get_level_id(level_t level);
int get_level_time(level_t level);
int get_level_time_by_id(int level_id);

pin_t *get_pins_by_names(const char **names, int n_pins);
pin_t *get_pin_by_name(const char *name);

int get_pin_gpio_by_name(const char *name);
int get_pin_id_by_name(const char *name);

void print_pin(pin_t *pin);
void print_pins(pin_t **pins, int n_pins);

/* avoid headaches */
void unselect_row_by_id_and_gpio(int, int);
void select_row_by_id_and_gpio(int, int);
void set_pins_row_off_by_gpio(int, int);
void set_pins_row_on_by_gpio(int, int);


void enable_gpios(void);
void set_pins_dir_ouput(pin_t *pins, int n_pins);
void set_pins_dir_ouput(pin_t *pins, int n_pins);
void select_row_by_pin(pin_t *pins);
void unselect_row_by_pin(pin_t *pins);
void unselect_rows(pin_t *pins);

void set_pins_row_on_for_level(matrix_t *m, pin_t *pins, int row_id, int level_id);
void set_pins_row_off(pin_t *pins);

#endif
