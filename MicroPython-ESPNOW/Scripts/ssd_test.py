from machine import Pin, I2C
import ssd1306

# using default address 0x3C
i2c = I2C(sda=Pin(5), scl=Pin(6))
display = ssd1306.SSD1306_I2C(72, 40, i2c)

display.text('Hello, World!', 0, 0, 1)
display.show()