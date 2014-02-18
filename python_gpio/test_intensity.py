#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
from time import sleep
from random import randint

PINS = ["P9_11", "P9_12", "P9_13", "P9_14"]
T = 1.42
LEVELS = [0, 1, 2, 3]
FREQS = [0, T/16, T/8, T]

def setup_pin(pin):
	GPIO.setup(pin, GPIO.OUT)

def set_pin_on(pin):
	GPIO.output(pin, GPIO.LOW)

def set_pin_off(pin):
        #print("%s off" % pin)
	GPIO.output(pin, GPIO.HIGH)

def msleep(sec):
        #print("sleep %s" % sec)
	sleep(sec/1000.0)

[setup_pin(pin) for pin in PINS ]
[set_pin_off(pin) for pin in PINS ]

def set_line():
    for freq in FREQS:
        for pin in PINS:
            if (LEVELS[PINS.index(pin)] <= FREQS.index(freq)):
                set_pin_off(pin)
        msleep(freq)

c = 0
while 1:
    [set_pin_on(p) for p in PINS if LEVELS[PINS.index(p)] > 0]
    set_line()
    
    msleep(20 - sum(LEVELS))

    if (c == 50):
        c = 0
        #LEVELS = [randint(0, 3) for i in range(0, 4)]

    c += 1
