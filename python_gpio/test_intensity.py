#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
from time import sleep

PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
T = 1.42

def setup_pin(pin):
	GPIO.setup(pin, GPIO.OUT)

def set_pin_on(pin):
	GPIO.output(pin, GPIO.LOW)

def set_pin_off(pin):
	GPIO.output(pin, GPIO.HIGH)

def msleep(sec):
	sleep(sec/1000.0)

[for pin in PINS setup_pin(pin)]
[for pin in PINS set_pin_off(pin)]

while 1:
	for i in range(0, 3):
		set_pin_on(PINS[i])
	msleep(T/4)

	set_pin_off(PINS[2])
	msleep(T/2)

	set_pin_off(PINS[1])
	msleep(T/2)

	set_pin_off(PINS[0])

	msleep(20-T-T/2-T/4)
