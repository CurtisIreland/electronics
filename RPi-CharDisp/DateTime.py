#!/usr/bin/python

from Adafruit_CharLCD import Adafruit_CharLCD
from time import sleep, strftime
from datetime import datetime

lcd = Adafruit_CharLCD()
lcd.begin(16, 1)


lcd.clear()
d = datetime.now()

lcd.message(d.strftime('%b. %d, %Y\n'))
lcd.message(d.strftime('%I:%M:%S %p'))
