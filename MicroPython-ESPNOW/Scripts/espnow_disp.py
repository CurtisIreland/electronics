# Rui Santos & Sara Santos - Random Nerd Tutorials
# Complete project details at https://RandomNerdTutorials.com/micropython-esp-now-esp32/

import network
import espnow
import time
import struct
from machine import Pin, I2C
import ssd1306


'''
float temp
float humid
float press
int ident
'''

# Stats tracking
last_stats_time = time.time()
stats_interval = 10  # Print stats every 10 seconds

# Initialize the OLED display
i2c = I2C(sda=Pin(5), scl=Pin(6))
display = ssd1306.SSD1306_I2C(72, 40, i2c)

# Initialize Wi-Fi in station mode
sta = network.WLAN(network.STA_IF)
sta.active(True)
sta.config(channel=1)  # Set channel explicitly if packets are not received
sta.disconnect()

# Initialize ESP-NOW
e = espnow.ESPNow()
try:
    e.active(True)
except OSError as err:
    print("Failed to initialize ESP-NOW:", err)
    raise

def print_stats():
    stats = e.stats()
    print("\nESP-NOW Statistics:")
    print(f"  Packets Sent: {stats[0]}")
    print(f"  Packets Delivered: {stats[1]}")
    print(f"  Packets Dropped (TX): {stats[2]}")
    print(f"  Packets Received: {stats[3]}")
    print(f"  Packets Dropped (RX): {stats[4]}")

print("Listening for ESP-NOW messages...")
while True:
    try:
        # Receive message (host MAC, message, timeout of 10 seconds)
        host, msg = e.recv(10000)

        if msg:
            # Show raw received data
#            print(f"Received from {host.hex()}: {msg.hex()}")

            # Read and format the incoming values
            (temp, humid, press, ident) = struct.unpack('<fffi', msg)
            temp_str = "%.1f C" % temp
            humid_str = "%.1f %%" % humid
            press_str = "%.1fhPa" % press

            # Output values to serial output
            print(temp)
            print(humid)
            print(press)
            print(ident)
            print("-----")

            # Display values on the OLED
#            display.fill_rect(0, 0, 71, 39, 0)
            display.fill(0)
            display.text(temp_str, 0, 1, 1)
            display.text(humid_str, 0, 16, 1) 
            display.text(press_str, 0, 31, 1) 
            display.show()

        # Print stats every 10 seconds
        if time.time() - last_stats_time >= stats_interval:
            print_stats()
            last_stats_time = time.time()
        
    except OSError as err:
        print("Error:", err)
        time.sleep(5)
        
    except KeyboardInterrupt:
        print("Stopping receiver...")
        e.active(False)
        sta.active(False)
        break
