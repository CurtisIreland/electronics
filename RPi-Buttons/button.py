#!/usr/bin/python

# import needed python libraries
import RPi.GPIO as GPIO

# General Raspberry Pi Settings
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Configure GPIO pin 21 for OUTPUT
GPIO.setup(20,GPIO.IN)
GPIO.setup(21,GPIO.OUT)

while True:
  if (GPIO.input(20)):
    GPIO.output(21,GPIO.HIGH)
  else:
    GPIO.output(21,GPIO.LOW)

# Reset Raspberry Pi GPIO
GPIO.cleanup
