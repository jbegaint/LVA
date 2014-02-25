#include <stdio.h>
#include <stdlib.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "utils.h"

/* 
	pin name conversion table
	P9_11, gpio0[30]
	P9_12, gpio1[28]
	P9_13, gpio0[31]
	P9_14, gpio1[18]
	
	P9_28, gpio3[17]
	P9_29, gpio3[15]
	P9_30, gpio3[16]
	P9_31, gpio3[14]
*/

#define N_PINS 4
#define N_LEVELS 4

static float T = 1.42;

static int PINS[4] = {BBBIO_GPIO_PIN_17, BBBIO_GPIO_PIN_15, BBBIO_GPIO_PIN_16, BBBIO_GPIO_PIN_14};

static int PINS_LEVELS[4] = {0, 3, 2, 1};
static int LEVELS[4] = {0, 16, 8, 1};

/* pin to level time */
int gpio_ptlt(int pin_id)
{
	int pin_level = PINS_LEVELS[pin_id];

	return LEVELS[pin_level];
}

int main()
{
	int c, p, l;
	int out;
	int current;
	float time_elapsed;

	/* init gpio */
	iolib_init();
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO3);


	/* init pins */
	for (p = 0; p < 4; p++)
		out |= PINS[p];
	BBBIO_GPIO_set_dir(BBBIO_GPIO3, out, 0);

	/* set all pins to low */
	BBBIO_GPIO_high(BBBIO_GPIO3, out);

	for (c = 0; c < 10000; c++) {
		time_elapsed = 0;
		
		/* loop over levels */
		for (l = 0; l < N_LEVELS; l++) {
			out = 0;

			/* get current values */
			current = BBBIO_GPIO_get(BBBIO_GPIO3, BBBIO_GPIO_PIN_17 | BBBIO_GPIO_PIN_16 | BBBIO_GPIO_PIN_15 | BBBIO_GPIO_PIN_14);
	
			/* loop over pins */
			for (p = 0; p < N_PINS; p++) {
				/* check if pin is on, and set next status */
				if ((PINS[p] & current) && (gpio_ptlt(p) > LEVELS[l]))
					out |= PINS[p];
			}

			/* set pins values */
			BBBIO_GPIO_low(BBBIO_GPIO3, out);
			BBBIO_GPIO_high(BBBIO_GPIO3, ~out);

			/* prevent 0 div */
			if (LEVELS[l] == 0)
				continue;
			/* 
				wait X us 
			 	we already waited for T/LEVELS[p-1], so let's subtract it
			*/

			BBBIO_sys_delay_us(((T / LEVELS[l]) - time_elapsed) * 1000);
			time_elapsed = T / LEVELS[l];

		}
		BBBIO_sys_delay_us((20 - T) * 1000);
	}

	/* bye */
	iolib_free();
	return 0;
}