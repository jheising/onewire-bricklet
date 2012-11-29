<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperature.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperature;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$t = new BrickletTemperature($uid, $ipcon); // Create device object

$ipcon->connect($host, $port); // Connect to brickd
// Don't use device before ipcon is connected

// Get current temperature (unit is °C/100)
$temperature = $t->getTemperature() / 100.0;

echo "Temperature: $temperature °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));

?>
