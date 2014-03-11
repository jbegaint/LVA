#include <stdio.h>
#include <stdlib.h>

#include "pins.h"

int main()
{
	int i;
	int n_pins = 4;

	const char *pins_names[4] = {"P8_11", "P8_12", "P8_15", "P8_16"};
	pin_t *pins = get_pins_by_names(pins_names, n_pins);


	printf("NAME  GPIO PIN\n");
	for (i = 0; i < n_pins; ++i) {
		print_pin(pins+i);
	}

	free(pins);

	return 0;
}