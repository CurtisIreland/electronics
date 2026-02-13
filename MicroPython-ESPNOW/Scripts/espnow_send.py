import network
import espnow
import time
import struct
import random
from machine import I2C
import bme280_float as bme280

# Stats tracking
last_stats_time = time.time()
stats_interval = 10  # Print stats every 10 seconds

# Initialize Wi-Fi in station mode
sta = network.WLAN(network.STA_IF)
sta.active(True)
#sta.config(channel=1)  # Set channel explicitly if packets are not delivered
sta.disconnect()

# Initialize the BME280
i2c=I2C(0)
bme = bme280.BME280(i2c=i2c)

# Initialize ESP-NOW
e = espnow.ESPNow()
try:
    e.active(True)
except OSError as err:
    print("Failed to initialize ESP-NOW:", err)
    raise

# Receiver's MAC address
#receiver_mac = b'\x30\xae\xa4\xf6\x7d\x4c'
receiver_mac = b'\xff\xff\xff\xff\xff\xff' #broadcast

# Add peer
try:
    e.add_peer(receiver_mac)
except OSError as err:
    print("Failed to add peer:", err)
    raise

def print_stats():
    stats = e.stats()
    print("\nESP-NOW Statistics:")
    print(f"  Packets Sent: {stats[0]}")
    print(f"  Packets Delivered: {stats[1]}")
    print(f"  Packets Dropped (TX): {stats[2]}")
    print(f"  Packets Received: {stats[3]}")
    print(f"  Packets Dropped (RX): {stats[4]}")
    
# Main loop to send messages
while True:
    try:
        # Encode the data packet
        ident = 2
        (temp, press, humid) = bme.read_compensated_data()
        message = struct.pack('fffi', temp, humid, press, ident)
        # Send the message with acknowledgment
        try:
            if e.send(receiver_mac, message, True):
                print(f"Sent message: {temp} {humid} {press} 2")
            else:
                print("Failed to send message (send returned False)")
        except OSError as err:
            print(f"Failed to send message (OSError: {err})")
        
        # Print stats every 10 seconds
        if time.time() - last_stats_time >= stats_interval:
            print_stats()
            last_stats_time = time.time()
        
        time.sleep(random.randint(2, 6))  # Send every 2 - 5 seconds
        
    except OSError as err:
        print("Error:", err)
        time.sleep(5)
        
    except KeyboardInterrupt:
        print("Stopping sender...")
        e.active(False)
        sta.active(False)
        break