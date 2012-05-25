#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_temperature'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new HOST, PORT # Create IP connection to brickd
t = BrickletTemperature.new UID # Create device object
ipcon.add_device t # Add device to IP connection
# Don't use device before it is added to a connection

# Get current temperature (unit is °C/100)
temperature = t.get_temperature / 100.0
puts "Temperature: #{temperature} °C"

puts 'Press key to exit'
$stdin.gets
ipcon.destroy
