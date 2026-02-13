#
# Example. Using I2C at P9, P10
#
from machine import I2C
import bme280_float as bme280
from utime import sleep
i2c=I2C(0)
bme = bme280.BME280(i2c=i2c)
while True:
    print(bme.values)
    sleep(1)