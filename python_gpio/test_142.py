#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
from time import sleep
from math import pow

PIN = "P9_11"
TIME = 1.42

GPIO.setup(PIN, GPIO.OUT)

GPIO.setup("P9_12", GPIO.OUT)
GPIO.setup("P9_13", GPIO.OUT)
GPIO.setup("P9_14", GPIO.OUT)

i = 0
j = 0
while True:

    T = TIME/pow(4, i)

    GPIO.output("P9_11", GPIO.LOW)
    sleep(T/1000.0)
   # GPIO.output("P9_12", GPIO.LOW)
   # sleep(T/2000.0)
   # GPIO.output("P9_13", GPIO.LOW)
   # sleep(T/1000.0)


    GPIO.output("P9_14", GPIO.HIGH)
    GPIO.output("P9_11", GPIO.HIGH)
    GPIO.output("P9_12", GPIO.HIGH)
    GPIO.output("P9_13", GPIO.HIGH)
    
    sleep((20-T)/1000.0)


    if (j%1000 == 0):
        i += 1
        if (i%2 == 0):
            i = 0
    j += 1

