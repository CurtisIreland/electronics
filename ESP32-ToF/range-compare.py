# Simple demo of the VL53L0X distance sensor.
# Will print the sensed range/distance every second.
import time

import board
import busio

import adafruit_vl53l0x
import adafruit_hcsr04

# Initialize I2C bus and sensor.
i2c = busio.I2C(board.IO9, board.IO8)
vl53 = adafruit_vl53l0x.VL53L0X(i2c)
sonar = adafruit_hcsr04.HCSR04(trigger_pin=board.IO5, echo_pin=board.IO6)

# Optionally adjust the measurement timing budget to change speed and accuracy.
# See the example here for more details:
#   https://github.com/pololu/vl53l0x-arduino/blob/master/examples/Single/Single.ino
# For example a higher speed but less accurate timing budget of 20ms:
# vl53.measurement_timing_budget = 20000
# Or a slower but more accurate timing budget of 200ms:
# vl53.measurement_timing_budget = 200000
# The default timing budget is 33ms, a good compromise of speed and accuracy.
print(f"Timing budget: {vl53.measurement_timing_budget}")
vl53.measurement_timing_budget = 250000
print(f"Timing budget: {vl53.measurement_timing_budget}")

# Main loop will read the range and print it every second.

while True:
    print(f"LoF Range: {vl53.range}mm")
    print(f"Sonar Range: {sonar.distance}cm\n")
    time.sleep(2.0)
