# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-cheap-yellow-display-board-cyd-esp32-2432s028r/
 
from machine import Pin, SPI, ADC, idle
import os
from time import sleep

# Save this file as ili9341.py https://github.com/rdagger/micropython-ili9341/blob/master/ili9341.py
from ili9341 import Display, color565
# Save this file as xglcd_font.py https://github.com/rdagger/micropython-ili9341/blob/master/xglcd_font.py
from xglcd_font import XglcdFont

# Function to set up SPI for TFT display
display_spi = SPI(1, baudrate=60000000, sck=Pin(14), mosi=Pin(13))
# Set up display
display = Display(display_spi, dc=Pin(2), cs=Pin(15), 
                  rst=Pin(15), width=320, height=240, rotation=0, bgr=False)

print('Display height: ' + str(display.height))
print('Display width: ' + str(display.width))

# Set colors (foreground) and background color
white_color = color565(255, 255, 255)  # white
black_color = color565(0, 0, 0)  # Black

# Turn on display backlight
backlight = Pin(21, Pin.OUT)
backlight.on()

# Clear display
display.clear(white_color)

def load_image():
#    display.draw_image('MicroPython128x128.raw', 0, 0, 128, 128)
    display.draw_image('BE-Tiny.raw', 111, 45, 98, 149)

try:
    load_image()  
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')