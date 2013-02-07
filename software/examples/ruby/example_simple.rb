#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_temperature'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change to your UID

ipcon = IPConnection.new # Create IP connection
t = BrickletTemperature.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Get current temperature (unit is °C/100)
temperature = t.get_temperature / 100.0
puts "Temperature: #{temperature} °C"

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
