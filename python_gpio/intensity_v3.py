#!/usr/bin/env python2

from utils import *

# global variables 
T = 1.42
SWITCH = "P9_15"
PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
LEVELS = [2, 1, 0, 3]

# level -> period
dict_level_to_period = {
	0: 0,
	1: T/16,
	2: T/8,
	3: T
}

# associate level to pin
dict_pins_levels = dict(zip(PINS, LEVELS))

def get_dict_pins_periods():
	d = dict()

	for pin in PINS:
		d[pin] = dict_level_to_period[dict_pins_levels[pin]]

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

if __name__ == '__main__':
	try:
		print("Starting...")
		main()

	except KeyboardInterrupt:
		print("Exiting...")
		
		# switch off board
		set_pin_off(SWITCH)
		GPIO.cleanup()

		exit(0)
