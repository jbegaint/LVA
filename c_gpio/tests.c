#include <stdio.h>
#include <stdlib.h>

#include "pins.h"

int main()
{
	int i;
	int n_pins = 3;

	const char *pins_names[3] = {"P9_11", "P9_12", "P9_28"};
	pin_t *pins = get_pins_by_names(pins_names, n_pins);

	for (i = 0; i < n_pins; ++i) {
		print_pin(pins+i);
	}

	free(pins);

	return 0;
}