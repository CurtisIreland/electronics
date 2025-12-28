# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-cheap-yellow-display-board-cyd-esp32-2432s028r/
 
from machine import Pin, SPI, ADC, idle
import os
from time import sleep

# Save this file as ili9341.py https://github.com/rdagger/micropython-ili9341/blob/master/ili9341.py
from ili9341 import Display, color565
# Save this file as xpt2046.py https://github.com/rdagger/micropython-ili9341/blob/master/xpt2046.py
from xpt2046 import Touch
# Save this file as xglcd_font.py https://github.com/rdagger/micropython-ili9341/blob/master/xglcd_font.py
from xglcd_font import XglcdFont

# Function to set up SPI for TFT display
display_spi = SPI(1, baudrate=60000000, sck=Pin(14), mosi=Pin(13))
# Set up display
display = Display(display_spi, dc=Pin(2), cs=Pin(15), 
                  rst=Pin(15), width=320, height=240, rotation=0)

print('Display height: ' + str(display.height))
print('Display width: ' + str(display.width))

# Set colors (foreground) and background color
white_color = color565(255, 255, 255)  # white
black_color = color565(0, 0, 0)  # Black

# Turn on display backlight
backlight = Pin(21, Pin.OUT)
backlight.on()

# Clear display
display.clear(black_color)

# Initial message
# Draw the text on the center of the display
font_size = 8
text_msg = 'Touch screen to test'
x_center = int((display.width-len(text_msg)*font_size)/2)
y_center = int(((display.height)/2)-(font_size/2))

display.draw_text8x8(x_center, y_center,text_msg, white_color, black_color, 0)

# SPI for touchscreen
touchscreen_spi = SPI(2, baudrate=1000000, sck=Pin(25), mosi=Pin(32), miso=Pin(39))

def touchscreen_press(x, y):
    display.clear(black_color)
    text_touch_coordinates = "Touch: X = " + str(x) + " | Y = " + str(y)
    x_center = int((display.width-len(text_touch_coordinates)*font_size)/2)
    display.draw_text8x8(x_center, y_center, text_touch_coordinates, white_color, black_color, 0)
    print("Touch: X = " + str(x) + " | Y = " + str(y))

touchscreen = Touch(touchscreen_spi, cs=Pin(33), int_pin=Pin(36), int_handler=touchscreen_press)

try:
    # Run the event loop indefinitely
    while True:
        # Loop to wait for touchscreen press
        touchscreen.get_touch()   
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')        
finally:
    display.cleanup()