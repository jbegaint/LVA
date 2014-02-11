#!/usr/bin/env python2

import Adafruit_BBIO.GPIO as GPIO
import sys

if (len(sys.argv) != 3):
    print("Syntax error")
    exit(1)

GPIO.setup(sys.argv[1], GPIO.OUT)

if (sys.argv[2] == "on"):
    GPIO.output(sys.argv[1], GPIO.HIGH)
elif (sys.argv[2] == "off"):
    GPIO.output(sys.argv[1], GPIO.LOW)
else:
    print("Syntax error")
    exit(1)

exit(0)
