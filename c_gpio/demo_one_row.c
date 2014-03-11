#include <stdio.h>
#include <stdlib.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "utils.h"

#define N_PINS 4
#define N_LEVELS 4

static float T = 1.42;

static int PINS[4] = {BBBIO_GPIO_PIN_13, BBBIO_GPIO_PIN_12, BBBIO_GPIO_PIN_15, BBBIO_GPIO_PIN_14};

static int PINS_LEVELS[4] = {0, 3, 2, 1};
static int LEVELS[4] = {0, 16, 8, 1};

int main()
{
	int c, p, l;
	int out;
	int current;

	/* init gpio */
	iolib_init();
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);

	/* init pins */
	for (p = 0; p < 4; p++)
		out |= PINS[p];

	BBBIO_GPIO_set_dir(BBBIO_GPIO1, 0, out);

	/* set all pins to low */
	set_pins_row_off(BBBIO_GPIO1, out);

	for (c = 0; c < 10000000; c++) {
		
		/* loop over levels */
		for (l = 1; l < N_LEVELS; l++) {
			out = 0;

			/* get current values */
			current = BBBIO_GPIO_get(BBBIO_GPIO1, BBBIO_GPIO_PIN_13 | BBBIO_GPIO_PIN_12 | BBBIO_GPIO_PIN_15 | BBBIO_GPIO_PIN_14);
	
			/* loop over pins */
			for (p = 0; p < N_PINS; p++) {
				if (PINS_LEVELS[p] >= l) {
					out |= PINS[p];
				}
			}

			/* set pins values */
			set_pins_row_on(BBBIO_GPIO1, out);
			set_pins_row_off(BBBIO_GPIO1, ~out);

			/*
				wait X us 
			 	we already waited for T/LEVELS[p-1], so let's subtract it
			*/

			pin_sys_delay_us(((T / LEVELS[l]) - (T / LEVELS[l-1])) * 1000);

		}
		set_pins_row_off(BBBIO_GPIO1, BBBIO_GPIO_PIN_13 | BBBIO_GPIO_PIN_12 | BBBIO_GPIO_PIN_15 | BBBIO_GPIO_PIN_14);
		pin_sys_delay_us((20 - T) * 1000);
	}

	/* bye */
	iolib_free();
	
	return 0;
}
