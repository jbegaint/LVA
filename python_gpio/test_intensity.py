#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
from time import sleep
from random import randint

RANDOM = False

PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
T = 1.42
LEVELS = [3, 2, 1, 0]
FREQS = [0, T/16, T/4, T]

MATRIX = dict(zip(PINS, LEVELS))
CONV = dict(zip(FREQS, LEVELS))

def setup_pin(pin):
	GPIO.setup(pin, GPIO.OUT)

def set_pin_on(pin):
	GPIO.output(pin, GPIO.LOW)

def set_pin_off(pin):
	GPIO.output(pin, GPIO.HIGH)

def msleep(sec):
	sleep(sec/1000.0)

[setup_pin(pin) for pin in PINS ]
[set_pin_off(pin) for pin in PINS ]

def set_line():
    for freq in FREQS:
        [set_pin_off(p) for p in PINS if (MATRIX[p] <= CONV[freq])]
        msleep(freq)

while 1:
    [set_pin_on(p) for p in PINS if MATRIX[p] > 0]
    set_line()
    
    msleep(20 - sum(LEVELS))
