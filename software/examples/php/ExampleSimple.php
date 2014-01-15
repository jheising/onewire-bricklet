<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperature.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperature;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change to your UID

$ipcon = new IPConnection(); // Create IP connection
$t = new BrickletTemperature(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current temperature (unit is °C/100)
$temperature = $t->getTemperature() / 100.0;

echo "Temperature: $temperature °C\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
