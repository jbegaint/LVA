#include <stdio.h>
#include <stdlib.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "utils.h"

void usage(char *str)
{
	die("usage: %s header pin delay\n", str);
}

int main(int argc, char **argv)
{
	int delay;
	int header;
	int pin;

	int c;

	if (argc != 4)
		usage(argv[0]);

	sscanf(argv[1], "%d", &header);
	sscanf(argv[2], "%d", &pin);
	sscanf(argv[3], "%d", &delay);

	iolib_init();
	iolib_setdir(header, pin, BBBIO_DIR_OUT);

	for (c = 0; c < 1000; ++c) {
		pin_high(header, pin);
		iolib_delay_ms(delay);

		pin_low(header, pin);
		iolib_delay_ms(delay);
	}

	iolib_free();

	return 0;
}
