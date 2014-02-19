# helpers for BBB GPIO

import Adafruit_BBIO.GPIO as GPIO
from time import sleep

def init_pin(pin):
	GPIO.setup(pin, GPIO.OUT)

def set_pin_on(pin):
	GPIO.output(pin, GPIO.LOW)

def set_pin_off(pin):
	GPIO.output(pin, GPIO.HIGH)

def msleep(sec):
	sleep(sec/1000.0)