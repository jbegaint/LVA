#include <stdio.h>
#include <stdlib.h>

#include "../BBBIOlib/BBBio_lib/BBBiolib.h"
#include "pins.h"
#include "utils.h"

void set_pins_row_on(unsigned int gpio, unsigned int pins)
{
	BBBIO_GPIO_low(gpio, pins);
}

void set_pins_row_off(unsigned int gpio, unsigned int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void set_pins_col_on(unsigned int gpio, unsigned int pins)
{
	BBBIO_GPIO_high(gpio, pins);
}

void set_pins_col_off(unsigned int gpio, unsigned int pins)
{
	BBBIO_GPIO_low(gpio, pins);
}
