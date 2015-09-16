#!/usr/bin/perl

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperature;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change to your UID

# Callback subroutine for temperature callback (parameter has unit °C/100)
sub cb_temperature
{
    my ($temperature) = @_;

    print "Temperature: " . $temperature/100.0 . " °C\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $t = Tinkerforge::BrickletTemperature->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register temperature callback to subroutine cb_temperature
$t->register_callback($t->CALLBACK_TEMPERATURE, 'cb_temperature');

# Set period for temperature callback to 1s (1000ms)
# Note: The temperature callback is only called every second
#       if the temperature has changed since the last call!
$t->set_temperature_callback_period(1000);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
