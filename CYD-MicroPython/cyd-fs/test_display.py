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
display = Display(display_spi, dc=Pin(2), cs=Pin(15), rst=Pin(15),
                  width=320, height=240, rotation=0)

def draw_text():
    # Set colors
    white_color = color565(255, 255, 255)  # white color
    black_color = color565(0, 0, 0)        # black color

    # Turn on display backlight
    backlight = Pin(21, Pin.OUT)
    backlight.on()

    # Clear display
    display.clear(black_color)

    # Draw the text on (0, 0) coordinates (x, y, text, font color, font background color, rotation)
    display.draw_text8x8(0, 0, 'ESP32 says hello!', white_color, black_color, 0)
    
    # Draw the text on the center of the display
    font_size = 8
    text_msg = 'Centered text'
    x_center = int((display.width-len(text_msg)*font_size)/2)
    y_center = int(((display.height)/2)-(font_size/2))
    display.draw_text8x8(x_center, y_center, text_msg, white_color, black_color, 0)
    
    # Draw the text on the right with rotation
    display.draw_text8x8(display.width-font_size, 0, 'Text with rotation', white_color, black_color, 90)

try:
    draw_text()
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')        
    display.cleanup()