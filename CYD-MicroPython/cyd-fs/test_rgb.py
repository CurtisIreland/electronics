# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-cheap-yellow-display-board-cyd-esp32-2432s028r/
 
from machine import Pin
import os
from time import sleep

# RGB LED at the back
red_led = Pin(4, Pin.OUT)
green_led = Pin(16, Pin.OUT)
blue_led = Pin(17, Pin.OUT)

# Turn on all LEDs (they are active low, so they work with inverted logic)
# Example: red_led.on() command turns the red LED off
red_led.on()
green_led.on()
blue_led.on()
sleep(3)

red_led.off()
sleep(3)

red_led.on()
green_led.off()
sleep(3)

green_led.on()
blue_led.off()
sleep(3)

blue_led.on()
