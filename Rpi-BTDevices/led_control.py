#!/usr/bin/python3

import sys
import asyncio
from bleak import BleakClient

DEVICE = ""
CHARACTERISTIC = "19b10001-e8f2-537e-4f6c-d104768a1214"
STATE = b"\xff"

async def main():
    async with BleakClient(DEVICE) as client:
        await client.write_gatt_char(CHARACTERISTIC, STATE, response=True)


if len(sys.argv) < 3:
    print("Usage: {} <addr> on|off".format(sys.argv[0]))
    sys.exit(1)

DEVICE = sys.argv[1]
if sys.argv[2] == "on":
    STATE = b"\x00"

asyncio.run(main())