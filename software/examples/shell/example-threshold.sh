#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change to your UID

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
tinkerforge call temperature-bricklet $uid set-debounce-period 10000

# Handle incoming temperature reached callbacks (parameter has unit °C/100)
tinkerforge dispatch temperature-bricklet $uid temperature-reached\
 --execute "echo Temperature: {temperature} °C/100. It is too hot, we need air conditioning!" &

# Configure threshold for temperature "greater than 30 °C" (unit is °C/100)
tinkerforge call temperature-bricklet $uid set-temperature-callback-threshold greater 3000 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
