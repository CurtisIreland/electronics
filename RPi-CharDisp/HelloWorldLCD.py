#!/usr/bin/python

from Adafruit_CharLCD import Adafruit_CharLCD

lcd = Adafruit_CharLCD()

lcd.begin(16, 1)


lcd.clear()
lcd.message('Hello World\n')
lcd.message('From Me')
