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

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
t.set_debounce_period 10000

# Register callback for temperature greater than 30 °C
t.register_callback(BrickletTemperature::CALLBACK_TEMPERATURE_REACHED) do |temperature|
  puts "We have #{temperature/100.0} °C."
  puts "It is too hot, we need air conditioning!"
end

# Configure threshold for "greater than 30 °C" (unit is °C/100)
t.set_temperature_callback_threshold '>', 30*100, 0

puts 'Press key to exit'
$stdin.gets
ipcon.destroy
