import time
import board
import neopixel
from random import seed
from random import choice

seed(5)

# On CircuitPlayground Express, and boards with built in status NeoPixel -> board.NEOPIXEL
# Otherwise choose an open pin connected to the Data In of the NeoPixel strip, i.e. board.D1
#pixel_pin = board.NEOPIXEL
pixel_pin = board.D4

# On a Raspberry pi, use this instead, not all pins are supported
# pixel_pin = board.D18

# The number of NeoPixels
num_pixels = 30

# The order of the pixel colors - RGB or GRB. Some NeoPixels have red and green reversed!
# For RGBW NeoPixels, simply change the ORDER to RGBW or GRBW.
ORDER = neopixel.GRB

pixels = neopixel.NeoPixel(
    pixel_pin, num_pixels, brightness=0.1, auto_write=False, pixel_order=ORDER
)

# Christmas colours
rgb_colours = [(0,0,0),(127,127,127),(127,0,0),(0,127,0)]

def colour_cycle(wait):
    col_start = 0
    for i in range(len(rgb_colours)):
        col_pos = col_start
        col_start = (col_start + 1) % len(rgb_colours)
        for j in range(num_pixels):
            pixels[j] = rgb_colours[col_pos]
            col_pos = (col_pos - 1) % len(rgb_colours)
        pixels.show()
        time.sleep(wait)

def colour_random(wait):
    for j in range(num_pixels):
        pixels[j] = choice(rgb_colours)
    pixels.show()
    time.sleep(wait)

while True:
# Select which colour function you wish to use
    colour_cycle(0.05)
#    colour_random(0.5)
