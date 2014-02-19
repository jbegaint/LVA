#!/usr/bin/env python2
import Adafruit_BBIO.GPIO as GPIO
from time import sleep

from utils import *

PIN = "P9_21"

init_pin(PIN)
set_pin_on(PIN)
sleep(5)

GPIO.cleanup()

exit(0)
