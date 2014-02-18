#!/usr/bin/env python2

from utils import *

T = 1.42
SWITCH = "P9_15"
PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
LEVELS = [0, 1, 2, 3]
PERIODS = [0, T/16, T/8, T]

dict_pins_levels = dict(zip(PINS, LEVELS))
dict_levels_periods = dict(zip(LEVELS, PERIODS))


def get_dict_pins_periods():
	d = dict()
	for pin in PINS:
		d[pin] = dict_levels_periods[dict_pins_levels[pin]]

	return d

def main():

	# init board
	init_pin(SWITCH)
	set_pin_on(SWITCH)

	# init pins
	[init_pin(p) for p in PINS]

	while 1:
		mat = get_dict_pins_periods()
		[set_pin_on(p) for p in PINS]

		for i in range(0, 17):

			[set_pin_off(p) for p in PINS if (mat[p] == i*T/16)]

			msleep(T/16)

		msleep(20 - T)

try:
	print("Starting...")
	main()

except KeyboardInterrupt:
	print("Exiting...")
	
	# switch off board
	set_pin_off(SWITCH)

	GPIO.cleanup()
	exit(0)
