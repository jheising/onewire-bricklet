#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Temperature Bricklet

# Get current temperature (unit is °C/100)
tinkerforge call temperature-bricklet $uid get-temperature
