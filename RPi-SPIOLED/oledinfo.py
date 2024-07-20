#!/usr/bin/python3
# -*- coding: utf-8 -*-

"""oledinfo.py: Show system information on the SH1106 OLED display"""

__author__     = "Frank Bernhardt"
__copyright__  = "Copyright 2024, My Raspberry Pi NAS Project"
__version__    = "1.0.0"

from luma.core.interface.serial import spi
from luma.core.render import canvas
from luma.core.error import DeviceNotFoundError
from luma.oled.device import sh1106
from PIL import ImageFont

import os
import time
import psutil
import subprocess

def show_omvlogo(ttl):

    font = ImageFont.truetype('DejaVuSans.ttf', 11)

    start_time = time.time()

    while True:

        with canvas(device) as draw:

            x1 = 0  ; x2 = x1 + 30
            y1 = 18 ; y2 = y1 + 29
            draw.rectangle([x1,y1,x2,y2], outline="white", fill="black")

            x1 += 4 ; y1 += 5
            draw.rectangle((x1,y1,x1+22,y1+5), fill="white")

            y1 += 7
            draw.rectangle((x1,y1,x1+22,y1+5), fill="white")

            y1 += 7
            draw.rectangle((x1,y1,x1+22,y1+5), fill="white")

            x1 += 2 ; y1 -= 13
            draw.ellipse((x1, y1, x1+3, y1+3), fill="white", outline="black")

            y1 += 7
            draw.ellipse((x1, y1, x1+3, y1+3), fill="white", outline="black")

            y1 += 7
            draw.ellipse((x1, y1, x1+3, y1+3), fill="white", outline="black")

            draw.text((35, 25), "openmediavault", font=font, fill="white")

        time.sleep(ttl/10)
        if time.time() > start_time + ttl:
            break


def show_sysinfo(ttl):
    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf', 9)

    start_time = time.time()

    while True:

        with canvas(device) as draw:

            x1 = 2 ; y1 = 2

            # Get hostname
            cmd = "hostname"
            hostname = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            draw.text((x1, y1), "Hostname: " + hostname, font=font, fill="white")

            # Get IP address (if any)
            cmd = "hostname -I | grep -Eo \'[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}|([0-9a-fA-F]{0,4}:){1,7}[0-9a-fA-F]{0,4}\'"
            try:
                ipaddr = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            except:
                ipaddr = "none"
            ipaddr = ipaddr.strip()
            draw.text((x1, y1+16), "IP: " + ipaddr, font=font, fill="white")

            # Get CPU temperature
            cmd = "vcgencmd measure_temp | cut -d= -f2 | cut -d\\' -f1"
            cputemp = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            cputemp = cputemp.strip()
            draw.text((x1, y1+31), "Temperature: " + str(cputemp) + " °C", font=font, fill="white")

            # Get uptime
            cmd = "cat /proc/uptime | cut -d\' \' -f1"
            uptime = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            uptime = uptime.strip()

            # Less than 2 hours?
            if float(uptime) < (120 * 60):
                uptimestr = str(round(float(uptime)/60)) + " minutes"
            # Less than 48 hours?
            elif float(uptime) < (48 * 60 * 60):
                uptimestr = str(round(float(uptime)/60/60)) + " hours"
            # More than 2 days
            else:
                uptimestr = str(round(float(uptime)/60/60/24)) + " days"

            draw.text((x1, y1+46), "Uptime: " + uptimestr, font=font, fill="white")

        time.sleep(ttl/10)
        if time.time() > start_time + ttl:
            break


def show_cpuinfo(ttl):

    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf', 9)

    start_time = time.time()

    while True:

        with canvas(device) as draw:

            x1 = 2 ; y1 = 2

            # Collect CPU info
            cmd = "lscpu | grep 'Vendor ID:' | cut -d: -f2 | sed 's/^ *//'"
            cputype = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            cputype = cputype.strip()
            cmd = "lscpu | grep 'Model name:' | cut -d: -f2 | sed 's/^ *//'"
            cpumodel = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            cpumodel = cpumodel.strip()

            # Show CPU Vendor and Model
            draw.text((x1, y1), "CPU: " + str(cputype) + " " + str(cpumodel), font=font, fill="white")

            # Collect more CPU info
            cpucount = psutil.cpu_count(logical = True)
            cmd = "lscpu | grep 'CPU max MHz:' | cut -d: -f2 | sed 's/^ *//' | cut -d. -f1"
            cpuspeed = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            cpuspeed = cpuspeed.strip()

            # Show CPU cores and speed
            draw.text((x1, y1+17), str(cpucount) + " CPU Cores @ " + str(cpuspeed) + " MHz", font=font, fill="white")

            # Get CPU load
            cmd = "top -bn1 | grep '%Cpu(s)' | cut -d, -f4 | sed 's/ //' | sed 's/id//'"
            cpuload = subprocess.check_output(cmd, shell = True, encoding='utf-8')
            cpuload = cpuload.strip()
            cpuload = round(100-float(cpuload),1)

            # Show usage in percent
            draw.text((x1, y1 + 33), "CPU Usage:", font=font, fill="white")
            draw.text((x1+58, y1 + 33), str(cpuload) + " %", font=font, fill="white")

            # Show usage bar
            draw.rectangle((x1, y1+48, x1+120, y1+58), outline="white", fill="black")
            draw.rectangle((x1, y1+48, x1+(cpuload*1.2), y1+58), outline="white", fill="white")

        time.sleep(ttl/10)
        if time.time() > start_time + ttl:
            break


