#!/usr/bin/env python
 
# Written by Limor "Ladyada" Fried for Adafruit Industries, (c) 2015
# This code is released into the public domain
 
import time
import RPi.GPIO as GPIO
 
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
 
# read SPI data from MCP3008 chip, 8 possible adc's (0 thru 7)
def readadc(adcnum, clockpin, mosipin, misopin, cspin):
    if ((adcnum > 7) or (adcnum < 0)):
        return -1
    GPIO.output(cspin, True)
 
    GPIO.output(clockpin, False)  # start clock low
    GPIO.output(cspin, False)     # bring CS low
 
    commandout = adcnum
    commandout |= 0x18  # start bit + single-ended bit
    commandout <<= 3    # we only need to send 5 bits here
    for i in range(5):
        if (commandout & 0x80):
            GPIO.output(mosipin, True)
        else:
            GPIO.output(mosipin, False)
        commandout <<= 1
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
 
    adcout = 0
    # read in one empty bit, one null bit and 10 ADC bits
    for i in range(12):
        GPIO.output(clockpin, True)
        GPIO.output(clockpin, False)
        adcout <<= 1
        if (GPIO.input(misopin)):
            adcout |= 0x1
 
    GPIO.output(cspin, True)
        
    adcout >>= 1       # first bit is 'null' so drop it
    return adcout
 
# change these as desired - they're the pins connected from the
# SPI port on the ADC to the Cobbler
SPICLK = 22
SPIMISO = 23
SPIMOSI = 24
SPICS = 25
 
# set up the SPI interface pins
GPIO.setup(SPIMOSI, GPIO.OUT)
GPIO.setup(SPIMISO, GPIO.IN)
GPIO.setup(SPICLK, GPIO.OUT)
GPIO.setup(SPICS, GPIO.OUT)
 
# 10k trim pot connected to adc #0
potentiometer_adc = 0;
 
last_read = 0       # this keeps track of the last potentiometer value
tolerance = 5       # to keep from being jittery we'll only change
                    # volume when the pot has moved more than 5 'counts'
 
while True:
    # we'll assume that the pot didn't move
    trim_pot_changed = False
 
    # read the analog pin
    trim_pot = readadc(potentiometer_adc, SPICLK, SPIMOSI, SPIMISO, SPICS)
    # how much has it changed since the last read?
    pot_adjust = abs(trim_pot - last_read)
 
    if ( pot_adjust > tolerance ):
        trim_pot_changed = True
        last_read = trim_pot
 
    if ( trim_pot_changed ):
        print "trim_pot_changed", trim_pot_changed
        print "trim_pot:", trim_pot
        print "pot_adjust:", pot_adjust
        print "last_read", last_read
 
    # hang out and do nothing for a half second
    time.sleep(0.5)
