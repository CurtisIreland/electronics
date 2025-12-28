from machine import Pin, PWM
from time import sleep
import random

# Initialize the three pins for the RGB LED
led = [PWM(4, freq=5000, duty_u16=65535),
    PWM(16, freq=5000, duty_u16=65535),
    PWM(17, freq=5000, duty_u16=65535)]

# Loop our fade in/fade out
while True:
    # Determine our RGB values (65535 = off)
    r_dutymax = random.randint(1,65536)
    g_dutymax = random.randint(1,65536)
    b_dutymax = random.randint(1,65536)
    print("RGB: ({0:d},{1:d},{2:d})".format(int((65535 - r_dutymax)/65535 * 255),int((65535 - g_dutymax)/65535 * 255),int((65535 - b_dutymax)/65535 * 255)))

    # Fade our RGB LED to our chosen values (+/- 2% each iteration)
    for i in range(101):
        if i < 50: # Calculate fade up
            r_duty = 65535 - (int(i * 2 * r_dutymax / 100)) # Red intensity going up
            g_duty = 65535 - (int(i * 2 * g_dutymax / 100)) # Green intensity going up
            b_duty = 65535 - (int(i * 2 * b_dutymax / 100)) # Blue intensity going up
        else: # Calculate fade down
            r_duty = 65535 - (r_dutymax - int((i - 50) * 2 * r_dutymax / 100)) # Red intensity going down
            g_duty = 65535 - (g_dutymax - int((i - 50) * 2 * g_dutymax / 100)) # Green intensity going down
            b_duty = 65535 - (b_dutymax - int((i - 50) * 2 * b_dutymax / 100)) # Blue intensity going down

        # Set our RGB intensity values
        led[0].duty_u16(r_duty) # LED_RED
        led[1].duty_u16(g_duty) # LED_GREEN
        led[2].duty_u16(b_duty) # LED_BLUE
        sleep(0.01)
