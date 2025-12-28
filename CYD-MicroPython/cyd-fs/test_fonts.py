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
    display.clear(white_color)

    # Loading Unispace font
    print('Loading Unispace font...')
    unispace_font = XglcdFont('fonts/Unispace12x24.c', 12, 24)
    
    # Draw the text on (0, 0) coordinates (x, y, text, font,  font color, font background color,
    #                                      landscape=False, rotate_180=False, spacing=1)
    display.draw_text(0, 0, 'ESP32 says hello!', unispace_font, black_color, white_color)

    # Draw the text on the center of the display
    font_size_w = unispace_font.width
    font_size_h = unispace_font.height
    text_msg = 'Centered text'
    x_center = int((display.width-len(text_msg)*font_size_w)/2)
    y_center = int(((display.height)/2)-(font_size_h/2))
    display.draw_text(x_center, y_center, text_msg, unispace_font, black_color, white_color)
    
    # Draw the text with rotation
    display.draw_text(display.width-font_size_h, display.height-font_size_w, 'Text with rotation',
                      unispace_font, black_color, white_color, landscape=True)

try:
    draw_text()
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')        
    display.cleanup()