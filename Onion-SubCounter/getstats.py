#!/usr/bin/python

import serial
import requests

channelID = "YouTube Channel ID"
key = "Google API Key"
url = "https://www.googleapis.com/youtube/v3/channels?part=statistics&id=" + channelID + "&key=" + key

ser = serial.Serial('/dev/ttyS1')
response = requests.get(url)
data = response.json()
subscribe = data[u'items'][0][u'statistics'][u'subscriberCount']
# print(subscribe)

ser.write(subscribe.encode('ascii'))
ser.close()