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
                  width=320, height=240, rotation=0, bgr=False)

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

def draw_shapes():
    display.draw_hline(10, 40, 70, color565(255, 0, 255))
    sleep(1)

    display.draw_vline(10, 0, 40, color565(0, 255, 255))
    sleep(1)

    display.fill_hrect(23, 50, 30, 75, color565(255, 255, 255))
    sleep(1)

    display.draw_hline(0, 0, 100, color565(255, 0, 0))
    sleep(1)

    display.draw_line(50, 0, 64, 40, color565(255, 255, 0))
    sleep(2)

    display.clear()

    coords = [[0, 63], [78, 80], [122, 92], [50, 50], [78, 15], [0, 63]]
    display.draw_lines(coords, color565(0, 255, 255))
    sleep(1)

    display.clear()
    display.fill_polygon(7, 120, 120, 100, color565(0, 255, 0))
    sleep(1)

    display.fill_rectangle(0, 0, 15, 227, color565(255, 0, 0))
    sleep(1)

    display.clear()

    display.fill_rectangle(0, 0, 163, 163, color565(128, 128, 255))
    sleep(1)

    display.draw_rectangle(0, 64, 163, 163, color565(255, 0, 255))
    sleep(1)

    display.fill_rectangle(64, 0, 163, 163, color565(128, 0, 255))
    sleep(1)

    display.draw_polygon(3, 120, 110, 30, color565(0, 64, 255), rotate=15)
    sleep(3)

    display.clear()

    display.fill_circle(132, 132, 70, color565(0, 255, 0))
    sleep(1)

    display.draw_circle(132, 96, 70, color565(0, 0, 255))
    sleep(1)

    display.fill_ellipse(96, 96, 30, 16, color565(255, 0, 0))
    sleep(1)

    display.draw_ellipse(96, 85, 16, 30, color565(255, 255, 0))

    sleep(5)
    display.cleanup()

try:
    draw_shapes()  
except Exception as e:
    print('Error occured: ', e)
except KeyboardInterrupt:
    print('Program Interrupted by the user')        
finally:
    display.cleanup()