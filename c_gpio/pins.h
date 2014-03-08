#ifndef _PINS_H_
#define _PINS_H_

typedef struct pin_t {
    const char *name;
    int gpio;
    int id;
} pin_t;

pin_t *get_pins_by_names(const char **names, int n_pins);
pin_t *get_pin_by_name(const char *name);

int get_gpio_by_name(const char *name);
int get_id_by_name(const char *name);

void print_pin(pin_t *pin);

/* avoid headaches */
void set_pins_col_off(int, int);
void set_pins_col_on(int, int);
void set_pins_row_ff(int, int);
void set_pins_row_on(int, int);

#endif