#!/usr/bin/env python
# -*- coding: utf-8 -*-

HOST = "localhost"
PORT = 4223
UID = "XYZ" # Change XYZ to the UID of your Temperature Bricklet

from tinkerforge.ip_connection import IPConnection
from tinkerforge.bricklet_temperature import BrickletTemperature

if __name__ == "__main__":
    ipcon = IPConnection() # Create IP connection
    t = BrickletTemperature(UID, ipcon) # Create device object

    ipcon.connect(HOST, PORT) # Connect to brickd
    # Don't use device before ipcon is connected

    # Get current temperature (unit is °C/100)
    temperature = t.get_temperature()
    print("Temperature: " + str(temperature/100.0) + " °C")

    raw_input("Press key to exit\n") # Use input() in Python 3
    ipcon.disconnect()
