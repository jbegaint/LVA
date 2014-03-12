#ifndef _PINS_H_
#define _PINS_H_

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

int get_level_id(level_t level);
int get_level_time(level_t level);
int get_level_time_by_id(int level_id);

pin_t *get_pins_by_names(const char **names, int n_pins);
pin_t *get_pin_by_name(const char *name);

int get_gpio_by_name(const char *name);
int get_id_by_name(const char *name);

void print_pin(pin_t *pin);
void print_pins(pin_t **pins, int n_pins);

/* avoid headaches */
void set_pins_col_off_by_gpio(int, int);
void set_pins_col_on_by_gpio(int, int);
void set_pins_row_off_by_gpio(int, int);
void set_pins_row_on_by_gpio(int, int);

#endif
