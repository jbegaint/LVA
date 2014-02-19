#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
from time import sleep
from random import randint

PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
T = 1.42
LEVELS = [3, 2, 1, 0]
FREQS = [0, T/16, T/4, T]

MATRIX = dict(zip(PINS, LEVELS))
CONV = dict(zip(LEVELS, FREQS))


GPIO.setup("P9_15", GPIO.OUT)
GPIO.output("P9_15", GPIO.LOW)

def setup_pin(pin):
	GPIO.setup(pin, GPIO.OUT)

def set_pin_on(pin):
	GPIO.output(pin, GPIO.LOW)

def set_pin_off(pin):
	GPIO.output(pin, GPIO.HIGH)

def msleep(sec):
	sleep(sec/1000.0)

[setup_pin(pin) for pin in PINS]
[set_pin_off(pin) for pin in PINS]

def get_tmp_matrix():
    d = dict()
    for i in PINS:
        d[i] = CONV[MATRIX[i]]
    return d

while 1:
    tmp = get_tmp_matrix()
    [set_pin_on(p) for p in PINS]
    for i in range(0, 17):
        msleep(T/16)

        for pin, freq in tmp.items():
            if (tmp[pin] > 0):
                tmp[pin] -= T/16
            if (tmp[pin] == 0):
                set_pin_off(pin)

    msleep(20-T)