def show_meminfo(ttl):
    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf', 9)

    start_time = time.time()

    while True:

        with canvas(device) as draw:

            x1 = 2 ; y1 = 2

            # Calculate memory information
            memory = psutil.virtual_memory()
            # Convert Bytes to MB (Bytes -> KB -> MB)
            mem_avail = round(memory.available/1024.0/1024.0,1)
            mem_total = round(memory.total/1024.0/1024.0,1)

            # Show total memory
            draw.text((x1, y1), "Memory Total:", font=font, fill="white")
            draw.text((x1+73, y1), str(mem_total) + " MB", font=font, fill="white")

            # Show free memory
            draw.text((x1, y1+17), "Memory Free:", font=font, fill="white")
            draw.text((x1+73, y1+17), str(mem_avail) + " MB", font=font, fill="white")

            # Show usage in percent
            draw.text((x1, y1+33), "Memory Usage:", font=font, fill="white")
            draw.text((x1+73, y1+33), str(memory.percent) + " %", font=font, fill="white")

            # Show usage bar
            draw.rectangle((x1, y1+48, x1+120, y1+58), outline="white", fill="black")
            draw.rectangle((x1, y1+48, x1+(memory.percent*1.2), y1+58), outline="white", fill="white")

        time.sleep(ttl/10)
        if time.time() > start_time + ttl:
            break


def show_diskinfo(ttl):
    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf', 9)

    # Build a list of mountpoints starting with /
    mountpoints=["/"]

    # Get a list of filesystems from openmediavault config
    omv_disks = ""
    if os.path.isfile("/etc/openmediavault/config.xml"):
        cmd = "sudo grep '<dir>/srv/' /etc/openmediavault/config.xml | sed -e 's/.*>\(.*\)<.*/\\1/'"
        omv_disks = subprocess.check_output(cmd, shell=True, encoding='utf-8')
        omv_disks = omv_disks.strip()

    # Add each openmediavault filesystem to the list of mountpoints
    if omv_disks != "": mountpoints.extend(omv_disks.splitlines())

    # Enumerate and show each disk for the ttl
    for index, mountpoint in enumerate(mountpoints):

        with canvas(device) as draw:

            x1 = 2 ; y1 = 2

            # Calculate disk information
            disk = psutil.disk_usage(mountpoint)
            # Convert Bytes to GB (Bytes -> KB -> MB -> GB)
            disk_free = round(disk.free/1024.0/1024.0/1024.0,1)
            disk_size = round(disk.total/1024.0/1024.0/1024.0,1)

            # Show total disk space
            draw.text((x1, y1), "Disk " + str(index) + " Total:", font=font, fill="white")
            draw.text((x1+75, y1), str(disk_size) + " GB", font=font, fill="white")

            # Show free disk space
            draw.text((x1, y1+17), "Disk " + str(index) + " Free:", font=font, fill="white")
            draw.text((x1+75, y1+17), str(disk_free) + " GB", font=font, fill="white")

            # Show usage in percent
            draw.text((x1, y1+33), "Disk " + str(index) + " Usage:", font=font, fill="white")
            draw.text((x1+75, y1+33), str(disk.percent) + " %", font=font, fill="white")

            # Show usage bar
            draw.rectangle((x1, y1+48, x1+120, y1+58), outline="white", fill="black")
            draw.rectangle((x1, y1+48, x1+(disk.percent*1.2), y1+58), outline="white", fill="white")

        time.sleep(ttl)


def show_clock(ttl):
    font = ImageFont.truetype('/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf', 36)

    start_time = time.time()

    while True:

        with canvas(device) as draw:

            x1 = 12 ; y1 = 12

            # Show current time
            draw.text((x1, y1), time.strftime("%H:%M",time.localtime()), font=font, fill="white")

        time.sleep(ttl/10)
        if time.time() > start_time + ttl:
            break


def main():

    while True:
        # Set display contrast
#        if ((int(time.strftime("%H")) > 6) and (int(time.strftime("%H")) < 22)):
#            # Contrast normal between 07:00 and 22:00
#            device.contrast(128)
        device.contrast(128)
#        else:
#            # Contrast low between 22:00 and 07:00
#            device.contrast(32)

        # Rotate info screens for 10 seconds each
#        if os.path.isfile("/etc/openmediavault/config.xml"):
#            show_omvlogo(10)

        show_sysinfo(10)
        show_cpuinfo(10)
        show_meminfo(10)
#        show_diskinfo(10)
        show_clock(15)


if __name__ == "__main__":
    try:
        # serial = i2c(port=1, address=0x3C)
        serial = spi(bus_speed_hz=1000000, spi_mode=0)
        device = sh1106(serial, rotate=0)
        main()
    except DeviceNotFoundError:
        print('Cannot find SPI device')
        exit()
    except KeyboardInterrupt:
        device.cleanup()
        pass