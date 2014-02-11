#!/usr/bin/env python2
#

import Adafruit_BBIO.GPIO as GPIO
from time import sleep

print("Hello World")

tab = ["P9_11", "P9_12", "P9_13", "P9_14"]

# init GPIO pins
GPIO.setup("P9_11", GPIO.OUT)
GPIO.setup("P9_12", GPIO.OUT)
GPIO.setup("P9_13", GPIO.OUT)
GPIO.setup("P9_14", GPIO.OUT)

i = 0

while True:
    print("Pin %s on, pin %s off" % (tab[i], tab[i-1]))
    GPIO.output(tab[i], GPIO.HIGH)
    GPIO.output(tab[i-1], GPIO.LOW)
    sleep(0.2)

    i += 1

    if (i%4 == 0):
        i = 0


# clean gpio for exit
GPIO.cleanup()

exit(0)
