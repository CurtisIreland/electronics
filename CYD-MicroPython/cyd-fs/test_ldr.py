# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-cheap-yellow-display-board-cyd-esp32-2432s028r/
 
from machine import Pin, ADC
import os
from time import sleep

try:
    # Run the event loop indefinitely
    while True:
        # Read light sensor
        lightsensor = ADC(34, atten=ADC.ATTN_0DB)
        print('LDR value: ' + str(lightsensor.read_uv()))
        sleep(1)
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')