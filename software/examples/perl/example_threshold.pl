#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperature;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $t = Tinkerforge::BrickletTemperature->new(&UID, $ipcon); # Create device object

# Callback subroutine for temperature greater than 30 °C (parameter has unit °C/100)
sub cb_temperature_reached
{
    my ($temperature) = @_;

    print "Temperature: " . $temperature/100.0 . " °C\n";
    print "It is too hot, we need air conditioning!";
}

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$t->set_debounce_period(10000);

# Register threshold reached callback to subroutine cb_temperature_reached
$t->register_callback($t->CALLBACK_TEMPERATURE_REACHED, 'cb_temperature_reached');

# Configure threshold for "greater than 30 °C" (unit is °C/100)
$t->set_temperature_callback_threshold('>', 30*100, 0);

print "Press any key to exit...\n";
<STDIN>;
$ipcon->disconnect();
