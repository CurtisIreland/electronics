import machine, network
import network_secrets

wlan = network.WLAN()
wlan.active(True)

wlan.connect(network_secrets.net_id, network_secrets.net_pass)
while not wlan.isconnected():
    machine.idle()
    
print("WLAN Connected: ")
wlan.isconnected()

print('network config:', wlan.ipconfig('addr4'))