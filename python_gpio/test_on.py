#!/usr/bin/env python2
import Adafruit_BBIO.GPIO as GPIO
from time import sleep

from utils import *

PIN = "P8_24"

init_pin(PIN)
set_pin_off(PIN)
sleep(5)

GPIO.cleanup()

exit(0)
