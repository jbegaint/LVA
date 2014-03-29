#!/usr/bin/env python2
# matrix 7 row, 5 cols

from utils import *
from time import sleep

# global variables 
T = 1.42
SWITCH = "P9_15"
PINS = ["P9_11", "P9_12", "P9_13", "P9_14", "P9_15"]
PINS_ROWS = ["P9_21", "P9_22", "P9_23", "P9_24", "P9_21", "P9_26", "P9_27"]
LEVELS = [0, 1, 2, 3, 0]
N_COLS = 7

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

	# init pins
	[init_pin(p) for p in PINS + PINS_ROWS]
	[set_pin_off(p) for p in PINS + PINS_ROWS]


        while 1:
		for pin in PINS_ROWS:
                	set_pin_on(pin)

			mat = get_dict_pins_periods()
			[set_pin_on(p) for p in PINS if dict_pins_levels[p] > 0]

			for i in range(0, 17):
				[set_pin_off(p) for p in PINS if (mat[p] == i*T/16)]
				msleep(T/16)

			msleep(20/N_COLS - T)

			set_pin_off(pin)

if __name__ == '__main__':
	try:
		print("Starting...")
		main()

	except KeyboardInterrupt:
		print("Exiting...")
		
		# switch off board
                [set_pin_on(p) for p in PINS+PINS_ROWS]
		GPIO.cleanup()

		exit(0)
