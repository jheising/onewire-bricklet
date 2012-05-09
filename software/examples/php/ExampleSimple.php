<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperature.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperature;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$t = new BrickletTemperature($uid); // Create device object

$ipcon->addDevice($t); // Add device to IP connection
// Don't use device before it is added to a connection

// Get current temperature (unit is °C/100)
$temperature = $t->getTemperature() / 100.0;

echo "Temperature: $temperature °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->destroy();

?>
