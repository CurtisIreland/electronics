#!/usr/bin/python3

import sys
import asyncio
from bleak import BleakClient

DEVICE = ""
CHARACTERISTIC = "19b10001-e8f2-537e-4f6c-d104768a1214"

async def main():
    async with BleakClient(DEVICE) as client:
        dev_value = await client.read_gatt_char(CHARACTERISTIC)
        print("Device Value: {0}".format(int(dev_value[0])))

if len(sys.argv) < 2:
    print("Usage: {} <addr>".format(sys.argv[0]))
    sys.exit(1)

DEVICE = sys.argv[1]

asyncio.run(main())