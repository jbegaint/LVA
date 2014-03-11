#include <stdio.h>
#include <stdlib.h>

#include "pins.h"
#include "utils.h"

int main()
{
	int n_pins = 4;

	const char *pins_names[4] = {"P8_11", "P8_12", "P8_15", "P8_16"};
	pin_t *pins = get_pins_by_names(pins_names, n_pins);

	print_pins(&pins, n_pins);

	free(pins);

	return 0;
}