#!/usr/bin/perl  

use Tinkerforge::IPConnection;
use Tinkerforge::BrickletTemperature;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'dVh'; # Change to your UID

my $ipcon = IPConnection->new(); # Create IP connection
my $t = BrickletTemperature->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Get current temperature (unit is °C/100)
my $temperature = $t->get_temperature()/100.0;

print "\nTemperature: ".$temperature." °C\n";

print "\nPress any key to exit...\n";
<STDIN>;
$ipcon->disconnect();

