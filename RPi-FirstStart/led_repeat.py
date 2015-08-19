#!/usr/bin/python

# import needed python libraries
import time
import RPi.GPIO as GPIO

# General Raspberry Pi Settings
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

# Configure GPIO pin 21 for OUTPUT
GPIO.setup(21,GPIO.OUT)

# Repeat endlessly
while 1:
  # Turn LED on
  GPIO.output(21,GPIO.HIGH)

  # Wait one second
  time.sleep(1)

  # Turn LED off
  GPIO.output(21,GPIO.LOW)

  # Wait one second
  time.sleep(1)
