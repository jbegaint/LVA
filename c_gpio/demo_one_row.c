#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "pins.h"
#include "utils.h"

static int running = 1;
static int PINS_LEVELS[4] = {0, 1, 2, 3};

extern int LEVELS[4];

static const char *pins_names[] = {"P8_11", "P8_12", "P8_15", "P8_16"};
static const pin_t *pins; 
static int N_PINS = ARRAY_SIZE(pins_names);

void handler(int sig)
{
	/* yes i know, we only catch SIGINT ATM.. */
	if (sig == SIGINT) {
		running = 0;
		printf("Exiting...\n");
	}
}

void setup(int *ctrl)
{
	int p;

	/* init gpio */
	iolib_init();
	BBBIO_sys_Enable_GPIO(BBBIO_GPIO1);

	/* get pins ctrl */
	for (p = 0; p < N_PINS; ++p) {
		*ctrl |= (pins[p]).id;
	}

	/* set pins as output */
	BBBIO_GPIO_set_dir(BBBIO_GPIO1, 0, *ctrl);

	/* set all pins to low */
	set_pins_row_off_by_gpio(BBBIO_GPIO1, *ctrl);
}

void cleanup(void)
{
	iolib_free();
}

int get_pins_to_set_on(int level_id)
{
	int out, p;

	/* loop over pins */
	for (p = 0; p < N_PINS; ++p) {
		if (PINS_LEVELS[p] < level_id) {
			out |= (pins[p]).id;
		}
	}

	return out;
}

void switch_leds(int ctrl)
{
	int l, out;
	
	while (running) {
		
		/* loop over levels */
		/* N_LEVELS-2, indeed no need to check level 3 (off)*/
		for (l = N_LEVELS - 2; l > 0 ; --l) {

			/* todo: fct to set on/off independent of gpios */
			out = get_pins_to_set_on(l);

			/* set pins values */
			set_pins_row_on_by_gpio(BBBIO_GPIO1, out);
			set_pins_row_off_by_gpio(BBBIO_GPIO1, ctrl & (~out));

			/*
				wait X us 
			 	we already waited for T/LEVELS[l-1], so let's subtract it
			*/

			level_sleep(l);

		}
		set_pins_row_off_by_gpio(BBBIO_GPIO1, ctrl);
		iolib_delay_ms((20 - T));
	}

	/* end of loop, exit */
	cleanup();
}

int main()
{
	int ctrl;
	
	pins = get_pins_by_names(pins_names, N_PINS);

	setup(&ctrl);
	switch_leds(ctrl);

	signal(SIGINT, handler);

	return 0;
}
